/// @file triangle.cpp

#ifndef TRIANGLE_H 
#define TRIANGLE_H

#define TINYOBJLOADER_IMPLEMENTATION

#include "rt3.h"
#include "shape.h"
#include "surfel.h"
#include "rt3.h"
#include "paramset.h"
#include "../ext/tiny_obj_loader.h"
#include <chrono>

class Surfel;
class Shape;

using namespace std;
using rt3::ParamSet;
using rt3::Point3f;
using rt3::Normal3f;
using rt3::Point2f;

/// This struct implements an indexd triangle mesh database.
struct TriangleMesh {
    int n_triangles; //!< # of triangles in the mesh.
    // The size of the three lists below should be 3 * nTriangles. Every 3 values we have a triangle.
    vector< int > vertex_indices;  //!< The list of indices to the vertex list, for each individual triangle.
    vector< int > normal_indices;  //!< The list of indices to the normals list, for each individual triangle.
    vector< int > uvcoord_indices; //!< The list of indices to the UV coord list, for each individual triangle.

    // Here comes the data associated with each vertex. WARNING: The length of the vectors below may differ.
    vector< Point3f > vertices;  //!< The 3D geometric coordinates
    vector< Normal3f > normals;  //!< The 3D normals.
    vector< Point2f > uvcoords; //!< The 2D texture coordinates.

    // Regular constructor (almost all data is imported via move semantics).
    TriangleMesh(){/*empty*/};
    /// Copy constructor disabled
    TriangleMesh( const TriangleMesh& ) = delete;
    /// Assign operator disabled.
    TriangleMesh& operator=( const TriangleMesh & ) = delete;
    /// Move constructor.
    TriangleMesh( TriangleMesh && other ) = delete;
};


/// Represents a single triangle.
class Triangle : public Shape {
	private:
		int *v; //!< Pointer to the vertex index array stored in the triangle mesh database. 
		// After the proper triangle initialization, we may access vertices with v[0], v[1], and v[2]
		int *n; //!< Pointer to the normal index array stored in the triangle mesh database.
		// After the proper triangle initialization, we may access normals with n[0], n[1], and n[2]
		int *uv;//!< Pointer to the UV coord index array stored in the triangle mesh database.
		// After the proper triangle initialization, we may access UV coords with uv[0], uv[1], and uv[2]

		shared_ptr<TriangleMesh> mesh; //!< This is the **indexed triangle mesh database** this triangle is linked to.
		bool backface_cull;            //!< Set it `true` to activate backface culling.
	
	public:
        // The single constructor, that receives the mesh, this triangle id, and an indication for backface culling.
		Triangle( shared_ptr<TriangleMesh> mesh, int tri_id, bool bfc=true )
			: Shape(), mesh{mesh}, backface_cull{bfc}
		{
			// This is just a shortcut to access this triangle's data stored in the mesh database.
			v  = &mesh->vertex_indices[ 3 * tri_id ];
			n  = &mesh->normal_indices[ 3 * tri_id ];
			uv = &mesh->uvcoord_indices[ 3 * tri_id ];
		}
		/// Return the triangle's bounding box.
		// Bounds3f object_bound() const;
        /// The regular intersection methods, as defined in the Shape parent class.
		bool intersect(Ray &ray, float *thit, Surfel *isect ){return true;}
		bool intersect_p( Ray &ray ){return true;}

        /// This friend function helps us debug the triangles, if we want to.
        friend std::ostream& operator<<( std::ostream& os, const Triangle & t );
};

/// This is the entry point for the client. This function begins the process of reading a triangle mesh.
vector<shared_ptr<Shape>>
create_triangle_mesh_shape( bool flip_normals, const ParamSet &ps );

/// This is the function that actually creates the mesh database and the triangles, ans store them in a Shape list.
vector<shared_ptr<Shape>> create_triangle_mesh( shared_ptr<TriangleMesh> , bool );

/// Internal function that calls the tinyobjloader api to read the OBJ data into memory.
bool load_mesh_data( const std::string & filename, bool rvo, bool cn, bool fn, shared_ptr<TriangleMesh> md );

/// This function essentially convertes the mesh data from the tinyobjloader internal data structure to our mesh data structure.
void extract_obj_data( const tinyobj::attrib_t& attrib,
                       const std::vector<tinyobj::shape_t>& shapes,
                       bool rvo, bool cn, bool fn, shared_ptr<TriangleMesh> md );


#endif
