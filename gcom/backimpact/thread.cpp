#include "framework.h"
#include "thread.h"
#include "main.h"
#include "user_interaction.h"
#include "acme/constant/user_message.h"
#include "aura/graphics/draw2d/lock.h"


namespace backimpact
{


   thread::thread()
   {

   }


   thread::~thread()
   {


   }


   void thread::initialize_backimpact_thread(user_interaction * pinterface)
   {

      //auto estatus = 
      
      initialize(pinterface);

      //if (!estatus)
      //{

      //   return estatus;

      //}

      //estatus = 
      
      __raw_construct_new(m_pmain);

      //if (!estatus)
      //{

      //   return estatus;

      //}

      m_pmain->set_backimpact_thread(this);

      //estatus = 
      
      m_pmain->initialize_backimpact_main(pinterface);

      //if (!estatus)
      //{

      //   return estatus;

      //}

      //return estatus;

   }


   void thread::init_task()
   {

      //auto estatus = 
      
      ::thread::init_task();

      //if (!estatus)
      //{

      //   return estatus;

      //}

      //m_happeningInitialized.set_happening();

      //return ::success;

   }


   void thread::term_task()
   {

      ::thread::term_task();

   }


   void thread::install_message_routing(::channel * pchannel)
   {

      ::thread::install_message_routing(pchannel);

   }


   void thread::pre_transition_image(backimpact::main * pimpact)
   {

      fork([pimpact]()
         {

            ::draw2d::lock draw2dlock(pimpact);

            pimpact->on_render_buffer();

            pimpact->image_change_post_event(backimpact::e_event_pre_transition_finished);

         });

   }


   long thread::on_image_stretched(::draw2d::bitmap * pbitmap, long cx, long cy, long iUserData)
   {

      __UNREFERENCED_PARAMETER(pbitmap);
      __UNREFERENCED_PARAMETER(cx);
      __UNREFERENCED_PARAMETER(cy);
      __UNREFERENCED_PARAMETER(iUserData);

      return true;

   }


   thread_dispatch::thread_dispatch(thread * pbackimpactthread)
   {

      m_pbackimpactthread = pbackimpactthread;

   }


   thread_dispatch & thread_dispatch::operator =(const thread_dispatch & dispatch)
   {

      m_pbackimpactthread = dispatch.m_pbackimpactthread;

      return *this;

   }


   thread * thread_dispatch::get_thread()
   {

      return m_pbackimpactthread;

   }


   void thread_dispatch::PostBackImpactThreadMessage(wparam wparam,lparam lparam)
   {

      get_thread()->post_message(::user::e_message_backimpact,wparam,lparam);

   }


} // namespace backimpact



