#include "background.h"

namespace rt3 {


    /*!
     * Samples a color base on spherical interpolation of an image ou colored background.
     *
     * @param pixel_ndc Pixel position in NDC space,  in \f$[0:1]\f$.
     * \return The interpolated color.
     */
    Spectrum Background::sampleXYZ( const Point2f &pixel_ndc ) const
    {
        // TODO
        return Spectrum{0,0,0};
    }

    /*float bilinear(float x, float y, float z01, float z00, float z11, float z10){
        return z00*(1-x)*(1-y) + z10*x*(1-y) + z01*(1-x)*y + z11*x*y;
    }


    Spectrum BackgroundColor::sampleXYZ( const Point2f &pixel_ndc ) const
    {
        // TODO
        float x = pixel_ndc[0];
        float y = pixel_ndc[1];
        //std::cout << x << " " << y << " ++++++++++++++++++++++++++++++++++++++++\n";

        float z00r = corners[0][0]; float z00g = corners[0][1]; float z00b = corners[0][2];
        //std::cout << z00r << " " << z00g << " " << z00b << "---------------------------------------------\n";
        float z01r = corners[1][0]; float z01g = corners[1][1]; float z01b = corners[1][2];
        //std::cout << z01r << " " << z01g << " " << z01b << "---------------------------------------------\n";
        float z11r = corners[2][0]; float z11g = corners[2][1]; float z11b = corners[2][2];
        //std::cout << z11r << " " << z11g << " " << z11b << "---------------------------------------------\n";
        float z10r = corners[3][0]; float z10g = corners[3][1]; float z10b = corners[3][2];
        //std::cout << z10r << " " << z10g << " " << z10b << "---------------------------------------------\n";  

        float r = bilinear(x, y, z00r, z01r, z10r, z11r);
        float g = bilinear(x, y, z00g, z01g, z10g, z11g);
        float b = bilinear(x, y, z00b, z01b, z10b, z11b);

        //std::cout << r << " " << g << " " << b << "---------------------------------------------\n";

        return Spectrum{r, g, b};
    }*/

    Spectrum BackgroundColor::sampleXYZ( const Point2f &pixel_ndc ) const{
        float x = pixel_ndc[0];
        float y = pixel_ndc[1];

        float v1_ = bl.e[0]*(1-x)*(1-y) + br.e[0]*x*(1-y) + tl.e[0]*(1-x)*y + tr.e[0]*x*y;
        float v2_ = bl.e[1]*(1-x)*(1-y) + br.e[1]*x*(1-y) + tl.e[1]*(1-x)*y + tr.e[1]*x*y;
        float v3_ = bl.e[2]*(1-x)*(1-y) + br.e[2]*x*(1-y) + tl.e[2]*(1-x)*y + tr.e[2]*x*y;

        if(bl.e[0] < 1.0){
            v1_ *= 255;
            v2_ *= 255;
            v3_ *= 255;
        }

        return vec3<float>(v1_, v2_, v3_);
    }


    BackgroundColor * create_color_background( const ParamSet &ps )
    {
        // TODO
        auto result = ps.find( "color" );
        if ( result != ps.end() ){
            vec3<float> c = rt3::retrieve<vec3<float>>(ps, "color");
            return new BackgroundColor( c, c, c, c );
        }

        vec3<float> bl_ = rt3::retrieve<vec3<float>>(ps, "bl");
        vec3<float> tl_ = rt3::retrieve<vec3<float>>(ps, "tl");
        vec3<float> tr_ = rt3::retrieve<vec3<float>>(ps, "tr");
        vec3<float> br_ = rt3::retrieve<vec3<float>>(ps, "br");
        return new BackgroundColor( bl_, tl_, tr_, br_ );

        //Falta adicionar outros tipos de background, verificando a partir do paramset
        //qual foi o tipo escolhido no atributo mapping
    }
}
