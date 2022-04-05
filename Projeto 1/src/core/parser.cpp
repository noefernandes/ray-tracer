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
#include "rt3.h"

// === Function Implementation

namespace rt3 {

using rt3::Point3f;
using rt3::Vector3f;
using rt3::Vector3i;

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
        if ( tag_name == "background" )
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

            

//-------------------------------------------------------------------------------

} // namespace rt3
