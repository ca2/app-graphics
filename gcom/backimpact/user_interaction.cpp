#include "framework.h"
#include "user_interaction.h"
#include "data.h"
#include "main.h"
#include "thread.h"
#include "gcom.h"
#include "transition_effect.h"
#include "acme/constant/id.h"
#include "acme/constant/user_message.h"
#include "acme/handler/topic.h"
#include "acme/parallelization/synchronous_lock.h"
#include "apex/database/_binary_stream.h"
#include "acme/filesystem/filesystem/directory_context.h"
#include "apex/platform/savings.h"
#include "aura/graphics/draw2d/graphics.h"
#include "aura/graphics/image/drawing.h"
#include "aura/platform/application.h"
#include "aura/message/user.h"
#include "aura/user/user/frame_interaction.h"
#include "aura/platform/system.h"
#include "aura/platform/session.h"


namespace backimpact
{


   user_interaction::user_interaction()
   {

      m_bTransferVoid = false;
      m_dwTimerStep = 2000;
      m_bShowGcomBackground = true;
      m_bEnableShowGcomBackground = true;


      m_flagNonClient.erase(e_non_client_background);

      m_flagNonClient.erase(e_non_client_focus_rect);

      m_bEnableShowGcomBackground = true;

      m_bShowGcomBackground = true;

      m_colorBackground = argb(255, 63, 150, 106);

   }


   user_interaction::~user_interaction()
   {

   }


   void user_interaction::install_message_routing(::channel * pchannel)
   {


      USER_MESSAGE_LINK(::user::e_message_create, pchannel, this, &user_interaction::on_message_create);
      USER_MESSAGE_LINK(::user::e_message_backimpact, pchannel, this, &user_interaction::_001OnBackImpactMessage);
      USER_MESSAGE_LINK(::user::e_message_backimpact_dequeue, pchannel, this, &user_interaction::_001OnDequeueMessage);
      USER_MESSAGE_LINK(::user::e_message_size, pchannel, this, &user_interaction::on_message_size);
      USER_MESSAGE_LINK(::user::e_message_create, pchannel, this, &user_interaction::on_message_create);
      USER_MESSAGE_LINK(::user::e_message_destroy, pchannel, this, &user_interaction::on_message_destroy);

   }


   void user_interaction::on_message_create(::message::message * pmessage)
   {

      if (pmessage->previous())
      {

         return;

      }

      initialize_user_interaction();

      if (m_bEnableShowGcomBackground)
      {

         bool bShowGcomBackground = true;

         //if (!datastream()->get("ShowGcomBackground&data_source=local&", bShowGcomBackground))
         //   bShowGcomBackground = true;

         show_gcom_background(bShowGcomBackground);

      }

   }


   void user_interaction::backimpact_get_client_rect(::int_rectangle & rect)
   {

      rect = this->rectangle();

   }


   ::shift_int user_interaction::backimpact_client_to_screen()
   {

      return client_to_screen();

   }


   void user_interaction::backimpact_update_screen(int_rectangle_array & recta, unsigned int uRedraw)
   {
      
      __UNREFERENCED_PARAMETER(recta);
      
      __UNREFERENCED_PARAMETER(uRedraw);
      
      set_need_redraw();

   }


   void user_interaction::backimpact_update_screen()
   {
      set_need_redraw();
   }

   
   void user_interaction::backimpact_update_screen(const ::int_rectangle & rectangle, unsigned int uRedraw)
   {
      
      __UNREFERENCED_PARAMETER(rectangle);
      
      __UNREFERENCED_PARAMETER(uRedraw);

      set_need_redraw();

   }


   void user_interaction::backimpact_post_message(::user::enum_message eusermessage, ::wparam wparam, ::lparam lparam)
   {

      post_message(eusermessage, wparam, lparam);

   }


   void user_interaction::backimpact_get_data(data & data)
   {

      if(data.has(e_data_next_image_path))
      {

         data.m_strNextImagePath = backimpact_get_next_background_image_path(data.m_strCurrentImagePath);

      }

//#ifdef WINDOWS_DESKTOP
//
//      if(data.has(e_data_enhanced_metafile))
//      {
//
//         data.m_henhmetafile = nullptr;
//
//      }
//
//#endif

   }


