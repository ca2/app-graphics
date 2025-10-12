#pragma once


#include "apex/parallelization/thread.h"


namespace backimpact
{


   class CLASS_DECL_GCOM thread :
      virtual public ::thread
   {
   public:


      ::pointer < ::mutex >                          m_pmutexBitmap;
      ::pointer<class main>          m_pmain;



      thread();
      ~thread() override;


      virtual void initialize_backimpact_thread(user_interaction * puserinteraction);

      void init_task() override;
      void term_task() override;

      void install_message_routing(::channel * pchannel) override;

      long on_image_stretched(::draw2d::bitmap * pbitmap, long cx, long cy, long iUserData);

      void pre_transition_image(backimpact::main * pimpact);


   };


   class thread_dispatch
   {
   public:


      thread * m_pbackimpactthread;


      thread_dispatch(thread * pbackimpactthread);
      thread_dispatch & operator =(const thread_dispatch & dispatch);


      thread * get_thread();
      void PostBackImpactThreadMessage(wparam wparam, lparam lparam);


   };


} // namespace backimpact



