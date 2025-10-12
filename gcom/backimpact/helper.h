#pragma once


#include "main.h"


namespace backimpact
{


   class CLASS_DECL_GCOM helper :
      virtual public ::object
   {
   public:


      ::pointer<main>   m_pmain;


      helper();
      virtual ~helper();


      virtual void initialize_backimpact_helper(main * pmain);


      main * helper_get_main();


      inline ::particle * gcom_pmutex() { return m_pmain->gcom_pmutex();}


   };


} // namespace backimpact