   void user_interaction::backimpact_set_data(data & data)
   {

      if(data.has(e_data_current_image_path))
      {

         backimpact_set_current_background_image_path(data.m_strCurrentImagePath);

      }

   }


   string user_interaction::backimpact_get_next_background_image_path(const ::scoped_string & scopedstrCurrentImagePath)
   {

//         string strCurrentImagePath;

      //       datastream()->get("::backimpact::CurrentBackgroundImagePath", strCurrentImagePath);

      //if(m_spfilesetBackgroundImage.is_set())
      //{

        // synchronous_lock synchronouslock(m_spfilesetBackgroundImage->synchronization(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);

         //return m_spfilesetBackgroundImage->get_next_file(strCurrentImagePath);

      //}

      return m_listing.get_next(scopedstrCurrentImagePath);


      //return "";

   }


   void user_interaction::backimpact_set_current_background_image_path(const ::scoped_string & scopedstr)
   {

      //datastream()->set("::backimpact::CurrentBackgroundImagePath", psz);

   }


   class ::time user_interaction::backimpact_get_background_update_time()
   {

      return m_timeBackgroundUpdate;

   }


   ::color::color user_interaction::get_background_color()
   {
      
      return m_colorBackground;

   }


   void user_interaction::initialize_user_interaction()
   {

      auto pframe = get_typed_parent < ::user::frame_interaction > ();

      if(pframe)
      {

         ::color::color color;
            
         color = pframe->get_border_main_body_color();

         ::color::hls hls;

         hls.m_dH = 1.0;
         hls.m_dL = 0.5;
         hls.m_dS = 0.5;
         
         color.hls_mult(hls);

         m_colorBackground = color.opaque();

      }

   }



   void user_interaction::_001OnDraw(::draw2d::graphics_pointer & pgraphics)
   {

      auto rectangleX = this->rectangle();

      try
      {

         

         if(m_bShowGcomBackground && m_bEnableShowGcomBackground && !session()->savings()->should_save(::e_resource_display_bandwidth))
         {

            if(::backimpact::user_interaction::is_enabled())
            {

               ::backimpact::main * pbackimpactmain = ::backimpact::user_interaction::get_main();

               //::backimpact::gcom * pgcom = pbackimpactmain->get_gcom();

               pbackimpactmain->defer_check_layout();

               if(pbackimpactmain->is_initialized())
               {

                  ::draw2d::save_context k(pgraphics);

                  //::draw2d::region_pointer rgn(e_create);
//                     ::int_rectangle rectangle(pgcom->m_rectangleFinalPlacement);
//                     client_to_screen(rectangle);
//                     rectangle.offset(pgraphics->get_origin());
//                                 //rgn->create_rect(rectangle);
//                     pgraphics->IntersectClipRect(rectangle);

                  ::backimpact::user_interaction::backimpact_render(pgraphics,rectangleX);
                  ///pgraphics->SelectClipRgn(nullptr);

               }
            }
            else
            {
               //pgraphics->fill_rectangle(rectangleX, rgb(200, 220, 180));
            }
         }

      }
      catch(...)
      {

      }


   }


   bool user_interaction::toggle_show_gcom_background()
   {

      return show_gcom_background(!m_bShowGcomBackground);

   }


