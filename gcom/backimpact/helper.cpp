#include "framework.h"

#include "helper.h"


namespace backimpact
{


   helper::helper()
   {

   }


   helper::~helper()
   {


   }


   main * helper::helper_get_main()
   {

      return m_pmain;

   }


   void helper::initialize_backimpact_helper(main * pmain)
   {

      //auto estatus = 
      
      initialize(pmain);

      //if (!estatus)
      //{

      //   return estatus;

      //}

      m_pmain = pmain;

      //return estatus;

   }


} // namespace backimpact



