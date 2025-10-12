#include "framework.h"

#include "data.h"


namespace backimpact
{

   
   data::data()
   {

   }


   data::~data()
   {

   }


   void data::add(enum_data edata)
   {

      m_einterfacedataflags.add(edata);

   }


   bool data::has(enum_data edata)
   {

      return m_einterfacedataflags.has(edata);

   }


} // namespace backimpact