   bool user_interaction::show_gcom_background(bool bShow)
   {

      single_lock synchronouslock(this->synchronization());

      if (!m_bEnableShowGcomBackground)
      {

         return false;

      }

      if (bShow)
      {

//         synchronouslock.lock();

         //if(m_spfilesetBackgroundImage.is_set())
         //{

//            m_spfilesetBackgroundImage.release();

  //       }

  //       synchronouslock.unlock();

         //auto pimagefileset = øcreate_new < simpledb::file_set > ();

         //auto papp = m_papplication;

         //auto pdataserver = papp->dataserver();

         //pimagefileset->initialize_data_client(pdataserver);

         //pimagefileset->set_data_key_modifier({"ImageDirectorySet", true });

         string_array_base straFilter;

         straFilter.add("*.bmp");

         straFilter.add("*.jpg");

         straFilter.add("*.png");

         m_listing.set_pattern_file_listing("image://weather", straFilter, e_depth_recursively);


         directory()->enumerate(m_listing);

         //pimagefileset->add_filter(straFilter);

         //bool bInit;

         //if (!pimagefileset->datastream()->get("initialize_default", bInit))
         //{

         //   bInit = true;

         //}

         //if (bInit)
         //{

         //   pimagefileset->datastream()->set("initialize_default", false);

         //}

         ////pimagefileset->initialize_os_wallpaper(bInit);
         //pimagefileset->initialize_os_wallpaper(true);

         //synchronouslock.lock();

         //m_spfilesetBackgroundImage = pimagefileset;

         //synchronouslock.unlock();

         set_background_change_period(1_s);

         enable(true);

         set_need_layout();

         get_main()->m_bPendingLayout = true;

      }
      else
      {

         enable(false);

      }

      m_bShowGcomBackground = bShow;

      ::cast < ::database::client > pdatabaseclient = application();

      pdatabaseclient->datastream()->set({"ShowGcomBackground", true }, bShow);

      return true;

   }


void user_interaction::initialize(::particle * pparticle)
{

   ::user::interaction::initialize(pparticle);

   __raw_construct_new(m_pthread);

   m_pthread->initialize_backimpact_thread(this);

   m_pthread->branch_synchronously();

}



//  void user_interaction::Shutdown()
//  {

//     try
//     {

//        if(m_pthread.is_set())
//        {

///*           if(m_pthread->m_pmain != nullptr)
//           {

//              m_pthread->m_pmain->m_pinterface = nullptr;

//           }*/

//           m_pthread->destroy();

//           m_pthread = nullptr;

//        }

//     }
//     catch(...)
//     {

//     }

//  }




bool user_interaction::is_full_screen()
{

   return false;

}


//::time d_tickLastBackImpactUpdate;

void user_interaction::_001OnBackImpactMessage(::message::message * pmessage)
{

   ::pointer<::user::message>pusermessage(pmessage);

   main * pmain = get_main();

   if (pmain == nullptr)
   {

      return;

   }

   switch(pusermessage->m_wparam)
   {
   case BackImpactWparamImageChangeEvent:
      pmain->pulse_happening((enum_happening)pusermessage->m_lparam.m_lparam);
      break;
   }
   pusermessage->m_lresult = 0;

}

void user_interaction::_001OnDequeueMessage(::message::message * pmessage)
{
   ::pointer<::user::message>pusermessage(pmessage);
   main * pmain = get_main();

   if (pmain == nullptr)
   {

      return;

   }

   pmain->_001OnDequeueMessage(pusermessage->m_wparam,pusermessage->m_lparam);
   pusermessage->m_lresult = 0;
}

void user_interaction::on_message_size(::message::message * pmessage)
{

   ::pointer<::user::message>pusermessage(pmessage);

   main * pmain = get_main();

   if (pmain == nullptr)
   {

      return;

   }

   pmain->m_bPendingLayout = true;

   pusermessage->m_bRet = false;

}


void user_interaction::initialize_timer()
{

   if (m_ptaskTimer2.is_null())
   {

      m_ptaskTimer2 = fork([&]()
      {

         try
         {

            while (::task_get_run())
            {

               image_change_post_event(::backimpact::e_event_timer);

               preempt(1_s);

            }

         }
         catch (...)
         {

         }

         m_ptaskTimer2.release();

      });

   }

}


/* void user_interaction::on_message_create(::message::message * pmessage)
{

   if (pmessage->previous())
   {

      return;

   }

      

}*/


void user_interaction::on_message_destroy(::message::message * pmessage)
{

   if (pmessage->previous())
   {

      return;

   }

}


void user_interaction::backimpact_post_message(wparam wparam,lparam lparam)
{

   if(!is_window())
      return;

   if(!task_get_run())
      return;

   backimpact_post_message(::user::e_message_backimpact,wparam,lparam);

}


bool user_interaction::update_buffer(const ::int_rectangle & rectangle)
{

   main * pmain = get_main();

   if (pmain == nullptr)
   {

      return false;

   }

   pmain->update_buffer(rectangle);

   return true;

}


//#ifdef WINDOWS_DESKTOP
//
//
//HENHMETAFILE user_interaction::backimpact_get_filling_HENHMETAFILE()
//{
//   data data;
//   data.add(e_data_enhanced_metafile);
//   backimpact_get_data(data);
//
//   return data.m_henhmetafile;
//
//}
//
//
//#endif


bool user_interaction::backimpact_get_image_placement(const ::scoped_string & scopedstr,enum_image_placement & eplacement)
{

   __UNREFERENCED_PARAMETER(scopedstr);

   __UNREFERENCED_PARAMETER(eplacement);

   return false;

}


void user_interaction::update_drawing_objects()
{

   main * pmain = get_main();

   if (pmain == nullptr)
   {

      return;

   }

   pmain->update_drawing_objects();

}


bool user_interaction::start_loading_next_image()
{

   main * pmain = get_main();

   if (pmain == nullptr)
   {

      return false;

   }

   return pmain->start_loading_next_image();

}


void user_interaction::on_image_loaded(::image::image *pimage)
{

   main * pmain = get_main();

   if (pmain == nullptr)
   {

      return;

   }

   pmain->on_image_loaded(pimage);

}


void user_interaction::on_after_image_loaded()
{

   main * pmain = get_main();

   if (pmain == nullptr)
   {

      return;

   }

   pmain->on_after_image_loaded();

}


bool user_interaction::on_render_buffer()
{

   main * pmain = get_main();

   if (pmain == nullptr)
   {

      return false;

   }

   return pmain->on_render_buffer();

}


void user_interaction::on_destroy()
{

   main * pmain = get_main();

   if (pmain == nullptr)
   {

      throw ::exception(error_failed);

   }

   //auto estatus = 
   
   pmain->on_destroy();

   //return estatus;

}


void user_interaction::set_background_change_period(const class time & time)
{

   main * pmain = get_main();

   if (pmain == nullptr)
   {

      return;

   }

   pmain->set_background_change_period(time);

}


void user_interaction::image_change_post_event(enum_happening ehappening)
{

   main * pmain = get_main();

   if (pmain == nullptr)
   {

      return;

   }

   pmain->image_change_post_event(ehappening);

}


::draw2d::graphics_pointer user_interaction::GetTransferDC()
{

   main * pmain = get_main();

   if (pmain == nullptr)
   {

      return nullptr;

   }

   return pmain->GetTransferDC();

}


::particle * user_interaction::get_transfer_mutex()
{

   main * pmain = get_main();

   if (pmain == nullptr)
   {

      return nullptr;

   }

   return pmain->get_transfer_mutex();

}


enum_image_placement user_interaction::get_current_image_placement()
{

   main * pmain = get_main();

   if (pmain == nullptr)
   {

      return ImagePlacementZoomAll;

   }

   return pmain->get_current_image_placement();

}


void user_interaction::set_current_image_placement(enum_image_placement eplacement)
{

   main * pmain = get_main();

   if (pmain == nullptr)
   {

      return;

   }

   pmain->set_current_image_placement(eplacement);

}


void user_interaction::get_current_image_path(string &str)
{

   main * pmain = get_main();

   if (pmain == nullptr)
   {

      return;

   }

   pmain->get_current_image_path(str);

}


void user_interaction::enable(bool bEnable)
{

   main * pmain = get_main();

   if (pmain == nullptr)
   {

      return;

   }

   pmain->enable(bEnable);

   if (bEnable)
   {

      m_dwTimerStep = 1;

      initialize_timer();

   }
   else
   {

      m_dwTimerStep = 0;

   }

}


bool user_interaction::is_enabled()
{

   main * pmain = get_main();

   if (pmain == nullptr)
   {

      return false;

   }

   return pmain->is_enabled();

}


void user_interaction::backimpact_render(::draw2d::graphics_pointer & pgraphics,const ::int_rectangle & rectangleParam)
{

   if (!m_bEnableShowGcomBackground)
      return;

   ::int_rectangle rectangle(rectangleParam);

   backimpact_render(pgraphics,rectangle.left,rectangle.top,rectangle.width(),rectangle.height());

}


void user_interaction::backimpact_render(::draw2d::graphics_pointer & pgraphics,  int x,int y, int w, int h)
{

   if (!m_bEnableShowGcomBackground)
      return;

   main  * pmain = get_main();

   if (pmain == nullptr)
   {

      return;

   }

   gcom * pgcom = pmain->get_gcom();

   pmain->defer_check_layout();

   if (!pmain->is_initialized())
   {

      return;

   }

   single_lock synchronouslock(pgcom->m_pmutex4Transfer,true);

   ::draw2d::graphics_pointer dcTransfer = pgcom->GetTransferDC();

   if (dcTransfer.is_null())
   {

      return;

   }

   if (dcTransfer->get_os_data() == nullptr)
   {

      return;

   }

   ::image::image_source imagesource(dcTransfer, double_rectangle_dimension(x,y, w,h));

   auto rectangle = double_rectangle_dimension(x,y, w,h);

   ::image::image_drawing_options imagedrawingoptions(rectangle);

   ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

   pgraphics->draw(imagedrawing);

}


//::color::color user_interaction::get_background_color()
//{

//   //if(pmain->layout().is_full_screen())
//   //   return rgb(63, 106, 150)
//   return rgb(63,150,106);

//}


// if the window draws something over the gcom::_backimpact
// background, this function call is a feedback requested by from some
// transition effect or draw2d effect in order to display to the
// the user an interactive effect. At first design, the only needed
// feedback is the final output window screenshot (without
// own gcom::_backimpact background?)
void user_interaction::backimpact_feedback(::image::image *pimage)
{

   __UNREFERENCED_PARAMETER(pimage);

}


bool user_interaction::backimpact_get_destroy()
{

   return false;

}


//void user_interaction::backimpact_get_client_rect(::int_rectangle * lprect)
//{

//   __UNREFERENCED_PARAMETER(lprect);

//}


//void user_interaction::backimpact_client_to_screen(::int_rectangle * lprect)
//{

//   __UNREFERENCED_PARAMETER(lprect);

//}


//void user_interaction::backimpact_update_screen(int_rectangle_array & recta,unsigned int uRedraw)
//{

//   __UNREFERENCED_PARAMETER(recta);
//   __UNREFERENCED_PARAMETER(uiRedraw);

//}


//void user_interaction::backimpact_update_screen()
//{

//}


//void user_interaction::backimpact_update_screen(const ::int_rectangle & rectangle,unsigned int uRedraw)
//{

//   __UNREFERENCED_PARAMETER(rectangle);
//   __UNREFERENCED_PARAMETER(uiRedraw);

//}


//void user_interaction::backimpact_post_message(::user::enum_message eusermessage, ::wparam wparam, ::lparam lparam)
//{

//   __UNREFERENCED_PARAMETER(emessage);
//   __UNREFERENCED_PARAMETER(wparam);
//   __UNREFERENCED_PARAMETER(lparam);

//   //throw ::exception(::exception("implement in derived class"));

//}


//void user_interaction::backimpact_get_data(data & data)
//{

//   __UNREFERENCED_PARAMETER(data);

//}


//void user_interaction::backimpact_set_data(data & data)
//{

//   __UNREFERENCED_PARAMETER(data);

//}


//string user_interaction::backimpact_get_next_background_image_path(const ::scoped_string & scopedstrCurrentImagePath)
//{

//   return "";

//}


//void user_interaction::backimpact_set_current_background_image_path(const ::scoped_string & scopedstr)
//{

//   __UNREFERENCED_PARAMETER(scopedstr);

//}


::user::interaction * user_interaction::backimpact_get_user_interaction()
{

   return this;

}


main * user_interaction::get_main()
{

   if (m_pthread.is_null())
   {

      return nullptr;

   }

   return m_pthread->m_pmain;

}


bool user_interaction::_0723TransferVoid()
{

   return m_bTransferVoid;

}


   void user_interaction::handle(::topic * ptopic, ::handler_context * phandlercontext)
   {

      if (ptopic->id() == id_pre_close_document)
      {

         ::backimpact::main * pbackimpactmain = ::backimpact::user_interaction::get_main();

         ::backimpact::transition_effect * peffect = pbackimpactmain->get_transition_effect();

         peffect->m_bActive = false;

         peffect->m_bDestroy = true;

      }

   }

      
   void user_interaction::on_layout(::draw2d::graphics_pointer & pgraphics)
   {

      main * pmain = get_main();

      if (pmain)
      {

         pmain->m_bPendingLayout = true;

      }

      //::user::box::on_layout(pgraphics);

   }


} //namespace backimpact




