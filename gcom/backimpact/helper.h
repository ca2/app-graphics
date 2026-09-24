// Added ::draw2d::domain_consumer inheritance to backimpact::helper by
//   camilo on 2026-09-24 05:16 <3ThomasBorregaardSørensen!! Mummi!! bilbo!!
#pragma once


#include "main.h"
#include "aura/graphics/draw2d/domain_consumer.h"


namespace backimpact
{


   class CLASS_DECL_GCOM helper :
      virtual public ::object,
      virtual public ::draw2d::domain_consumer
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







