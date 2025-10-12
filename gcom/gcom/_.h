#pragma once


#include "axis/_.h"

#include "_constant.h"


// gcom - gcom Commons Component Acronym


#if defined(_gcom_project)
#define CLASS_DECL_GCOM CLASS_DECL_EXPORT
#else
#define CLASS_DECL_GCOM CLASS_DECL_IMPORT
#endif


#include "_constant.h"


namespace gcom
{


   class impact;


   class pixel_explosion;


   class slide;
   class slide_show;
   class slide_drawer;
   class slide;


   class space;
   class space_util;


   class util;


} // namespace gcom



