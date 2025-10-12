#include "framework.h"
#include "user_interaction.h"
#include "main.h"
#include "data.h"
#include "thread.h"
#include "image_change.h"
#include "gcom.h"
#include "transition_effect.h"
#include "visual_effect.h"
#include "acme/constant/user_message.h"
#include "acme/parallelization/mutex.h"
//#include "acme/prototype/prototype/pointer.h"
#include "aura/graphics/image/image.h"


namespace backimpact
{


   main::main()
   {

      m_bEnabled = false;
      m_estate = StateTiming;
      m_bInitialized = false;


      m_bPendingLayout = true;

   }


   main::~main()
   {


   }


   void main::initialize_backimpact_main(user_interaction * pbackimpactinterface)
   {

      //auto estatus = 
      
      initialize(pbackimpactinterface);

      //if (!estatus)
      //{

      //   return estatus;

      //}

      //estatus = 
      
      m_puserinteraction = pbackimpactinterface;

      //if (!estatus)
      //{

      //   return estatus;

      //}

      //estatus = 
      
      __raw_construct_new(m_pimagechange);

      //if (!estatus)
      //{

      //   return estatus;

      //}

      //estatus = 
      
      m_pimagechange->initialize_backimpact_helper(this);

      //if (!estatus)
      //{

      //   return estatus;

      //}

      //estatus = 
      
      __raw_construct_new(m_pgcom);

      //if (!estatus)
      //{

      //   return estatus;

      //}

      //estatus = 
      
      m_pgcom->initialize_backimpact_helper(this);

      //if (!estatus)
      //{

      //   return estatus;

      //}

      //estatus = 
      
      __raw_construct_new(m_ptransitioneffect);

      //if (!estatus)
      //{

      //   return estatus;

      //}

      //estatus =
      
      m_ptransitioneffect->initialize_backimpact_helper(this);

      //if (!estatus)
      //{

      //   return estatus;

      //}

      //estatus = 
      
      __raw_construct_new(m_pvisualeffect);

      //if (!estatus)
      //{

      //   return estatus;

      //}

      //estatus = 
      
      m_pvisualeffect->initialize_backimpact_helper(this);

      //if (!estatus)
      //{

      //   return estatus;

      //}

      //return estatus;

   }


   void main::set_backimpact_thread(thread* pthread)
   {

      //auto estatus = 
      
      m_pthread = pthread;

      //if (!estatus)
      //{

      //   return estatus;

      //}

      //return estatus;

   }





   bool main::is_full_screen()
   {

      return false;

   }


   void main::update_drawing_objects()
   {

      gcom * pgcom = get_gcom();

      pgcom->update_objects();
      pgcom->ApplyImagePlacementSettings();
      pgcom->buffer_to_back();
      pgcom->back_to_transfer();

      m_bInitialized = true;

   }


   bool main::update_buffer(const ::int_rectangle & rectangle)
   {

      __UNREFERENCED_PARAMETER(rectangle);

      return true;

   }


   bool main::start_loading_next_image()
   {

      return get_image_change()->start_loading_next_image();

   }


   void main::_001OnImageLoaded(::message::message * pmessage)
   {

      on_image_loaded(m_pimagechange->m_pimage);

   }


   void main::on_image_loaded(::image::image *pimage)
   {

      if(pimage->is_ok())
      {

         image_change_post_event(e_event_load_failed);

         return;

      }

      data data;

      data.add(e_data_current_image_path);

      get_current_image_path(data.m_strCurrentImagePath);

      get_user_interaction()->backimpact_set_data(data);

      gcom * pgcom = get_gcom();

      try
      {

         pgcom->on_image_loaded(pimage);

      }
      catch(...)
      {

      }

   }


   void main::on_after_image_loaded()
   {

      image_change * pimagechange = get_image_change();

      gcom * pgcom = get_gcom();

      enum_image_placement eplacement;

      informationf("CXfplayerImpact::on_after_image_loaded");

      if(get_user_interaction()->backimpact_get_image_placement(pimagechange->m_strCurrentImagePath, eplacement))
      {

         pimagechange->m_eplacement = (enum_image_placement) eplacement;

      }
      else
      {

         pimagechange->m_eplacement = pgcom->GetDefaultPlacement();

      }

   }

   bool main::on_render_buffer()
   {
 
      if (!m_bInitialized)
      {

         return false;

      }

      gcom * pgcom = get_gcom();

      pgcom->ApplyImagePlacementSettings();

      return true;

   }

   
   void main::on_destroy()
   {

      //auto estatus = 
      
      get_gcom()->on_destroy();

      //return estatus;

   }


