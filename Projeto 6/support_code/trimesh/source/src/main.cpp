/// @file main.cpp

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include "rt3.h"
#include "triangle.h"

int main(void)
{
    // Here I'm "simulating" a ParamSet and manually filling in that object
    // with information about the triangle mesh. In particular, I'm creating
    // a situation in which the XML file has the following tag.
    // <object type="trianglemesh" filename="source/data/plane.obj" back_face_cull="on"
    //  reverse_vertex_order="false" compute_normals="fase"\>
    //
    ParamSet ps;
    ps["filename"] = "source/data/teapot-low.obj";
    // ps["filename"] = "source/data/plane.obj";
    ps["backface_cull"] = "on";
    ps["reverse_vertex_order"] = "false";
    ps["compute_normals"] = "false";
    bool flip_normals{false}; // This info comes from an API command.

    // Create triangle mesh. Entry-point for our mesh handling code.
    auto tri_list = create_triangle_mesh_shape( flip_normals, ps ); 

    std::cout << "\n\n>>> Accessing mesh data:\n";

    // DEBUG: Printing out the triangles.
    size_t tri_count{0};
    for ( const auto & tri : tri_list )
    {
        std::cout << "Triangle #" << ++tri_count << '\n';
        // Cast shape back to a triangle.
        const Triangle* pt_tri = dynamic_cast< Triangle* >( tri.get() );
        std::cout << *pt_tri << std::endl;
    }

    return 0;
}
