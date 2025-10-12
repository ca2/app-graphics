#include "framework.h"
#include "image_change.h"
#include "transition_effect.h"
#include "user_interaction.h"
#include "gcom.h"
#include "apex/database/_binary_stream.h"
#include "aura/graphics/image/context.h"
#include "aura/platform/context.h"


namespace backimpact
{


   image_change::image_change()
   {

      defer_create_synchronization();

      m_bLastLoadImageSynch = false;
      
      auto start = ::time::now();

      m_timeLoadStart = start;

      m_timeBackgroundChangePeriod = 1_s;

      m_bLoadImage = false;

      m_bInitialPath = false;

   }


   image_change::~image_change()
   {


   }


   void image_change::initialize_backimpact_helper(main * pmain)
   {

      helper::initialize_backimpact_helper(pmain);

   }


   void image_change::OnHappeningLoadNow()
   {

      main * pmain = helper_get_main();

      if (!pmain)
      {

         return;

      }

      auto ptransitioneffect = pmain->get_transition_effect();

      if (!ptransitioneffect)
      {

         return;

      }
         
      ptransitioneffect->Reset();

      if(start_loading_next_image())
      {

         pmain->set_state(e_state_loading);

      }

   }



   bool image_change::LoadImageSync()
   {
      
      auto pmain = helper_get_main();

      if (!pmain)
      {

         return false;

      }

      string str;

      pmain->get_next_image_path(str);

      return LoadImageSync(str);

   }


   bool image_change::LoadImageAsync()
   {

      auto pmain = helper_get_main();

      if (!pmain)
      {

         return false;

      }

      string str;
      
      pmain->get_next_image_path(str);

      if(str.is_empty())
      {

         return false;

      }

      return LoadImageAsync(str);

   }

   
   bool image_change::LoadImageAsync(const ::scoped_string & scopedstrImagePath)
   {

      m_strCurrentImagePath = scopedstrImagePath;

      m_dwLoadCounter = 0;

      fork([this, scopedstrImagePath]()
      {

         LoadImageSync(scopedstrImagePath);

      });

      return true;

   }


   bool image_change::start_loading_next_image()
   {

      if (m_bLoadImage)
      {

         return true;

      }

      m_bLoadImage = true;

      fork([&]()
      {

         if (!m_timeLoadStart.timeout(1_s))
         {

            m_bLoadImage = false;

            return;

         }

         if (m_strCurrentImagePath.is_empty() && !m_bInitialPath)
         {

            //auto pmain = helper_get_main();

            ::database::client * pclient = dynamic_cast <::database::client *> (helper_get_main()->get_user_interaction());

            if (pclient != nullptr)
            {

               pclient->datastream()->get("gcom backimpact::image_change::m_strCurrentImagePath", m_strCurrentImagePath);

            }


         }

         m_bInitialPath = true;

         string strPath(m_strCurrentImagePath);

         main * pmain = helper_get_main();

         pmain->get_next_image_path(strPath);

         if (strPath.is_empty())
         {

            m_bLoadImage = false;

            return;

         }

         m_bLastLoadImageSynch = false;

         LoadImageAsync(strPath);

         m_bLoadImage = false;

      });

      return true;

   }


   bool image_change::LoadImageSync(const ::scoped_string & scopedstrImagePath)
   {


      m_strCurrentImagePath = scopedstrImagePath;

      main * pmain = helper_get_main();

      ::database::client * pclient = dynamic_cast < ::database::client * > (pmain->get_user_interaction());

      if(pclient != nullptr)
      {

         pclient->datastream()->set("gcom backimpact::image_change::m_strCurrentImagePath", m_strCurrentImagePath);

      }

      string str;


      auto pimage = image()->load_image(scopedstrImagePath);

      if (!pimage)
      {

         //if (!psystem->imaging().LoadImage(m_pimage, lpcwszImagePath))
         //{

         //   return false;

         //}

         return false;

      }

      m_pimage = pimage;

      informationf("image_change::OnLoadImageSynch lpcwszImagePath.lock");

      pmain->get_gcom()->get_image(e_image_source)->copy_from(m_pimage);

      informationf("image_change::OnLoadImageSynch slGdi.UnLock");

      pmain->image_change_post_event(e_event_loaded);

      return true;

   }


   class ::time image_change::get_background_change_period()
   {

      return m_timeBackgroundChangePeriod;

   }


   void image_change::set_background_change_period(const class time & time)
   {
      
      m_timeBackgroundChangePeriod = maximum(time, 1_s);

   }


} // namespace backimpact








