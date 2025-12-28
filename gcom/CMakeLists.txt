cmake_minimum_required(VERSION 3.16)
project(gcom CXX)


set(LIBRARY_NAME ${PROJECT_NAME})


list(APPEND library_source
        _.cpp
        backimpact/_.h
        backimpact/aaa_interface.h
        backimpact/data.cpp
        backimpact/data.h
        #backimpact/enum.h
        backimpact/gcom.cpp
        backimpact/gcom.h
        backimpact/helper.cpp
        backimpact/helper.h
        backimpact/image_change.cpp
        backimpact/image_change.h
        backimpact/main.cpp
        backimpact/main.h
        backimpact/thread.cpp
        backimpact/thread.h
        backimpact/transition_effect.cpp
        backimpact/transition_effect.h
        backimpact/transition_effect_tool001.cpp
        backimpact/user_interaction.cpp
        backimpact/user_interaction.h
        backimpact/visual_effect.cpp
        backimpact/visual_effect.h
        gcom/_.h
        #   gcom/_const.h
        #gcom/#_library.h
        gcom/double3d_point.h
        gcom/ipp.cpp
        gcom/pixel_explosion.cpp
        gcom/pixel_explosion.h
        gcom/slide.cpp
        gcom/slide.h
        gcom/slide_drawer.h
        gcom/space.cpp
        gcom/space.h
        gcom/space_util.cpp
        gcom/space_util.h
        gcom/util.cpp
        gcom/util.h
        gcom/impact.cpp
        gcom/impact.h
        _.h
        ##_library.h
        image_show.cpp
        image_show.h
        framework.cpp
        framework.h
        )
        
        
list(APPEND library_references
   acme
   apex
   aqua
   aura
   axis
   berg
)


add_library(${PROJECT_NAME} SHARED ${library_source})
add_library(static_${PROJECT_NAME} STATIC ${library_source})

if (NOT ${NO_PRECOMPILED_HEADER})

    target_precompile_headers(${PROJECT_NAME} PRIVATE framework.h)
    target_precompile_headers(static_${PROJECT_NAME} PRIVATE framework.h)

endif ()

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

if(${WIN32})
    target_compile_options(${PROJECT_NAME} PRIVATE "/MD$<$<CONFIG:Debug>:d>")
    target_link_options(${PROJECT_NAME} PRIVATE "/DEFAULTLIB:msvcrt$<$<CONFIG:Debug>:d>")
    target_link_options(${PROJECT_NAME} PRIVATE "/SUBSYSTEM:WINDOWS")
    target_link_libraries(${PROJECT_NAME} PRIVATE acme apex aqua aura axis berg)
elseif(${LINUX} OR ${__BSD__} OR ${ANDROID})

   target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
   #set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)



    target_compile_options(${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-fpermissive  ${EXTRA_CXX_TARGET_COMPILER_OPTIONS}>)
    #target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-rpath=\$ORIGIN")
    target_link_libraries(${PROJECT_NAME} PRIVATE ${library_references} ${global_library_references})
    target_link_options(${PROJECT_NAME} PRIVATE -Wl,-z,defs)
endif ()

list(APPEND library_include_directories ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND library_include_directories ${CMAKE_CURRENT_SOURCE_DIR}/..)
list(APPEND library_include_directories ${CMAKE_CURRENT_SOURCE_DIR}/../..)

target_include_directories(${PROJECT_NAME} PRIVATE ${library_include_directories})
target_include_directories(static_${PROJECT_NAME} PRIVATE ${library_include_directories})

target_compile_definitions(${PROJECT_NAME} PRIVATE _${PROJECT_NAME}_project)






