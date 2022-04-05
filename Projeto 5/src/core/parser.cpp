/*!
 * Implementation of XML processing functions.
 * @file parser.h
 */
#include <iostream>
#include <cstring> //c_str
#include <typeinfo> //typeid
#include <sstream>
#include <exception>

#include "paramset.h"
#include "parser.h"
#include "api.h"
#include "geometric_primitive.h"
#include "sphere.h"
#include "flatmaterial.h"
#include "rt3.h"
#include "light/light.hpp"
#include "light/pointlight.hpp"
#include "light/spotlight.hpp"
#include "light/directionallight.hpp"
#include "light/ambientlight.hpp"
#include "blinnmaterial.hpp"

// === Function Implementation

namespace rt3 {

using rt3::Point3f;
using rt3::Vector3f;
using rt3::Vector3i;

//std::vector<ParamSet> objects;
//guarda material atual e atribui a cada primitiveObj
std::shared_ptr<Material> material;
std::vector<std::shared_ptr<GeometricPrimitive>> objects_;
std::vector<std::shared_ptr<Light>> lights_;
// conjunto de materiais nomeados criados
std::vector<std::shared_ptr<BlinnMaterial>> named_materials_;
//material nomeado atual
std::shared_ptr<BlinnMaterial> named_material;
std::string filename;
vec3<float> translate;

/// This is the entry function for the parsing process.
void parse( const char* scene_file_name )
{
    tinyxml2::XMLDocument xml_doc;

    // Load file.
    if ( xml_doc.LoadFile( scene_file_name ) != tinyxml2::XML_SUCCESS )
        RT3_ERROR( std::string{"The file \""} + scene_file_name + std::string{"\" either is not available OR contains an invalid RT3 scene provided!"} );

    // ===============================================
    // Get a pointer to the document's root node.
    tinyxml2::XMLNode * p_root = xml_doc.FirstChild();
    if ( p_root == nullptr )
        RT3_ERROR( "Error while trying to find \"RT3\" tag in the scene file." );
    // ===============================================

    // Get the first-level tag inside the root node.
    auto *p_child { p_root->FirstChildElement( ) };
    if ( p_child == nullptr)
        RT3_ERROR( "No \"children\" tags found inside the \"RT3\" tag. Empty scene file?" );

    parse_tags( p_child, /* initial level */ 0 );
}

/// Main loop that handles each possible tag we may find in a RT3 scene file.
void parse_tags(  tinyxml2::XMLElement *p_element, int level )
{
    // All messages sent to std::clog is for DEBUG purposes.
    std::clog << "[parse_tags()]: level is " << level << std::endl;

    // Traverse all items on the children's level. 
    while ( p_element != nullptr )
    {
        // Convert the attribute name to lowecase before testing it.
        auto tag_name = CSTR_LOWERCASE( p_element->Value() );
        clog << "\n"<< setw(level*3) << "" << "***** Tag id is `" << tag_name << "`, at level " << level << std::endl;

        // Big switch for each possible RT3 tag type.
        if(tag_name == "make_named_material")
        {
            ParamSet ps;

            // TODO: retrieve information from the XML child into the ParamSet.
            vector<std::pair<param_type_e, string>> param_list
            {
                { param_type_e::STRING  , "type"       },
                { param_type_e::STRING  , "name"       },
                { param_type_e::VEC3F   , "ambient"    },
                { param_type_e::VEC3F   , "diffuse"    },
                { param_type_e::VEC3F   , "specular"   },
                { param_type_e::VEC3F   , "mirror"     },
                { param_type_e::REAL   , "glossiness" }
            };
            parse_parameters( p_element, param_list, /* out */&ps );

            if(retrieve<std::string>(ps, "type") == "blinn")
            {
                std::string type = retrieve<std::string>(ps, "type");
                std::string name = retrieve<std::string>(ps, "name");
                vec3<float> ambient = retrieve<vec3<float>>(ps, "ambient");
                vec3<float> diffuse = retrieve<vec3<float>>(ps, "diffuse");
                vec3<float> specular = retrieve<vec3<float>>(ps, "specular");
                vec3<float> mirror = retrieve<vec3<float>>(ps, "mirror");
                float glossiness = retrieve<float>(ps, "glossiness");

                std::shared_ptr<BlinnMaterial> blinn_material = std::make_shared<BlinnMaterial>(diffuse, specular, ambient, type, name, mirror, glossiness); 
                named_materials_.push_back(blinn_material);
            }


            if(p_element->NextSiblingElement() == nullptr or 
            CSTR_LOWERCASE(p_element->NextSiblingElement()->Value()) != "make_named_material")
            {
            // Calling the corresponding API method.
                API::make_named_materials( named_materials_ );
                //named_materials_.clear();
            }
        }
        else if( tag_name == "named_material" )
        {
            ParamSet ps;

            vector<std::pair<param_type_e, string>> param_list
            {
                { param_type_e::STRING  , "name"      }
                
            };
            parse_parameters( p_element, param_list, /* out */&ps );
            
            std::string name = retrieve<std::string>(ps, "name");

            for(size_t i(0); i < named_materials_.size(); i++)
            {
                if(named_materials_[i]->get_name() == name)
                {
                    named_material = named_materials_[i]; 
                    break;
                }
            }


        }
        else if( tag_name == "translate" )
        {
            ParamSet ps;

            vector<std::pair<param_type_e, string>> param_list
            {
                { param_type_e::VEC3F  , "value"      }
                
            };
            parse_parameters( p_element, param_list, /* out */&ps );
            
            translate = retrieve<vec3<float>>(ps, "value");
        }   
        else if(tag_name == "light_source")
        {   
            ParamSet ps;

            // TODO: retrieve information from the XML child into the ParamSet.
            vector<std::pair<param_type_e, string>> param_list
            {
                { param_type_e::STRING  , "type"   },
                { param_type_e::VEC3F   , "L"      },
                { param_type_e::VEC3F   , "scale"  },
                { param_type_e::VEC3F   , "from"   },
                { param_type_e::VEC3F   , "to"     },
                { param_type_e::VEC3F   , "I"      },
                { param_type_e::REAL    , "cutoff" },
                { param_type_e::REAL    , "falloff"}
                
            };
            parse_parameters( p_element, param_list, /* out */&ps );

            //criar o objeto atual verificando o tipo (atualmente só sphere)
            //Atribuir o material atual ao objeto e adicionar a lista
            //Se a próxima tag não for object, chamar método e esvaziar lista 
            //objects.push_back(ps);

            if(retrieve<std::string>(ps, "type") == "ambient")
            {
                std::string type = retrieve<std::string>(ps, "type");
                vec3<float> L = retrieve<vec3<float>>(ps, "L");
                
                std::shared_ptr<AmbientLight> ambient = std::make_shared<AmbientLight>(type, L);
                API::setAmbientLight(ambient.get());
                lights_.push_back(ambient);
            }
            else if(retrieve<std::string>(ps, "type") == "directional")
            {
                std::string type = retrieve<std::string>(ps, "type");
                vec3<float> L = retrieve<vec3<float>>(ps, "L");
                vec3<float> scale = retrieve<vec3<float>>(ps, "scale");
                vec3<float> from = retrieve<vec3<float>>(ps, "from");
                vec3<float> to = retrieve<vec3<float>>(ps, "to");

                std::shared_ptr<DirectionalLight> directional = std::make_shared<DirectionalLight>(type, L, scale, from, to);
                lights_.push_back(directional);
            }
            else if(retrieve<std::string>(ps, "type") == "point")
            {
                std::string type = retrieve<std::string>(ps, "type");
                vec3<float> I = retrieve<vec3<float>>(ps, "I");
                vec3<float> scale = retrieve<vec3<float>>(ps, "scale");
                vec3<float> from = retrieve<vec3<float>>(ps, "from");

                std::shared_ptr<PointLight> point = std::make_shared<PointLight>(type, I, scale, from);
                lights_.push_back(point);   
            }
            else if(retrieve<std::string>(ps, "type") == "spot")
            {
                std::string type = retrieve<std::string>(ps, "type");
                vec3<float> I = retrieve<vec3<float>>(ps, "I");
                vec3<float> scale = retrieve<vec3<float>>(ps, "scale");
                vec3<float> from = retrieve<vec3<float>>(ps, "from");
                vec3<float> to = retrieve<vec3<float>>(ps, "to");
                float cutoff = retrieve<float>(ps, "cutoff");
                float falloff = retrieve<float>(ps, "falloff");

                std::shared_ptr<SpotLight> spot = std::make_shared<SpotLight>(type, I, scale, from, to, cutoff, falloff);
                lights_.push_back(spot);   
            }

            if(CSTR_LOWERCASE(p_element->NextSiblingElement()->Value()) != "light_source"){
            // Calling the corresponding API method.
                API::lights( lights_ );
                lights_.clear();
            }
        }
        else if( tag_name == "include" )
        {
            ParamSet ps;

            vector<std::pair<param_type_e, string>> param_list
            {
                { param_type_e::STRING  , "filename"      }
                
            };
            parse_parameters( p_element, param_list, /* out */&ps );
            filename = retrieve<std::string>(ps, "filename");
            parse(filename.c_str());
        }
        else if( tag_name == "integrator" )
        {   
            ParamSet ps;

            vector<std::pair<param_type_e, string>> param_list
            {
                { param_type_e::STRING  , "type"      },
                { param_type_e::INT     , "depth"     }
                
            };
            parse_parameters( p_element, param_list, /* out */&ps );

            // Calling the corresponding API method.
            API::integrator( ps );

        }
        else if( tag_name == "material" )
        {
            ParamSet ps;

            // TODO: retrieve information from the XML child into the ParamSet.
            vector<std::pair<param_type_e, string>> param_list
            {
                { param_type_e::STRING  , "type"      },
                { param_type_e::COLOR   , "color"     },
                { param_type_e::STRING  , "name"      },
                { param_type_e::VEC3F   , "ambient"   },
                { param_type_e::VEC3F   , "diffuse"   },
                { param_type_e::VEC3F   , "specular"  },
                { param_type_e::VEC3F   , "mirror"    },
                { param_type_e::REAL    , "glossiness"}

                
            };
            parse_parameters( p_element, param_list, /* out */&ps );

            //API::material( ps );

            if(retrieve<std::string>(ps, "type") == "flat")
            {
                vec3<float> color = retrieve<vec3<float>>(ps, "color");
                material = std::make_shared<FlatMaterial>(color);
                //std::cout << material->kd()[0] << " " << material->kd()[1] << " " << material->kd()[2] << "------------------\n";
            }
            else if(retrieve<std::string>(ps, "type") == "blinn")
            {
                std::string type = retrieve<std::string>(ps, "type");
                std::string name = retrieve<std::string>(ps, "name");
                vec3<float> ambient = retrieve<vec3<float>>(ps, "ambient");
                vec3<float> diffuse = retrieve<vec3<float>>(ps, "diffuse");
                vec3<float> specular = retrieve<vec3<float>>(ps, "specular");
                vec3<float> mirror = retrieve<vec3<float>>(ps, "mirror");
                float glossiness = retrieve<float>(ps, "glossiness");
                
                material = std::make_shared<BlinnMaterial>(diffuse, specular, ambient, type, name, mirror, glossiness);               
            }
        }
        else if( tag_name == "object" )
        {
            ParamSet ps;

            // TODO: retrieve information from the XML child into the ParamSet.
            vector<std::pair<param_type_e, string>> param_list
            {
                { param_type_e::STRING  , "type"      },
                { param_type_e::REAL    , "radius"     },
                { param_type_e::VEC3F   , "center"     },
                
            };
            parse_parameters( p_element, param_list, /* out */&ps );

            //criar o objeto atual verificando o tipo (atualmente só sphere)
            //Atribuir o material atual ao objeto e adicionar a lista
            //Se a próxima tag não for object, chamar método e esvaziar lista 
            //objects.push_back(ps);

            if(retrieve<std::string>(ps, "type") == "sphere")
            {
                float radius = retrieve<float>(ps, "radius");
                vec3<float> center = retrieve<vec3<float>>(ps, "center");
                std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(center, radius);

                std::shared_ptr<GeometricPrimitive> gp = std::make_shared<GeometricPrimitive>();
                gp->set_shape(sphere);
                gp->set_material(material);
                objects_.push_back(gp);

                //std::cout << "\n-- " << radius << "\n";
            }
            else if(named_material != nullptr)
            {
                float radius = retrieve<float>(ps, "radius");
                vec3<float> center = translate;
                std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(center, radius);

                std::shared_ptr<GeometricPrimitive> gp = std::make_shared<GeometricPrimitive>();
                gp->set_shape(sphere);
                gp->set_material(named_material);
                objects_.push_back(gp);                
            }
    
            if(p_element->NextSiblingElement() == nullptr)
            {
                API::object( objects_ );
                objects_.clear();
            }
            else if(CSTR_LOWERCASE(p_element->NextSiblingElement()->Value()) != "identity")
            {
            // Calling the corresponding API method.
                API::object( objects_ );
                objects_.clear();
            }
            else if(CSTR_LOWERCASE(p_element->NextSiblingElement()->Value()) != "object")
            {
            // Calling the corresponding API method.
                API::object( objects_ );
                objects_.clear();
            }
        }
        else if( tag_name == "lookat" )
        {
            ParamSet ps;

            // TODO: retrieve information from the XML child into the ParamSet.
            vector<std::pair<param_type_e, string>> param_list
            {
                { param_type_e::VEC3F  , "look_from"      },
                { param_type_e::VEC3F  , "look_at"        },
                { param_type_e::VEC3F  , "up"             }
                
            };
            parse_parameters( p_element, param_list, /* out */&ps );

            // Calling the corresponding API method.
            API::lookat( ps );
        }
        else if( tag_name == "camera" )
        {
            ParamSet ps;

            // TODO: retrieve information from the XML child into the ParamSet.
            vector<std::pair<param_type_e, string>> param_list
            {
                { param_type_e::STRING    , "type"              },
                { param_type_e::ARR_REAL  , "screen_window"     },
                { param_type_e::REAL       , "fovy"              }
                
            };
            parse_parameters( p_element, param_list, /* out */&ps );

            // Calling the corresponding API method.
            API::camera( ps );
        }
        else if ( tag_name == "background" )
        {
            ParamSet ps;

            // TODO: retrieve information from the XML child into the ParamSet.
            vector<std::pair<param_type_e, string>> param_list
            {
                { param_type_e::STRING  , "type"        },
                { param_type_e::STRING  , "filename"    }, // Texture file name.
                { param_type_e::STRING  , "mapping"     }, // Type of mapping required.
                { param_type_e::COLOR   , "color"       }, // Single color for the entire background.
                { param_type_e::COLOR   , "tl"          }, // Top-left corner
                { param_type_e::COLOR   , "tr"          }, // Top-right corner
                { param_type_e::COLOR   , "bl"          }, // Bottom-left corner
                { param_type_e::COLOR   , "br"          }  // Bottom-right corner
            };
            parse_parameters( p_element, param_list, /* out */&ps );

            // Calling the corresponding API method.
            API::background( ps );
        }
        else if ( tag_name == "film" )
        {
            ParamSet ps;

            // TODO: retrieve information from the XML child into the ParamSet.
            vector<std::pair<param_type_e, string>> param_list
            {
                { param_type_e::STRING   , "type"        },
                { param_type_e::STRING   , "filename"    },
                { param_type_e::STRING   , "img_type"    },
                { param_type_e::INT      , "x_res"       },
                { param_type_e::INT      , "y_res"       },
                { param_type_e::ARR_REAL , "crop_window" },
                { param_type_e::STRING   , "gamma_corrected" } // bool
            };
            parse_parameters( p_element, param_list, /* out */&ps );

            // Calling the corresponding API method.
            API::film( ps );
        }
        else if ( tag_name == "world_begin" )
        {
            //std::clog << ">>> Entering WorldBegin, at level " << level+1 << std::endl;
            // We should get only one `world` tag per scene file.
            API::world_begin();
        }
        else if ( tag_name == "world_end" )
        {
            API::world_end();
            //std::clog << ">>> Leaving WorldBegin, at level " << level+1 << std::endl;
        }
        //else RT3_WARNING( "Undefined tag `" + tag_name + "` found!" );

        // Get next (to the right) sibling on this tree level.
        p_element = p_element->NextSiblingElement();
    }
}

/// Universal parameters parser.
/*!
 * This function receives a list of pairs <param_type, name>, traverse all the attributes found
 * in `p_element` and extract the attribute values into the `ps_out` `ParamSet` object.
 * Only named attributes found are read into the `ps_out`.
 *
 * @param p_element XML element we are extracting information from.
 * @param param_list List of pairs <param_type, name> we need to extract from the XML element.
 * @param ps_out The `ParamSet` object we need to fill in with parameter information extracted from the XML element.
 */
void parse_parameters( tinyxml2::XMLElement * p_element,
        const vector<std::pair<param_type_e, string>> param_list, ParamSet *ps_out )
{
    //std::clog << "parse_parameters(): p_element = " << p_element << endl;

    // Traverse the list of paramters pairs: type + name.
    for ( const auto& e : param_list )
    {
        const auto & [ type, name ] = e; // structured binding, requires C++ 17
        std::clog << "---Parsing att \"" << name << "\", type = " << (int)type << "\n";

        // This is just a dispatcher to the proper extraction functions.
        switch ( type )
        {
            // TODO: Provide code or function call to each case you need at this point.
            //=============================================================================
            // ATTENTION: We do not parse bool from the XML file because TinyXML2 cannot parse one.
            // Bools are treated as strings.
            // case param_type_e::BOOL:
                //parse_single_BASIC_attrib<std::string>( p_element, ps_out, name );
                // break;
            //=============================================================================
            case param_type_e::UINT:
                parse_single_BASIC_attrib<uint>( p_element, ps_out, name );
                break;
            case param_type_e::INT:
                parse_single_BASIC_attrib<int>( p_element, ps_out, name );
                break;
            case param_type_e::REAL:
                parse_single_BASIC_attrib<float>( p_element, ps_out, name );
                break;
            case param_type_e::STRING:
                parse_single_BASIC_attrib<std::string>( p_element, ps_out, name );
                break;
            case param_type_e::VEC3F:
                parse_vec<float>( p_element, ps_out, name );
                break;
            case param_type_e::VEC3I:
                break;
            case param_type_e::NORMAL3F:
                break;
            case param_type_e::POINT3F:
                break;
            // case param_type_e::POINT2I:
                // parse_single_COMPOSITE_attrib<int, Point2i, int(2)>( p_element, ps_out, name );
                // break;
            case param_type_e::COLOR:
                parse_vec<float>( p_element, ps_out, name );
                break;
            case param_type_e::SPECTRUM:
                break;
            case param_type_e::ARR_REAL:
                parse_array<float>( p_element, ps_out, name );
                break;
            case param_type_e::ARR_INT:
                break;
            case param_type_e::ARR_VEC3F:
                break;
            case param_type_e::ARR_VEC3I:
                break;
            case param_type_e::ARR_NORMAL3F:
                break;
            case param_type_e::ARR_POINT3F:
                break;
            case param_type_e::ARR_COLOR:
                break;
            default:
                RT3_WARNING( string{"parse_params(): unkonwn param type received!" } );
                break;
        }
        clog << "---Done!\n";
    }
}

//Converte de string para outros tipos numericos
template<typename T>
T getValue(std::string name) {
    T result;
    std::istringstream buffer(name);
    
    buffer >> result;
    return result;
}

template<typename T>
vec3<T> getValueSeq(std::string name) {
    vec3<T> result;
    std::istringstream buffer(name);
    int i(0);
    T value;
    while(buffer >> value){
        result.e[i] = value;
        i++;
    }

    return result;
}

template<typename T>
std::vector<T> getValueVector(std::string name) {
    std::vector<T> result;
    std::istringstream buffer(name);
    T value;
    while(buffer >> value){
        result.push_back(value);
    }

    return result;
}

//Função generica para pegar os atributos do nó considerado e salvar no dicionario.
template<typename T>
void parse_single_BASIC_attrib( tinyxml2::XMLElement* p_element, ParamSet* ps_out, std::string name ){
    T value;
    std::string temp = name;
    const char* nm = temp.c_str();
    nm = p_element->Attribute(nm);         //p_element->QueryAttribute(nm, &value);
    if(nm != nullptr){
        value = getValue<T>(std::string(nm));
        ps_out->operator[](name) = std::make_shared<Value<T>>(value);
        //std::cout << rt3::retrieve<T>(paramSet, name) << std::endl;
    }
}

template<typename T>
void parse_vec( tinyxml2::XMLElement* p_element, ParamSet* ps_out, std::string name ){
    vec3<T> value;
    std::string temp = name;
    const char* nm = temp.c_str();
    nm = p_element->Attribute(nm);         //p_element->QueryAttribute(nm, &value);
    if(nm != nullptr){
        value = getValueSeq<T>(std::string(nm));
        ps_out->operator[](name) = std::make_shared<Value<vec3<T>>>(value);
    }
}

template<typename T>
void parse_array( tinyxml2::XMLElement* p_element, ParamSet* ps_out, std::string name ){
    std::vector<T> value;
    std::string temp = name;
    const char* nm = temp.c_str();
    nm = p_element->Attribute(nm);         //p_element->QueryAttribute(nm, &value);
    if(nm != nullptr){
        value = getValueVector<T>(std::string(nm));
        ps_out->operator[](name) = std::make_shared<Value<std::vector<T>>>(value);
    }
}



            

//-------------------------------------------------------------------------------

} // namespace rt3
