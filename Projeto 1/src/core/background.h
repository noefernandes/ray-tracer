#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "rt3.h"
#include "rt3-base.h"

namespace rt3 {
    // TODO: Create a virtual class Background and derive BackgroundColor, BackgroundSphereImage, BackgroundSkyBoxImage.
    /*!
     * A background is basically a rectangle, have a color associated to each corner.
     * A background might be sampled based on a normalized coordinate in \f$[0,1]^2.\f$.
     * We generate samples with bilinear interpolation of the corner colors.
     * We consider the corners in a circular clockwise order, starting at the lower-left corner.
     *
     * Alternatively, a background might be a regular image.
     */
    class Background
    {
        public:
            /// Types of color mapping scheme
            enum class mapping_t {
                screen=0,  //!< background is assigned to the image screen
                spherical  //!< background is assigne to a sphere surrounding the scene.
            };

        public:
            mapping_t mapping_type; //!< sampling type we use to pick a color.

            /// Ctro receiving a single color for the entire background.
            Background( mapping_t mt=mapping_t::spherical ): mapping_type{mt} {/* empty */}

            virtual ~Background(){ /* empty */ };
            //Spectrum sampleXYZ( const Point2f &pixel_ndc ) const;
            virtual Spectrum sampleXYZ( const Point2f &pixel_ndc ) const;
    };

    class BackgroundColor : public Background
    {
        private:
            /// Each corner has a color associated with.
            Spectrum corners[4]={{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
            /// Corner indices.
            enum Corners_e {
                bl=0, //!< Bottom left corner.
                tl,   //!< Top left corner.
                tr,   //!< Top right corner.
                br    //!< Bottom right corner.
            };

        public:

            /// Ctro receives a list of four colors, for each corner.
            BackgroundColor( vec3<float>& c1, vec3<float>& c2, vec3<float>& c3, vec3<float>& c4 )
            {
                // TODO
                corners[0][0] = c1.e[0]; corners[0][1] = c1.e[1]; corners[0][2] = c1.e[2];
                corners[1][0] = c2.e[0]; corners[1][1] = c2.e[1]; corners[1][2] = c2.e[2];
                corners[2][0] = c3.e[0]; corners[2][1] = c3.e[1]; corners[2][2] = c3.e[2];
                corners[3][0] = c4.e[0]; corners[3][1] = c4.e[1]; corners[3][2] = c4.e[2];
            }

            Spectrum sampleXYZ( const Point2f &pixel_ndc ) const;

            virtual ~BackgroundColor() { };
    };

    // factory pattern functions.
    BackgroundColor * create_color_background( const ParamSet &ps );
}
#endif
