#pragma once


//#include "acme/prototype/geometry2d/_geometry2d.h"
#include "acme/prototype/geometry2d/rectangle.h"
#include "acme/platform/flags.h"


namespace backimpact
{


   class CLASS_DECL_GCOM data
   {
   public:


      flags < enum_data >           m_einterfacedataflags;
      enum_image_placement          m_eimageplacement;
      string                        m_strNextImagePath;
      string                        m_strCurrentImagePath;
      ::int_rectangle               m_rectangle;


      data();
      ~data();


      void add(enum_data edata);
      bool has(enum_data edata);

   };


} // namespace backimpact
