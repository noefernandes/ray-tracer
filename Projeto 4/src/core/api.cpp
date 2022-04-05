#include "api.h"

namespace rt3 {
            
//=== API's static members declaration and initialization.
API::APIState API::curr_state = APIState::Uninitialized;
RunningOptions API::curr_run_opt;
unique_ptr< RenderOptions > API::render_opt;
// GraphicsState API::curr_GS;

// THESE FUNCTIONS ARE NEEDED ONLY IN THIS SOURCE FILE (NO HEADER NECESSARY)
// ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ


Film * API::make_film( const std::string &type, const ParamSet &ps )
{
    std::cout << ">>> Inside API::make_film()\n";
    Film *film{ nullptr };
    film = create_film( ps );

    film->write_image();
    // Return the newly created film.
    return film;
}

Background * make_background( const std::string &type, const ParamSet& ps )
{
    std::cout << ">>> Inside API::background()\n";
    Background *bkg{ nullptr };
    bkg = create_color_background( ps );

    //colocado para teste: bkg->sampleXYZ(Point2f(0.2, 0.6));

    // Return the newly created background.
    return bkg;
}

//Coloquei agora
Camera * make_camera( const std::string &type, const ParamSet& ps_camera, const ParamSet& ps_lookat,  std::shared_ptr<Film> film )
{
    std::cout << ">>> Inside API::camera()\n";
    std::string type_cam = retrieve( ps_camera, "type", std::string{"unknown"} );

    Camera* camera( nullptr );

    if(type_cam == "orthographic"){
        camera = create_orthographic_camera( ps_camera, ps_lookat, film );
    }
    else if(type_cam == "perspective"){
        camera = create_perspective_camera( ps_camera, ps_lookat, film );
    }

    //colocado para teste: bkg->sampleXYZ(Point2f(0.2, 0.6));

    // Return the newly created background.
    return camera;
}

Integrator* make_integrator( const std::string& type, const ParamSet& ps, std::shared_ptr<Camera> camera )
{
    std::cout << ">>> Inside API::integrator()\n";
    std::string type_integ = retrieve( ps, "type", std::string{"unknown"} );

    Integrator* integrator( nullptr );

    if(type_integ == "flat"){
        integrator = create_flat_integrator( ps, camera );
    }

    return integrator;
}

// ˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆ

// END OF THE AUXILIARY FUNCTIONS
// =========================================================================

//=== API's public methods implementation
void API::init_engine( const RunningOptions & opt )
{
   // Save running option sent from the main().
   curr_run_opt = opt;
   // Check current machine state.
   if ( curr_state != APIState::Uninitialized )
       RT3_ERROR( "API::init_engine() has already been called! ");
   // Set proper machine state
   curr_state = APIState::SetupBlock;
   // Preprare render infrastructure for a new scene.
   render_opt = make_unique< RenderOptions >(); 
   // Create a new initial GS
   // curr_GS = GraphicsState();
   RT3_MESSAGE( "[1] Rendering engine initiated.\n" );
}

void API::clean_up( void )
{
    // Check for correct machine state
    if ( curr_state == APIState::Uninitialized )
        RT3_ERROR("API::clean_up() called before engine initialization.");
    else if ( curr_state == APIState::WorldBlock )
        RT3_ERROR("API::clean_up() called inside world definition section.");
    curr_state = APIState::Uninitialized;

    RT3_MESSAGE( "[4] Rendering engine clean up concluded. Shutting down...\n" );
}

void API::run( void )
{
    // Try to load and parse the scene from a file.
    RT3_MESSAGE( "[2] Beginning scene file parsing...\n" );
    // Recall that the file name comes from the running option struct.
    parse( curr_run_opt.filename.c_str() );
}


void API::world_begin( void )
{
    VERIFY_SETUP_BLOCK("API::world_begin");      // check for correct machine state.
    curr_state = APIState::WorldBlock;      // correct machine state.
}

void API::world_end( void )
{

    VERIFY_WORLD_BLOCK("API::world_end");
    // The scene has been properly set up and the scene has
    // already been parsed. It's time to render the scene.

    // At this point, we have the background as a solitary pointer here.
    // In the future, the background will be parte of the scene object.
    shared_ptr<Background> the_background{ make_background(render_opt->bkg_type, render_opt->bkg_ps) };
    // Same with the film, that later on will belong to a camera object.
    shared_ptr<Film> the_film{ make_film(render_opt->film_type, render_opt->film_ps) };

    shared_ptr<Camera> the_camera{ make_camera(render_opt->camera_type, render_opt->camera_ps, 
                                        render_opt->lookat_ps, the_film) };
    
    the_camera->setHeightWidth(the_film->getFilmHeight(), the_film->getFilmWidth());
    //-----------------

    shared_ptr<Integrator> the_integrator{ make_integrator(render_opt->integ_type, render_opt->integ_ps, the_camera) };

    shared_ptr<Scene> the_scene = std::make_shared<Scene>(the_background, render_opt->objects);

    if( the_integrator and the_background )
    {
        RT3_MESSAGE( "    Parsing scene successfuly done!\n" );
        RT3_MESSAGE( "[2] Starting ray tracing progress.\n" );

        // Structure biding, c++17.
        auto res  = the_film->get_resolution();
        int w = res[0];
        int h = res[1];
        RT3_MESSAGE( "    Image dimensions in pixels (W x H): " + std::to_string(w) + " x " + std::to_string(h) + ".\n" );
        RT3_MESSAGE( "    Ray tracing is usually a slow process, please be patient: \n" );

        //================================================================================
        auto start = std::chrono::steady_clock::now();

        the_integrator->render(the_scene);
           
        auto end = std::chrono::steady_clock::now();
        //================================================================================
        auto diff = end - start; //Store the time difference between start and end
        // Seconds
        auto diff_sec = std::chrono::duration_cast<std::chrono::seconds>(diff);
        RT3_MESSAGE( "    Time elapsed: " + std::to_string(diff_sec.count()) + " seconds (" +
                 std::to_string( std::chrono::duration <double, std::milli> (diff).count() ) +  " ms) \n" );   
    }

    // Run only if we got film and background.
    /*if ( the_film and the_background )
    {
        RT3_MESSAGE( "    Parsing scene successfuly done!\n" );
        RT3_MESSAGE( "[2] Starting ray tracing progress.\n" );

        // Structure biding, c++17.
        auto res  = the_film->get_resolution();
        int w = res[0];
        int h = res[1];
        RT3_MESSAGE( "    Image dimensions in pixels (W x H): " + std::to_string(w) + " x " + std::to_string(h) + ".\n" );
        RT3_MESSAGE( "    Ray tracing is usually a slow process, please be patient: \n" );

        //================================================================================
        auto start = std::chrono::steady_clock::now();
        //render( ); // TODO: This is the ray tracer's  main loop.

        //int count = 0;
        // Traverse all pixels to shoot rays from.
        for ( int j = 0 ; j < h; j++ ) {
            for( int i = 0 ; i < w ; i++ ) {
                Ray ray = the_camera->generate_ray(i, j);
                //ray.show();
                ColorXYZ color = the_background->sampleXYZ( Point2f{float(i)/float(w), float(j)/float(h)} ); // get background color.


                for(size_t k = 0; k < render_opt->objects.size(); k++){
                    std::shared_ptr<GeometricPrimitive> obj = render_opt->objects[k];
                    if(obj->get_shape()->intersect_p(ray)){
                        color = obj->get_material()->kd();
                    }
                }

                the_film->add_sample( Point2f{float(i), float(h-j)}, color ); // set image buffer at position (i,j), accordingly.
            }
        }

        the_film->write_image();






        auto end = std::chrono::steady_clock::now();
        //================================================================================
        auto diff = end - start; //Store the time difference between start and end
        // Seconds
        auto diff_sec = std::chrono::duration_cast<std::chrono::seconds>(diff);
        RT3_MESSAGE( "    Time elapsed: " + std::to_string(diff_sec.count()) + " seconds (" +
                 std::to_string( std::chrono::duration <double, std::milli> (diff).count() ) +  " ms) \n" );
    }*/
    // [4] Basic clean up
    curr_state = APIState::SetupBlock;      // correct machine state.
    reset_engine();
}



/// This api function is called when we need to re-render the *same* scene (i.e. objects, lights, materials, etc) , maybe with different integrator, and camera setup.
/// Hard reset on the engine. User needs to setup all entities, such as camera, integrator, accelerator, etc.
void API::reset_engine(void)
{
    // curr_GS = GraphicsState();
    // This will delete all information on integrator, cameras, filters,
    // acceleration structures, etc., that has been set previously.
    render_opt.reset( new RenderOptions );
}

void API::background( const ParamSet& ps )
{
    std::cout << ">>> Inside API::background()\n";
    VERIFY_WORLD_BLOCK("API::background");

    // retrieve type from ps.
    string type = retrieve( ps, "type", string{"unknown"} );
    render_opt->bkg_type = type;
    // Store current background object.
    render_opt->bkg_ps = ps;
}

void API::material( const ParamSet& ps )
{
    std::cout << ">>> Inside API::material()\n";
    VERIFY_WORLD_BLOCK("API::material");

    // retrieve type from ps.
    string type = retrieve( ps, "type", string{"unknown"} );
    render_opt->material_type = type;
    // Store current material object.
    render_opt->material_ps = ps;
}

void API::object( const std::vector<std::shared_ptr<GeometricPrimitive>> objects_ )
{
    std::cout << ">>> Inside API::object()\n";
    VERIFY_WORLD_BLOCK("API::object");

    // retrieve type from ps.
    //string type = retrieve( ps, "type", string{"unknown"} );
    //render_opt->material_type = type;
    // Store current material object.
    render_opt->objects.insert(render_opt->objects.end(), objects_.begin(), objects_.end());
}

void API::film( const ParamSet &ps )
{
    std::cout << ">>> Inside API::film()\n";
    VERIFY_SETUP_BLOCK("API::film");

    // retrieve type from ps.
    string type = retrieve( ps, "type", string{"unknown"} );
    render_opt->film_type = type;
    render_opt->film_ps = ps;
}

void API::lookat( const ParamSet &ps )
{
    std::cout << ">>> Inside API::lookat()\n";
    VERIFY_SETUP_BLOCK("API::lookat");
    
    render_opt->lookat_ps = ps;
}

void API::camera( const ParamSet &ps )
{
    std::cout << ">>> Inside API::camera()\n";
    VERIFY_SETUP_BLOCK("API::camera");

    // retrieve type from ps.
    string type = retrieve( ps, "type", string{"unknown"} );
    render_opt->camera_type = type;
    render_opt->camera_ps = ps;
}

void API::integrator( const ParamSet &ps )
{
    std::cout << ">>> Inside API::integrator()\n";
    VERIFY_SETUP_BLOCK("API::integrator");

    // retrieve type from ps.
    string type = retrieve( ps, "type", string{"unknown"} );
    render_opt->integ_type = type;
    render_opt->integ_ps = ps;
}

}  // namespace rt3
