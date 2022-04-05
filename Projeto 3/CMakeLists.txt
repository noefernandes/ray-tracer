cmake_minimum_required(VERSION 3.5)
project (BasicRT3 VERSION 1.0.0 LANGUAGES CXX )

#=== FINDING PACKAGES ===#

# # Locate TinyXml2 package (library)
# # find_package(TinyXml2 REQUIRED)
# # include_directories(${TinyXML2_INCLUDE_DIRS})

# TinyXML2 is not a module package, therefore cannot be found with find_package() command.
# Set those manually, for now.
set( TinyXML2_INCLUDE_DIRS "/usr/local/include" )
set( TinyXML2_LIB_DIRS "/usr/local/lib" )
set( TinyXML2_LIBRARIES "tinyxml2" )

# Set "manually" paths that need to be considered while compiling/linking
include_directories( cameras
                     integrators
                     core
                     ext
                     ${TinyXML2_INCLUDE_DIRS} )
link_directories( ${TinyXML2_LIB_DIRS} )

#=== SETTING VARIABLES ===#
# Compiling flags
set( GCC_COMPILE_FLAGS "-Wall" )
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}" )
set( RT3_SOURCE_DIR "src" )

#=== main  target ===

file( GLOB SOURCE_BASICRT3 ${RT3_SOURCE_DIR}/cameras/*.cpp
                           ${RT3_SOURCE_DIR}/core/*.cpp
                           ${RT3_SOURCE_DIR}/ext/*.cpp
                           ${RT3_SOURCE_DIR}/integrators/*.cpp
                           ${RT3_SOURCE_DIR}/main/*.cpp
                        )
add_executable(basic_rt3 ${SOURCE_BASICRT3})

target_link_libraries(basic_rt3 PRIVATE ${TinyXML2_LIBRARIES} PRIVATE glog)

#define C++17 as the standard.
set_property(TARGET basic_rt3 PROPERTY CXX_STANDARD 17)
#target_compile_features(run_tests PUBLIC cxx_std_11)
