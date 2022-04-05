#include "film.h"
#include "paramset.h"
#include "image_io.h"
#include "api.h"

#include <cmath>

namespace rt3 {

    //=== Film Method Definitions
    Film::Film( const Point2i &resolution, const std::string &filename , image_type_e imgt ) :
        m_full_resolution{resolution},
        m_filename{filename},
        image_type{ imgt }
    {
        // TODO
        m_color_buffer_ptr = std::make_unique<ColorBuffer>(resolution[0], resolution[1]);
    }

    Film::~Film()
    {
    }

    /// Add the color to image.
    void Film::add_sample( const Point2f &pixel_coord, const ColorXYZ &pixel_color )
    {
        // TODO: add color to the proper location.
        int w = m_color_buffer_ptr->w;
        m_color_buffer_ptr->data[w*int(pixel_coord[1])*3 + int(pixel_coord[0])*3 + 0] = pixel_color[0];
        m_color_buffer_ptr->data[w*int(pixel_coord[1])*3 + int(pixel_coord[0])*3 + 1] = pixel_color[1];
        m_color_buffer_ptr->data[w*int(pixel_coord[1])*3 + int(pixel_coord[0])*3 + 2] = pixel_color[2];
    }

    /// Convert image to RGB, compute final pixel values, write image.
    void Film::write_image(void) const
    {
        // TODO: call the proper writing function, either PPM or PNG.
        int w = m_color_buffer_ptr->w;
        int h = m_color_buffer_ptr->h;
        
        if(image_type == Film::image_type_e::PNG)
            save_png( m_color_buffer_ptr->data, w, h, 3, m_filename );
        else if(image_type == Film::image_type_e::PPM3)
            save_ppm3( m_color_buffer_ptr->data, w, h, 3, m_filename );
        else if(image_type == Film::image_type_e::PPM6)
            save_ppm6( m_color_buffer_ptr->data, w, h, 3, m_filename );

    }
    
    // Factory function pattern.
    // This is the function that retrieves from the ParamSet object
    // all the information we need to create a Film object.
    Film * create_film( const ParamSet &ps )
    {
        std::cout << ">>> Inside create_film()\n";
        std::string filename;
        // Let us check whether user has provided an output file name via
        // command line arguments in main().
        if ( API::curr_run_opt.outfile != "")
        {
            // Retrieve filename defined via CLI.
            filename = API::curr_run_opt.outfile;
            // Retrieve another filename, now from the ParamSet object.
            std::string filename_from_file = retrieve( ps, "filename", std::string{""} );
            if ( filename_from_file != "") // We also get a filename from scene file...
                RT3_WARNING( string{"Output filename supplied on command line, \""} +
                         API::curr_run_opt.outfile +
                         string{"\" is overriding filename provided in scene description file, \""} +
                         filename_from_file );
        }
        else
        {
            // Try yo retrieve filename from scene file.
            filename = retrieve( ps, "filename", std::string{"image.png"} );
        }

        // Read resolution.
        // Aux function that retrieves info from the ParamSet.
        int xres = retrieve( ps, "x_res", int(1280) );
        // Aux function that retrieves info from the ParamSet.
        int yres = retrieve( ps, "y_res", int(720) );
        // Quick render?
        if ( API::curr_run_opt.quick_render == true)
        {
            // decrease resolution.
            xres = std::max(1, xres / 4);
            yres = std::max(1, yres / 4);
        }


        // TODO
        // Read crop window information.
        //vec3<float> crop = rt3::retrieve<vec3<float>>(ps, "crop_window");
        auto result = ps.find( "crop_window" );
        if ( result != ps.end() )
        {
            vec3<float> crop_window = rt3::retrieve<vec3<float>>(ps, "crop_window");
        }

        Film::image_type_e img_type;
        auto result2 = ps.find( "img_type" );
        
        if ( result2 != ps.end() )
        {
            std::string img_type_str = rt3::retrieve<std::string>(ps, "img_type");
            if(img_type_str == "png")
            {
                img_type = Film::image_type_e::PNG;
            }
            else if(img_type_str == "ppm3")
            {
                img_type = Film::image_type_e::PPM3;
            }
            else if(img_type_str == "ppm6")
            {
                img_type = Film::image_type_e::PNG;
            }
        }

        // Note that the image type is fixed here. Must be read from ParamSet, though.
        //return new Film( Point2i{ xres, yres}, filename, Film::image_type_e::PNG );
        return new Film( Point2i{ xres, yres}, filename, img_type );
    }
}  // namespace pbrt