   void main::set_state(enum_state estate)
   {
      m_estate = estate;
      switch(estate)
      {
      case StateFinish:
      {
         get_image_change()->m_happeningImageChangeFinish.set_happening();
      }
      break;
      default:
         break;
      }
      
      //return 1;

   }






#define ONRESIZEDELAY 5000


   void main::pulse_happening(enum_happening ehappening)
   {

      if (!is_enabled())
      {

         //return 1;

         return;

      }

      if (!m_bInitialized)
      {

         //return 1;

         return;

      }

      //thread * pthreadIdle = get_thread();

      //auto ::time = ::time::now();

      if(ehappening == e_event_timer)
      {


         auto ptransitioneffect = get_transition_effect();

         if (ptransitioneffect)
         {

            ptransitioneffect->one_second_timer();

         }

      }

      switch(m_estate)
      {
      case StateTiming:
         switch(ehappening)
         {
         case e_event_resize:
         {
            on_resize();
         }
         break;
         case e_event_flag_changed:
         {
            get_user_interaction()->backimpact_post_message(::user::e_message_backimpact_dequeue, 18, 3);
         }
         break;
         case e_event_timer:
         {

            if(m_bFirstLoadImage || get_image_change()->m_timeBackgroundLastChange.elapsed() >= get_image_change()->get_background_change_period())
            {

               if(start_loading_next_image())
               {

                  m_bFirstLoadImage = false;

                  set_state(e_state_loading);

               }

            }

         }
         break;
         case e_event_load_now:
            get_image_change()->OnHappeningLoadNow();
            break;
         default:;
         }
         break;
      case e_state_loading:
         switch(ehappening)
         {
         case e_event_resize:
         {
            on_resize();
         }
         break;
         case e_event_flag_changed:
         {
            get_user_interaction()->backimpact_post_message(::user::e_message_backimpact_dequeue, 18, 3);
         }
         break;
         case e_event_loaded:
         {

            if(get_user_interaction()->backimpact_get_destroy())
            {

               set_state(StateFinish);

               break;

            }
            
            on_after_image_loaded();
            
            set_state(StatePreTransition);
            
            get_image_change()->m_timeBackgroundPreTransitionStart.Now();
            
            get_backimpact_thread()->pre_transition_image(this);

         }
         break;
         case e_event_load_now:
         case e_event_load_failed:
         {
            
            informationf("image_change::ChangeStateMachine case 120");
            
            set_state(StateTiming);
            
            get_image_change()->m_timeBackgroundLastChange.Now();

         }
         break;
         case e_event_timer:
         {
            get_image_change()->m_dwLoadCounter++;
            //  informationf("image_change::ChangeStateMachine case 130");
            if(get_image_change()->m_dwLoadCounter > 5) // Espera 10 segundos para carregar imagem
            {
               get_image_change()->m_dwLoadCounter = 0;
               get_image_change()->LoadImageAsync();
            }
            break;
         }
         default:
            break;
         }
         break;
      case StatePreTransition:
         switch(ehappening)
         {
         case e_event_load_now:
            get_image_change()->OnHappeningLoadNow();
            break;
         case e_event_resize:
         {
            set_state(StatePreTransitionResize);
         }
         break;
         case e_event_timer:
            break;
         case e_event_pre_transition_finished:
         {

            auto ptransitioneffect = get_transition_effect();

            if (!ptransitioneffect)
            {

               //return 1;

               return;

            }

            //auto estatus = 
            
            ptransitioneffect->initialize_transition_effect();

            //if (!estatus)
            //{

            //   return estatus;

            //}

            informationf("image_change::ChangeStateMachine case 110");

            set_state(StateInTransitionEffect);
            //return estatus;

            return;
         }

         
            break;
         default:;
         }
         break;
      case StatePreTransitionResize:
         switch(ehappening)
         {
         case e_event_load_now:
         {
            get_image_change()->OnHappeningLoadNow();
         }
         break;
         case e_event_resize:
         {
         }
         break;
         case e_event_timer:
            break;
         case e_event_pre_transition_finished:
         {
            
            set_state(StatePreTransition);

            get_image_change()->m_timeBackgroundPreTransitionStart.Now();

            get_backimpact_thread()->pre_transition_image(this);

         }
         break;
         default:;
         }
         break;
      case StateInTransitionEffect:
      {
         switch(ehappening)
         {
         case e_event_resize:
         {
            
            set_state(StatePreTransition);

            get_image_change()->m_timeBackgroundPreTransitionStart.Now();

            get_backimpact_thread()->pre_transition_image(this);

         }
         break;
         case e_event_flag_changed:
         {

            auto ptransitioneffect = get_transition_effect();

            if (!ptransitioneffect)
            {

               //return 1;

               return;

            }
            
            ptransitioneffect->Restart();

         }
         break;
         case e_event_timer:
         {

            auto ptransitioneffect = get_transition_effect();

            if (!ptransitioneffect)
            {

               //return 1;

               return;

            }

            ptransitioneffect->OnTimer();

         }
         break;
         case e_event_transition_effect_finished:
         {

            get_image_change()->m_timeBackgroundLastChange.Now();

            set_state(StateTiming);

         }
            break;
         case e_event_load_now:;

            get_image_change()->OnHappeningLoadNow();

            break;
         default:;
         }
         break;
         default:;
         }
      }
      
      //return 1;

   }


