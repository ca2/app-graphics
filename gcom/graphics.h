#pragma once


#include "app-core/hellomultiverse/hellomultiverse.h"


#if defined(_app_core_miliverse_project)
    #define CLASS_DECL_APP_CORE_MILIVERSE  CLASS_DECL_EXPORT
#else
    #define CLASS_DECL_APP_CORE_MILIVERSE  CLASS_DECL_IMPORT
#endif


namespace miliverse
{

   class application;

} // namespace flag

#undef APP_TYPE
#define APP_TYPE ::miliverse::application

#include "miliverse_application.h"