   void main::on_resize()
   {

      get_user_interaction()->backimpact_post_message(::user::e_message_backimpact_dequeue, 18, 4);

   }


   void main::_001OnDequeueMessage(wparam wparam, lparam lparam)
   {

      bool bRunStep =
      wparam == 18
      && lparam == 0;

      /*while(PeekMessage(
      &msg,
      INVALID_HANDLE_VALUE,
      ::user::e_message_backimpact_deque,
      ::user::e_message_backimpact_deque,
      PM_REMOVE))
      {
      if(
      msg.wParam == 18
      && msg.lParam == 0)
      {
      bRunStep = true;
      }
      }*/

      if(bRunStep)
      {
         ASSERT(false);
         //RunTransitionEffectStep();
      }
      else if(wparam == 18)
      {
         if(lparam == 3)
         {
            ::int_rectangle rectangle;
            update_buffer(rectangle);

            get_user_interaction()->backimpact_update_screen(rectangle,0);
         }
         else if(lparam == 4)
         {

            ::int_rectangle rectangle;

            update_buffer(rectangle);

            //get_user_interaction()->backimpact_update_screen(rectangle, RDW_INVALIDATE);
            get_user_interaction()->backimpact_update_screen(rectangle, 0);

         }

      }

   }


   void main::image_change_post_event(enum_happening ehappening)
   {

      try
      {

         get_user_interaction()->backimpact_post_message(user_interaction::BackImpactWparamImageChangeEvent, (lparam)(::iptr) ehappening);

      }
      catch(...)
      {


      }
   }


   void main::set_background_change_period(const class time & time)
   {

      get_image_change()->set_background_change_period(time);

   }


   ::draw2d::graphics_pointer main::GetTransferDC()
   {
      return get_gcom()->GetTransferDC();
   }


   ::pointer< ::particle > main::get_transfer_mutex()
   {

      return get_gcom()->m_pmutex4Transfer;

   }


   bool main::get_next_image_path(string &str)
   {

      data data;

      data.m_strCurrentImagePath = str;

      data.add(e_data_next_image_path);

      get_user_interaction()->backimpact_get_data(data);

      str = data.m_strNextImagePath;

      return true;

   }


   enum_image_placement main::get_current_image_placement()
   {
      return get_image_change()->m_eplacement;
   }

   void main::set_current_image_placement(enum_image_placement eplacement)
   {
      get_image_change()->m_eplacement = eplacement;
   }

   void main::get_current_image_path(string &str)
   {
      str = get_image_change()->m_strCurrentImagePath;
   }
   bool main::is_enabled()
   {
      return m_bEnabled;
   }

   void main::enable(bool bEnable)
   {
      m_bEnabled = bEnable;
   }

   bool main::is_initialized()
   {
      return m_bInitialized;
   }

   void main::defer_check_layout()
   {

      if(m_bPendingLayout)
      {

         m_bPendingLayout = false;

         update_drawing_objects();

         pulse_happening((enum_happening) e_event_resize);

      }

   }


   gcom * main::get_gcom()
   {

      return m_pgcom;

   }


   thread * main::get_backimpact_thread()
   {

      return m_pthread;

   }


   image_change * main::get_image_change()
   {

      return m_pimagechange;

   }


   transition_effect * main::get_transition_effect()
   {

      return m_ptransitioneffect;

   }


   visual_effect * main::get_visual_effect()
   {

      return m_pvisualeffect;

   }

   
   user_interaction * main::get_user_interaction()
   {

      return m_puserinteraction;

   }


} // namespace backimpact



