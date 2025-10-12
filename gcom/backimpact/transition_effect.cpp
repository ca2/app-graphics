#include "framework.h"
#include "transition_effect.h"
#include "visual_effect.h"
#include "gcom.h"
#include "user_interaction.h"
#include "acme/platform/system.h"
#include "acme/parallelization/mutex.h"
#include "acme/parallelization/synchronous_lock.h"
#include "acme/platform/keep.h"
#include "acme/prototype/geometry2d/ellipse.h"
#include "acme/prototype/mathematics/mathematics.h"
#include "apex/database/_binary_stream.h"
#include "aura/graphics/draw2d/lock.h"
#include "aura/graphics/draw2d/path.h"
#include "aura/graphics/image/drawing.h"
#include "aura/graphics/image/image.h"
#include "aura/user/user/frame_interaction.h"
#include "app-core/gcom/gcom/space.h"
#include "acme/prototype/geometry2d/_defer_bounding_box.h"

#include <math.h>
#include <time.h>


namespace backimpact
{

#define HIDOUBLETOINT(d) (((d) > (int) (d)) ? ((int) (d)) + 1 : (int) (d))


   transition_effect::transition_effect()
   {
      m_timeDelay = 20_ms;
      m_bRun = true;
      m_bDestroy = false;
      m_bTransitionStepRunActive = false;
      m_bTransitionStepPostActive = false;
      m_timeTransitionStepLastRunTime.Now();
      m_timeTransitionStepPostActive.Now();
      m_timeLastTransitionTime= ::time::now();

      m_timeLastTransitionTime= ::time::now();


   }


   transition_effect::~transition_effect()
   {

      m_bDestroy = true;

   }


   void transition_effect::initialize_backimpact_helper(main * pmain)
   {

      //auto estatus = 
      
      helper::initialize_backimpact_helper(pmain);


      int iStart;
      int iEnd;

      iStart = ((int)TransitionEffectStart) + 1;
      iEnd = ((int)TransitionEffectEnd) - 1;

      //iStart  = ((int) TransitionEffectSquarypixelate_);
      //iEnd    = ((int) TransitionEffectSquarypixelate_);
      //iStart  = ((int) TransitionEffectHexagonalpixelate_);
      //iEnd    = ((int) TransitionEffectHexagonalpixelate_);
      //iStart  = ((int) TransitionEffectCirclypixelate_);
      //iEnd    = ((int) TransitionEffectCirclypixelate_);
      //iStart    = ((int) TransitionEffectEllipsoidalpixelate_);
      //iEnd    = ((int) TransitionEffectEllipsoidalpixelate_);

      //iStart = (int) TransitionEffectSingleStep;
      //iEnd   = (int) TransitionEffectSingleStep;

      //iStart  = (int) TransitionEffectLinearFadingTopBottom;
      //iEnd    = (int) TransitionEffectLinearFadingRightLeft;

      //iStart  = (int) TransitionEffectAccumulLinearFadingTopBottom;
      //iEnd    = (int) TransitionEffectAccumulLinearFadingRightLeft;
      //iStart = (int)TransitionEffectAccumulLinearFadingLeftRight;
      //iEnd    = (int) TransitionEffectAccumulLinearFadingRightLeft;

      //iStart   = (int) TransitionEffectFlyInTopBottom;
      //iEnd     = (int) TransitionEffectFlyInRightTop;

      //iStart   = (int) TransitionEffectWipeBottom;
      //iEnd     = (int) TransitionEffectWipeIn;

      //iStart   = (int) TransitionEffectScaleBottom;
      //iEnd     = (int) TransitionEffectScaleCenter;

      //iStart   = (int) TransitionEffectpixelate_TopBottom;
      //iEnd     = (int) TransitionEffectpixelate_RightLeft;

      //iStart   = (int) TransitionEffectSquarypixelate_

      //iStart   = (int) TransitionEffectVisual;
      //iEnd     = (int) TransitionEffectVisual;

      //iStart      = (int) TransitionEffectRadialUnveil;
      //iEnd      = (int) TransitionEffectRadialUnveil;

      //iStart   = (int) TransitionEffectpixelate_RightLeft;
      //iEnd     = (int) TransitionEffectRadialUnveil;

      //iStart   = (int)TransitionEffectRadialUnveil;
      //iEnd     = (int) TransitionEffectRadialUnveil;

      //iEnd     = (int) TransitionEffectVisual - 1;

      for (int i = iStart; i <= iEnd; i++)
      {
         m_etypea.add((ETransitionEffect)i);
      }

      iStart = ((int)VisualEffectStart) + 1;
      iEnd = ((int)VisualEffectEnd) - 1;

      //iStart = iEnd = (int) VisualEffectPixelExplosion;
      //iStart = iEnd = (int) VisualEffectAlphaPixelExplosion;
      //iStart = iEnd = (int) VisualEffectPixelExplosion2;

      //iStart = (int)VisualEffectRotateEx8;
      //iStart = iEnd = (int) VisualEffectRotateEx8;
      //iStart = iEnd = (int) VisualEffectRotateEx4;
      //iStart = iEnd = (int) VisualEffectRain1;
      //iStart = iEnd = (int) VisualEffectRotateEx5;
      //iStart = iEnd = (int) VisualEffectRotateEx6;
      //iStart = iEnd = (int) VisualEffectRotateEx7;
      //iEnd = (int)VisualEffectExpand8;

      //iStart = iEnd = (int) VisualEffectExpand8;
      //iStart = iEnd = (int) VisualEffectExpand4;
      //iStart = iEnd = (int) VisualEffectExpand5;
      //iStart = iEnd = (int) VisualEffectExpand6;
      //iStart = iEnd = (int) VisualEffectExpand7;

      //iStart = iEnd = (int) VisualEffectRotateBlend;
      //iStart = iEnd = (int) VisualEffectNoPrecisionRotateBlend;
      //iStart = iEnd = (int) VisualEffectNoPrecisionRotatecolor_blend_;
      //iStart = iEnd = (int) VisualEffectNoPrecisionRotateTrackcolor_blend_;
      //iStart = iEnd = (int) VisualEffectRotateEx1;
      //iStart = iEnd = (int) VisualEffectRotateEx2;
      //iEnd = (int) VisualEffectRotateEx3;
      //iEnd = (int)iStart;

      if (m_etypea.contains(TransitionEffectVisual))
      {

         for (int i = iStart; i <= iEnd; i++)
         {

            m_iaVisual.add(i);

         }

      }


      m_bInitialized = false;
      m_bActive = false;
      m_iType = mathematics()->random(0, m_etypea.get_upper_bound());
      m_iVisual = 0;

      m_iInit = 0;


      //if (!estatus)
      //{

      //   return estatus;

      //}

      //estatus = 
      
      øconstruct_new(m_ptool001);

      
      m_ptool001->initialize_backimpact_helper(pmain);



      //if (!estatus)
      //{

      //   return estatus;

      //}

      //return estatus;

   }


   void transition_effect::destroy()
   {

      //auto estatus = 
      
      helper::destroy();

      //return estatus;

   }


   void transition_effect::StepBack()
   {

      m_bActive = false;

      auto pmain = helper_get_main();

      if (!pmain)
      {

         return;

      }

      ::collection::index iTypeOld = m_iType;

      ::collection::index iTypeNew = m_iType;

      if(m_etypea[iTypeOld] == TransitionEffectVisual)
      {

         m_iVisual--;

         if(m_iVisual < 0)
         {

            m_iVisual = 0;

            iTypeNew = iTypeOld - 1;

         }

      }
      else
      {

         iTypeNew = iTypeOld - 1;

      }

      if(iTypeNew < 0)
      {

         iTypeNew = m_etypea.get_size() - 1;

         if(m_etypea[iTypeNew] == TransitionEffectVisual)
         {

            m_iVisual = pmain->get_visual_effect()->get_effect_count() - 1;

         }

      }

      if (iTypeNew >= m_etypea.get_size())
      {

         iTypeNew = m_etypea.get_size() - 1;

      }

      if(m_etypea[iTypeNew] == TransitionEffectVisual &&
            m_etypea[iTypeOld] != TransitionEffectVisual)
      {

         m_iVisual = pmain->get_visual_effect()->get_effect_count() - 1;

      }

      if (iTypeNew < 0 || iTypeNew >= m_etypea.get_size())
      {

         m_iType = 0;

      }
      else
      {

         m_iType = iTypeNew;

      }

   }


   void transition_effect::StepNext()
   {

      m_bActive = false;

      ::collection::index iTypeOld = m_iType;

      ::collection::index iTypeNew = m_iType;

      if(m_etypea[iTypeOld] == TransitionEffectVisual)
      {

         m_iVisual++;

         if(m_iVisual >= m_iaVisual.get_size())
         {

            m_iVisual = 0;

            iTypeNew = iTypeOld + 1;

         }

      }
      else
      {

         iTypeNew = iTypeOld + 1;

      }

      if(iTypeNew >= m_etypea.get_size())
      {

         iTypeNew = 0;

         if(m_etypea[iTypeNew] == TransitionEffectVisual)
         {

            m_iVisual = 0;

         }

      }

      if (iTypeNew >= m_etypea.get_size())
      {

         iTypeNew = m_etypea.get_size() - 1;

      }

      if(m_etypea[iTypeNew] == TransitionEffectVisual &&
            m_etypea[iTypeOld] != TransitionEffectVisual)
      {

         m_iVisual = 0;

      }

      if (iTypeNew < 0 || iTypeNew >= m_etypea.get_size())
      {

         m_iType = 0;

      }
      else
      {

         m_iType = iTypeNew;

      }

   }


   void transition_effect::initialize_transition_effect()
   {

      auto pmain = helper_get_main();

      if (!pmain)
      {

         //return error_failed;

         throw ::exception(error_null_pointer);

      }

      gcom * pgcom = pmain->get_gcom();

      single_lock sl1Back(pgcom->m_pmutex1Back, false);

      pmain->on_after_image_loaded();

      m_bInitialized = true;

      m_bActive = true;

      pmain->defer_check_layout();

      //return ::success;

   }


   void transition_effect::_Final()
   {

      m_bActive = false;

      m_ptool001->Finalize();

      m_bInitialized = false;

   }


   void transition_effect::Restart()
   {

      ::int_rectangle rectangleX;

      main * pmain = helper_get_main();

      user_interaction * puserinteraction = pmain->get_user_interaction();

      puserinteraction->backimpact_get_client_rect(rectangleX);

      int cx = rectangleX.width();

      int cy = rectangleX.height();

      m_ptool001->m_iStep = 1;

      m_ptool001->m_iStepCount = -1;

      m_timeDelay = 200_ms;

      m_ptool001->Initialize(m_etypea[m_iType], cx, cy, *this);

      pmain->on_after_image_loaded();

   }


   bool transition_effect::TestEnd()
   {

      if(m_ptool001->m_iStep >= m_ptool001->m_iStepCount)
      {

         m_bActive = false;

         return true;

      }

      if (!m_bActive)
      {

         return true;

      }

      return false;

   }


   void transition_effect::End()
   {

      main * pmain = helper_get_main();

      pmain->image_change_post_event(e_event_transition_effect_finished);

   }


   void transition_effect::Reset()
   {

      m_ptool001->m_ia.erase_all();

      m_ptool001->m_iStep = 0;

   }


   void transition_effect::RunStepProcLevel2()
   {

      KEEP(m_bTransitionStepRunActive);

      int iRepeat = CalcRepeatCount();

      auto pmain = helper_get_main();

      if(!pmain->get_user_interaction()->is_window_visible()          ||
            (pmain->get_user_interaction()->parent_frame() != nullptr && pmain->get_user_interaction()->parent_frame()->layout().is_iconic())
            ||           (pmain->get_user_interaction()->top_level_frame() != nullptr && pmain->get_user_interaction()->top_level_frame()->layout().is_iconic()))
      {

         sleep(200_ms);

      }
      else
      {

         int_rectangle_array recta;

         while(!TestEnd() && iRepeat > 0)
         {

            RunStepProcLevel1(recta);

            iRepeat--;

         }

      }

      m_timeLastTransitionTime.Now();

      m_timeTransitionStepLastRunTime.Now();

      m_bTransitionStepPostActive = false;

      if(TestEnd())
      {

         End();

      }

   }


   void transition_effect::RunStepProcLevel1(int_rectangle_array & recta)
   {

      if (TestEnd())
      {

         return;

      }

      if(m_ptool001->m_iStep == 1 && m_ptool001->m_ia.get_size() <= 0)
      {

         //synchronous_lock synchronouslock(gcom_pmutex(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);

         on_step(recta);

      }
      else
      {

         if(m_ptool001->m_ia.get_size() <= 0)
         {

            //synchronous_lock synchronouslock(gcom_pmutex(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);

            //auto puserinteraction = helper_get_main()->get_user_interaction();

            on_step(recta);

         }
         else
         {

            ::int_rectangle rectangle;

            for(int i = 0; i < 1 && m_ptool001->m_ia.get_size(); i++)
            {

               //synchronous_lock synchronouslock(gcom_pmutex(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);

               on_step(recta);

            }

         }

      }

      if (m_timeDelay > 0_s)
      {

         preempt(m_timeDelay);

      }

      m_ptool001->m_iStep++;

   }


   void transition_effect::on_step(int_rectangle_array & recta)
   {

      synchronous_lock synchronouslockGcom(gcom_pmutex(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);

      if (!m_bInitialized)
      {

         initialize_transition_effect();

      }

      recta.erase_all();

      main * pmain = helper_get_main();

      if(m_etypea[m_iType] == TransitionEffectVisual)
      {

         pmain->get_visual_effect()->on_render_buffer(recta);

         return;

      }

      pmain->defer_check_layout();

      synchronouslockGcom.unlock();

      ::draw2d::lock lock(this);

      synchronouslockGcom.lock();

      user_interaction * puserinteraction = pmain->get_user_interaction();

      ::int_rectangle rectangleX;

      puserinteraction->backimpact_get_client_rect(rectangleX);

      int cx = rectangleX.width();

      int cy = rectangleX.height();

      //auto psystem = m_papplication;

      //class imaging & imaging = psystem->imaging();

      gcom * pgcom = pmain->get_gcom();

      synchronous_lock sl1Back(pgcom->m_pmutex1Back, DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);

      synchronous_lock sl2Buffer(pgcom->m_pmutex2Buffer, DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);

      ::draw2d::graphics_pointer dcBack = pgcom->GetBackDC();

      ::draw2d::graphics_pointer dcBuffer = pgcom->GetBufferDC();

      ::draw2d::graphics_pointer dcSource = pgcom->GetSourceDC();

      ::draw2d::graphics_pointer dcFrame1 = pgcom->GetFrame1DC();

      ::draw2d::bitmap_pointer bitmapBuffer = pgcom->GetBufferBitmap();

      ::image::image_pointer pimageBack = pgcom->get_image(e_image_back);

      ::image::image_pointer pimageSource = pgcom->get_image(e_image_source);

      if(dcBack.is_null() || dcBack->get_os_data() == nullptr)
      {
         End();
         return;
      }
      if(dcBuffer->get_os_data() == nullptr)
      {
         End();
         return;
      }

      ::draw2d::save_context k(dcBack);
      ::draw2d::save_context k2(dcBuffer);
//      dcBack->SelectClipRgn(nullptr);
      //    dcBuffer->SelectClipRgn(nullptr);

      //if(!bitmapBuffer->is_set(::draw2d::e_default_object))
      //{

      //   End();

      //   return;

      //}

//         ::int_size  sizeBack2 = bitmapBuffer->get_size();
                  //double dBase = 40.0;
      constexpr double dGamma = 0.5; // ? Parable

      constexpr auto gamma_fix = [](double dRate)
      {

         double dRateClipped = minimum_maximum(dRate, 0.0, 1.0);

         double d = dRateClipped * 2.0;

         if (d <= 1.0)
         {

            return pow(d, 1.0 / dGamma) * 0.5;

         }
         else
         {

            return 1.0 - pow(2.0 - d, 1.0 / dGamma) * 0.5;

         }

         //double d = pow(dBase, dRateClipped * 0.9) / dBase;

         //return ;

      };


      ::int_rectangle rectangleUpdate;
      switch(m_etypea[m_iType])
      {
      case TransitionEffectSingleStep:
      {
         if(m_ptool001->m_iStep == m_ptool001->m_iStepCount / 2)
         {
            //dcBack->fill_rectangle(100, 10, 50, 50, argb(20, 30, 40, 20));

            rectangleUpdate = rectangleX;

            {

               ::image::image_source imagesource(dcBuffer, rectangleUpdate);

               ::image::image_drawing_options imagedrawingoptions(rectangleUpdate);

               ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

               dcBack->draw(imagedrawing);

            }

            /*                  drawpimage->draw(
                              dcBack,
                              rectangleUpdate.left(), rectangleUpdate.top(),
                              rectangleUpdate.width(), rectangleUpdate.height(),
                              pgcom->get_image(e_image_buffer),
                              rectangleUpdate.left(), rectangleUpdate.top(),
                              rectangleUpdate.width(), rectangleUpdate.height(),
                              SRCCOPY);
            */
            recta.add(rectangleUpdate);
         }
      }
      break;
      case TransitionEffectSquarypixelate_:
      {
         if(m_ptool001->m_iStep > 0)
         {
            if(m_ptool001->m_pointa.get_size() <= 0)
               break;
            ::collection::index iIndex = (::collection::index)mathematics()->random(0, m_ptool001->m_pointa.get_size() - 1);



            if(m_ptool001->m_iStep <= 0)
               break;;
            ::int_point point;
            try
            {
               point = m_ptool001->m_pointa.element_at(iIndex);
               if(m_ptool001->m_iStep <= 0)
                  break;
               m_ptool001->m_pointa.erase_at(iIndex);
            }
            catch(...)
            {
               break;
            }
            //::double_point pointa[4];

            m_ptool001->m_polygon.set_size(4);

            m_ptool001->GetRotateRect(point.x(), point.y(), m_ptool001->m_polygon.data());
            //::draw2d::region_pointer rgnClip(e_create);
            //rgnClip->create_polygon(pointa, 4, ::draw2d::e_fill_mode_winding);

            //dcBack->SelectClipRgn(nullptr);
            dcBack->intersect_clip(m_ptool001->m_polygon);

            ::get_bounding_box(rectangleUpdate, m_ptool001->m_polygon.data(), 4);

            /*               if(lprect != nullptr)
            {
            *lprect = rectangleUpdate;
            }*/

            {

               ::image::image_source imagesource(dcBuffer, rectangleUpdate);

               ::image::image_drawing_options imagedrawingoptions(rectangleUpdate);

               ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

               dcBack->draw(imagedrawing);

            }
            /*drawpimage->draw(
               dcBack,
               rectangleUpdate.left(), rectangleUpdate.top(),
               rectangleUpdate.width(), rectangleUpdate.height(),
               pgcom->get_image(e_image_buffer),
               rectangleUpdate.left(), rectangleUpdate.top(),
               rectangleUpdate.width(), rectangleUpdate.height(),
               SRCCOPY);*/
            dcBack->reset_clip();
            recta.add(rectangleUpdate);
         }
      }
      break;
      case TransitionEffectHexagonalpixelate_:
      {
         if(m_ptool001->m_iStep > 0)
         {
            ::collection::index iIndex = (::collection::index)mathematics()->random(0, m_ptool001->m_pointa.get_size() - 1);

            if(m_ptool001->m_iStep <= 0)
               break;
            ::int_point point;
            try
            {
               point = m_ptool001->m_pointa.element_at(iIndex);
               if(m_ptool001->m_iStep <= 0)
                  break;
               m_ptool001->m_pointa.erase_at(iIndex);
            }
            catch(...)
            {
               break;
            }
            m_ptool001->m_polygon.set_size(6);
            m_ptool001->GetRotateHexagon(point.x(), point.y(), m_ptool001->m_polygon.data());
            dcBack->intersect_clip(m_ptool001->m_polygon);
            ::get_bounding_box(rectangleUpdate, m_ptool001->m_polygon.data(), 6);

            /*               if(lprect != nullptr)
            {
            *lprect = rectangleUpdate;
               }*/

            {

               ::image::image_source imagesource(dcBuffer, rectangleUpdate);

               ::image::image_drawing_options imagedrawingoptions(rectangleUpdate);

               ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

               dcBack->draw(imagedrawing);

            }

            dcBack->reset_clip();
         }
         recta.add(rectangleUpdate);
      }
      break;
      case TransitionEffectCirclypixelate_:
      case TransitionEffectEllipsoidalpixelate_:
      {
         int finalX = 0;
         int finalY = 0;
         int finalW = cx;
         int finalH = cy;

         const int tilesx = maximum(4, m_ptool001->m_data.m_tiles.m_iTilesX);
         const int tilesy = maximum(4, m_ptool001->m_data.m_tiles.m_iTilesY);
//               const int tiles = maximum(16, m_ptool001->m_data.m_tiles.m_iTiles);


         if(m_ptool001->m_iStep > 0)
         {
            if(m_ptool001->m_ia.get_size() > 0)
            {
               ::collection::index iIndex = (::collection::index)mathematics()->random(0, m_ptool001->m_ia.get_size() - 1);

               //                  SendMessage(WM_USER, USERMESSAGE_WPARAM_RAND_LPINT, (LPARAM) &iRand);

               /*                  int iIndex =
               m_math.LinearMap(
               0, m_ptool001->m_ia.get_size() - 1,
               iRand, 0, RAND_MAX);*/
               if(m_ptool001->m_iStep <= 0)
                  break;
               int iStep = 0;
               try
               {
                  iStep = m_ptool001->m_ia.element_at(iIndex);
                  if(m_ptool001->m_iStep <= 0)
                     break;
                  m_ptool001->m_ia.erase_at(iIndex);
               }
               catch(...)
               {
                  break;
               }

               int i = iStep % tilesx;
               int j = iStep / tilesx;
               double dUpdateH = (double) finalH / tilesy;
               double dUpdateW = (double) finalW / tilesx;
               double dX = dUpdateW * i;
               double dY = dUpdateH * j;
               rectangleUpdate.set(
               (int) dX,
               (int) dY,
               (int) (dX + dUpdateW + 1.0),
               (int) (dY + dUpdateH + 1.0));
            }
            else
            {
               rectangleUpdate.set(finalX, finalY, finalX + finalW, finalY + finalH);
            }
            /*               if(lprect != nullptr)
            {
            *lprect = rectangleUpdate;
               }*/

            ::draw2d::region_pointer rgnClip(e_create, this);

            if(m_etypea[m_iType] == TransitionEffectCirclypixelate_
                  || m_etypea[m_iType] == TransitionEffectEllipsoidalpixelate_)
            {

               ::int_rectangle rectangleEllipse;

               if(m_etypea[m_iType] == TransitionEffectCirclypixelate_)
               {

                  m_ptool001->FindRandomEnglobingCircle(&rectangleUpdate, &rectangleEllipse, 64);

               }
               else
               {

                  m_ptool001->FindRandomEnglobingEllipse(&rectangleUpdate, &rectangleEllipse, 64);

               }

               rectangleUpdate = rectangleEllipse;

               auto ppathClip = øcreate < ::draw2d::path >();

               ::double_ellipse ellipse;
               
               ellipse.set(rectangleUpdate);

               ppathClip->add_ellipse(ellipse);

               dcBack->intersect_clip(ppathClip);

               {

                  ::image::image_source imagesource(dcBuffer, rectangleUpdate);

                  ::image::image_drawing_options imagedrawingoptions(rectangleUpdate);

                  ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

                  dcBack->draw(imagedrawing);

               }

               dcBack->reset_clip();

            }
            else
            {
               {

                  ::image::image_source imagesource(dcBuffer, rectangleUpdate);

                  ::image::image_drawing_options imagedrawingoptions(rectangleUpdate);

                  ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

                  dcBack->draw(imagedrawing);

               }

            }
            recta.add(rectangleUpdate);
         }

      }
      break;
      case TransitionEffectLinearFadingBottomTop:
      case TransitionEffectLinearFadingTopBottom:
      case TransitionEffectLinearFadingLeftRight:
      case TransitionEffectLinearFadingRightLeft:
      {

         int finalX = 0;
         int finalY = 0;
         int finalW = cx;
         int finalH = cy;

         ::int_rectangle & rectangleUpdate = m_ptool001->m_rectangle;

         if(m_ptool001->m_iStep == 1)
         {

            pgcom->get_image(e_image_temp1)->copy_from(pimageBack);

         }

         pimageBack->copy_from(pgcom->get_image(e_image_temp1));

         double dRate = ((double) (m_ptool001->m_iStep + 1.0)) / ((double) m_ptool001->m_iStepCount);

         rectangleUpdate.set(finalX, finalY, finalX + finalW, finalY + finalH);

         dcBack->set_alpha_mode(::draw2d::e_alpha_mode_blend);

         {

            ::image::image_source imagesource(dcBuffer);

            ::image::image_drawing_options imagedrawingoptions(rectangleUpdate);

            imagedrawingoptions.opacity(dRate);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

            dcBack->draw(imagedrawing);

         }

         recta.add(rectangleUpdate);

      }
      break;
      case TransitionEffectAccumulLinearFadingBottomTop:
      case TransitionEffectAccumulLinearFadingTopBottom:
      case TransitionEffectAccumulLinearFadingLeftRight:
      case TransitionEffectAccumulLinearFadingRightLeft:
      {

         int finalX = 0;

         int finalY = 0;

         int finalW = cx;

         int finalH = cy;

         ::int_rectangle & rectangleUpdate = m_ptool001->m_rectangle;

         const int iTileCount = m_ptool001->m_data.m_sliceframe.m_iTileCount;

         const int iTileMax = iTileCount - 1;

         //int iGroupCount = m_ptool001->m_data.m_sliceframe.m_iGroupCount;

         ::collection::index iIndex = m_ptool001->m_iStep - 1;

         iIndex = minimum(iIndex, m_ptool001->m_pointa.get_size() -1);

         int iTile = -1;

         if(iIndex >= 0)
         {

            if (iTile == 0)
            {

               m_ptool001->m_dMinimumAlpha = 1.0;

            }

            ::int_point point = m_ptool001->m_pointa.element_at(iIndex);

            iTile = point.x();

            ///int iGroup = point.y();

            double dRate = (double)( iTile + 1) /(double)(iTileMax);

            double dRateMinus = (double) iTile / (double)(iTileMax);

            int iImageCount = m_ptool001->m_data.m_sliceframe.m_iImageCount;

            int iIntensity = 0;

            if(m_ptool001->m_iStep > 0
                  && m_ptool001->m_iStepCount > 0)
            {

               m_ptool001->Start(cx, cy, dRateMinus, dRate);
               
               m_ptool001->Go();

               m_ptool001->m_ia.element_at(iTile)++;
               
               if(m_ptool001->m_ia[iTile] >= iImageCount)
               {

                  m_ptool001->m_ia.element_at(iTile) = iImageCount;

               }

               iIntensity = m_ptool001->m_ia[iTile];

            }
            else
            {

               rectangleUpdate.set(finalX, finalY, finalX + finalW, finalY + finalH);

            }

            rectangleUpdate.intersect(pgcom->m_rectangleFinalPlacement);

            auto pimageBuffer = pgcom->get_image(e_image_buffer);

            if(iIndex == 0)
            {

               for (::collection::index i = 0; i <= iImageCount; i++)
               {

                  auto pimage = pgcom->get_image((enum_image)i);

                  pimage->create(pimageBuffer->size());

                  if (pimage.ok())
                  {

                     pimage->copy_from(pimageBack);

                     ::image::image_source imagesource(pimageBuffer);

                     double_rectangle rectangle(pimageBuffer->size());

                     ::image::image_drawing_options imagedrawingoptions(rectangle);

                     imagedrawingoptions.m_bIntegerPlacement = true;

                     {

                        double dRate = (double)(i+1) / (double)(iImageCount + 1);

                        imagedrawingoptions.opacity(gamma_fix(dRate));

                        ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

                        pimage->g()->set_alpha_mode(::draw2d::e_alpha_mode_blend);

                        pimage->draw(imagedrawing);

                     }

                  }

               }

            }

            double dAlpha = (double) iIntensity / (double) iImageCount;

            if(dAlpha > 0 && dAlpha <= 1.0 && !rectangleUpdate.is_empty())
            {

               auto pimage = pgcom->get_image((enum_image)iIntensity);

               if (pimage.ok())
               {

                  ::image::image_source imagesource(pimage, rectangleUpdate);

                  double_rectangle rectangle(rectangleUpdate);

                  ::image::image_drawing_options imagedrawingoptions(rectangle);

                  imagedrawingoptions.m_bIntegerPlacement = true;

                  ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

                  pimageBack->g()->set_alpha_mode(::draw2d::e_alpha_mode_set);

                  pimageBack->draw(imagedrawing);

                  recta.add(rectangleUpdate);

               }

            }

            if (iTile == iTileCount)
            {

               if (m_ptool001->m_dMinimumAlpha >= 0.9999)
               {

                  m_ptool001->m_iStep = m_ptool001->m_iStepCount;

               }

            }

         }

      }
      break;
      case TransitionEffectFlyInBottomTop:
      case TransitionEffectFlyInTopBottom:
      case TransitionEffectFlyInLeftRight:
      case TransitionEffectFlyInRightLeft:
      case TransitionEffectFlyInLeftTop:
      case TransitionEffectFlyInLeftBottom:
      case TransitionEffectFlyInRightBottom:
      case TransitionEffectFlyInRightTop:
      {
//               int finalX = 0;
//               int finalY = 0;
//               int finalW = cx;
//               int finalH = cy;

         const int iFrameCount = maximum(1, m_ptool001->m_data.m_fly.m_iFrameCount);
         const int iFrameMax = iFrameCount -1;



         ::collection::index iFrame = m_ptool001->m_iStep - 1;
         double & dFlyRate = m_ptool001->m_data.m_fly.m_dRate;
         if (iFrame == 0)
         {

            dFlyRate = 0.0;

         }

         double dFrameStd = (double) iFrame / (double) iFrameMax;
         dFlyRate = dFlyRate + ((1.0 - dFlyRate)/ sqrt((double)iFrameMax));
         double dRate = minimum_maximum(dFlyRate, 0., 1.);
         double dComplementRate = 1.0 - dRate;
         double dAlpha = gamma_fix(dFrameStd);

         ::int_rectangle rectangleBound;
         pgcom->GetFinalPlacement(&rectangleBound);

         ::int_rectangle rectangle(rectangleBound);

         ::gcom::space space;
         space.deviate(&rectangle, &rectangleBound, GetDirection(m_etypea[m_iType]), dComplementRate);

         ::int_rectangle rectangleIntersect;
         rectangleIntersect.intersect(rectangle, rectangleBound);

         ::double_point pointSrc(rectangleIntersect.left() - rectangle.left() + rectangleBound.left(),
            rectangleIntersect.top() - rectangle.top() + rectangleBound.top());

         auto pimageFrame1 = pgcom->get_image(e_image_frame1);

         ::int_rectangle rectangleX(pimageFrame1->size());

         {

            ::image::image_source imagesource(pimageFrame1);

            ::image::image_drawing_options imagedrawingoptions(rectangleX);

            dcBack->set_alpha_mode(::draw2d::e_alpha_mode_set);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

            dcBack->draw(imagedrawing);

         }


         //auto pimageFinal = pgcom->get_image(e_image_final);
         auto pimageBuffer = pgcom->get_image(e_image_buffer);
         {

           // ::image::image_source imagesource(pimageFinal, ::double_rectangle(pointSrc, rectangleIntersect.size()));
            ::image::image_source imagesource(pimageBuffer, ::double_rectangle(pointSrc, rectangleIntersect.size()));

            ::image::image_drawing_options imagedrawingoptions(rectangleIntersect);

            imagedrawingoptions.opacity(dAlpha);

            dcBack->set_alpha_mode(::draw2d::e_alpha_mode_blend);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

            dcBack->draw(imagedrawing);

         }

         recta.add(rectangleIntersect);

      }
      break;
      case TransitionEffectWipeTop:
      case TransitionEffectWipeBottom:
      case TransitionEffectWipeRight:
      case TransitionEffectWipeLeft:
      {
//               int finalX = 0;
//               int finalY = 0;
//               int finalW = cx;
//               int finalH = cy;

         const int iFrameCount = maximum(4, m_ptool001->m_data.m_sliceframe.m_iFrameCount);
         const int iFrameMax = iFrameCount;

         ::collection::index iFrame = m_ptool001->m_iStep - 1;
         double dFrameStd = 1.0 - ((double) iFrame / iFrameMax);
         double dFrameStdPlus = 1.0 - ((double) (iFrame + 1) / iFrameMax);
         double dRate = 1.0 - dFrameStd * dFrameStd;

         double dRatePlus = 1.0 - dFrameStdPlus * dFrameStdPlus;


         ::int_rectangle rectangle;
         pgcom->GetFinalPlacement(&rectangle);

         ::gcom::space space;

         space.slice(&rectangle, &rectangleX, GetDirection(get_type()), dRate, dRatePlus);

         {

            ::image::image_source imagesource(dcBuffer, rectangle);

            ::image::image_drawing_options imagedrawingoptions(rectangle);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

            dcBack->draw(imagedrawing);

         }

         recta.add(rectangle);

      }
      break;
      case TransitionEffectWipeTopLeft:
      case TransitionEffectWipeTopRight:
      case TransitionEffectWipeBottomLeft:
      case TransitionEffectWipeBottomRight:
      {
//               int finalX = 0;
//               int finalY = 0;
//               int finalW = cx;
//               int finalH = cy;

         const int iFrameCount = maximum(4, m_ptool001->m_data.m_sliceframe.m_iFrameCount);
         const int iFrameMax = iFrameCount - 1;

         ::collection::index iFrame = m_ptool001->m_iStep - 1;
//               double dFrameStdMinus = 1.0 - ((double) (iFrame - 1)/ iFrameMax);
         double dFrameStd = 1.0 - ((double) iFrame / iFrameMax);
         double dFrameStdPlus = 1.0 - ((double) (iFrame + 1) / iFrameMax);
//               double dRateMinus = 1.0 - dFrameStdMinus * dFrameStdMinus;
         double dRate = 1.0 - dFrameStd * dFrameStd;
         double dRatePlus = 1.0 - dFrameStdPlus * dFrameStdPlus;

         ::int_rectangle rectangleA;
         ::int_rectangle rectangleB;
         ::int_rectangle rectangleC;
//               ::int_rectangle & rectangleUpdate = m_ptool001->m_rectangle;




         ::int_rectangle rectangle;
         pgcom->GetFinalPlacement(&rectangle);


         ::gcom::space space;
         space.slice(&rectangleA, &rectangleB, &rectangleC, &rectangle, GetDirection(get_type()), dRate, dRatePlus);


         {

            ::image::image_source imagesource(dcBuffer, rectangleA);

            ::image::image_drawing_options imagedrawingoptions(rectangleA);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

            dcBack->draw(imagedrawing);

         }

         {

            ::image::image_source imagesource(dcBuffer, rectangleB);

            ::image::image_drawing_options imagedrawingoptions(rectangleB);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

            dcBack->draw(imagedrawing);

         }

         {

            ::image::image_source imagesource(dcBuffer, rectangleC);

            ::image::image_drawing_options imagedrawingoptions(rectangleC);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

            dcBack->draw(imagedrawing);

         }

         recta.add(rectangleA);
         recta.add(rectangleB);
         recta.add(rectangleC);

      }
      break;
      case TransitionEffectWipeCenter:
      {

         const int iFrameCount = maximum(4, m_ptool001->m_data.m_sliceframe.m_iFrameCount);
         const int iFrameMax = iFrameCount - 1;

         ::collection::index iFrame = m_ptool001->m_iStep - 1;
         double dFrameStd = 1.0 - ((double) iFrame / iFrameMax);
         double dRate = 1.0 - dFrameStd * dFrameStd;

         ::int_rectangle rectangle;
         pgcom->GetFinalPlacement(&rectangle);

         ::gcom::space space;
         space.scale(&rectangle, dRate);

         ::image::image_source imagesource(dcBuffer, rectangle);

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         recta.add(rectangle);

      }
      break;
      case TransitionEffectWipeIn:
      {

         const int iFrameCount = maximum(4, m_ptool001->m_data.m_sliceframe.m_iFrameCount);
         const int iFrameMax = iFrameCount - 1;

         ::collection::index iFrame = m_ptool001->m_iStep - 1;
         double dFrameStd = 1.0 - ((double) iFrame / iFrameMax);
         double dFrameStdPlus = 1.0 - ((double) (iFrame + 1) / iFrameMax);
         double dRate = 1.0 - dFrameStd * dFrameStd;
         double dRatePlus = 1.0 - dFrameStdPlus * dFrameStdPlus;

         ::int_rectangle rectangleA;
         ::int_rectangle rectangleB;
         ::int_rectangle rectangleC;
         ::int_rectangle rectangleD;

         ::int_rectangle rectangle;
         pgcom->GetFinalPlacement(&rectangle);

         ::gcom::space space;
         space.wipe_in(
         &rectangleA,
         &rectangleB,
         &rectangleC,
         &rectangleD,
         &rectangle,
         1.0 - dRatePlus,
         1.0 - dRate);

         {

            ::image::image_source imagesource(dcBuffer, rectangleA);

            ::image::image_drawing_options imagedrawingoptions(rectangleA);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

            dcBack->draw(imagedrawing);

         }

         {

            ::image::image_source imagesource(dcBuffer, rectangleB);

            ::image::image_drawing_options imagedrawingoptions(rectangleB);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

            dcBack->draw(imagedrawing);

         }

         {

            ::image::image_source imagesource(dcBuffer, rectangleC);

            ::image::image_drawing_options imagedrawingoptions(rectangleC);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

            dcBack->draw(imagedrawing);

         }

         {

            ::image::image_source imagesource(dcBuffer, rectangleD);

            ::image::image_drawing_options imagedrawingoptions(rectangleD);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

            dcBack->draw(imagedrawing);

         }

         recta.add(rectangleA);
         recta.add(rectangleB);
         recta.add(rectangleC);
         recta.add(rectangleD);
      }
      break;
      case TransitionEffectWipeMidHorizontal:
      {

         const int iFrameCount = maximum(4, m_ptool001->m_data.m_sliceframe.m_iFrameCount);
         const int iFrameMax = iFrameCount - 1;

         ::collection::index iFrame = m_ptool001->m_iStep - 1;
         double dFrameStd = 1.0 - ((double) iFrame / iFrameMax);
         double dRate = dFrameStd * dFrameStd;

         ::int_rectangle rectangle;
         pgcom->GetFinalPlacement(&rectangle);

         double greekdeltay = (rectangle.height() / 2) * dRate;

         rectangle.top() += (long) greekdeltay;
         rectangle.bottom() -= (long) greekdeltay;


         ::image::image_source imagesource(dcBuffer, rectangle);

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         dcBack->draw(imagedrawing);

         recta.add(rectangle);

      }
      break;
      case TransitionEffectWipeMidVertical:
      {

         const int iFrameCount = maximum(4, m_ptool001->m_data.m_sliceframe.m_iFrameCount);
         const int iFrameMax = iFrameCount - 1;

         ::collection::index iFrame = m_ptool001->m_iStep - 1;
         double dFrameStd = 1.0 - ((double) iFrame / iFrameMax);
         double dRate = dFrameStd * dFrameStd;

         ::int_rectangle rectangle;
         pgcom->GetFinalPlacement(&rectangle);

         double greekdeltax = (rectangle.width() / 2) * dRate;

         rectangle.left() += (long) greekdeltax;
         rectangle.right() -= (long) greekdeltax;

         ::image::image_source imagesource(dcBuffer, rectangle);

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         dcBack->draw(imagedrawing);

         recta.add(rectangle);

      }
      break;
      case TransitionEffectScaleBottom:
      case TransitionEffectScaleTop:
      case TransitionEffectScaleRight:
      case TransitionEffectScaleLeft:
      case TransitionEffectScaleTopLeft:
      case TransitionEffectScaleTopRight:
      case TransitionEffectScaleBottomLeft:
      case TransitionEffectScaleBottomRight:
      {

         const int iFrameCount = m_ptool001->m_data.m_sliceframe.m_iFrameCount;

         const int iFrameMax = iFrameCount - 1;

         ::collection::index iFrame = m_ptool001->m_iStep - 1;

         double dFrameStd = 1.0 - ((double) iFrame / iFrameMax);

         double dRate = dFrameStd * dFrameStd;

         ::int_rectangle rectangleBound;

         pgcom->GetFinalPlacement(&rectangleBound);

         ::int_rectangle rectangle(rectangleBound);

         ::gcom::space space;

         space.deviate(&rectangle, &rectangleX, GetDirection(get_type()), dRate);

         rectangle.intersect(rectangle, rectangleBound);

         dcBack->set_interpolation_mode(::draw2d::e_interpolation_mode_high_quality_bicubic);

         //dcBack->set_origin(0, 0);

         //pimageSource->get_graphics()->set_origin(0, 0);

         dcBack->set_alpha_mode(::draw2d::e_alpha_mode_set);

         dcBack->reset_clip();

         double_rectangle rectangleSource(
            0,
            0,
            pgcom->get_image(e_image_final)->width(),
            pgcom->get_image(e_image_final)->height());

         ::image::image_source imagesource(pgcom->get_image(e_image_final), rectangleSource);

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         dcBack->draw(imagedrawing);

         recta.add(rectangle);

      }
      break;
      case TransitionEffectScaleCenter:
      {
//               int finalX = 0;
//               int finalY = 0;
//               int finalW = cx;
//               int finalH = cy;

         const int iFrameCount = maximum(4, m_ptool001->m_data.m_sliceframe.m_iFrameCount);
         const int iFrameMax = iFrameCount - 1;

         ::collection::index iFrame = m_ptool001->m_iStep - 1;
         double dFrameStd = 1.0 - ((double) iFrame / iFrameMax);

         double dRate = dFrameStd * dFrameStd;

         ::int_rectangle rectangle = rectangleX;

         ::gcom::space space;

         space.scale(&rectangle, dRate);

         ::image::image_source imagesource(dcBuffer, rectangle);

         ::image::image_drawing_options imagedrawingoptions(rectangleX);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         dcBack->draw(imagedrawing);

         dcBack->reset_clip();

         recta.add(rectangleX);

      }
      break;
      case TransitionEffectpixelate_BottomTop:
      case TransitionEffectpixelate_TopBottom:
      case TransitionEffectpixelate_LeftRight:
      case TransitionEffectpixelate_RightLeft:
      {


         ::int_rectangle rectangleBound;
         pgcom->GetFinalPlacement(&rectangleBound);



         int finalX = rectangleBound.left();
         int finalY = rectangleBound.top();
         int finalW = rectangleBound.width();
         int finalH = rectangleBound.height();


         ::int_rectangle rectangleDest;

         rectangleDest.left() = 0;
         rectangleDest.top() = 0;
         rectangleDest.right() = finalW;
         rectangleDest.bottom() = finalH;


         ::int_rectangle rectangleUpdate;

         int c1 = m_ptool001->m_data.m_alphapixelate.m_c1;
         ::collection::index iIndex = m_ptool001->m_iStep - 1;
         ::collection::index iSizeIndex = m_ptool001->m_pointa[iIndex].x();
         int iSize = m_ptool001->m_ia[iSizeIndex];
         int nextc = m_ptool001->m_pointa[iIndex].y();
         int ca;

         if(iIndex <= 0 || nextc == 0)
         {
            ca = 0;
         }
         else
         {
            ca = m_ptool001->m_pointa[iIndex - 1].y();
         }


         m_timeDelay = 90_ms;

         ::image::image_pointer pimage = pgcom->get_image(e_image_source);

         if(iIndex >= 0)
         {
            double dRateMinus = (double) ca / (double) c1;
            double dRate = (double) nextc / (double) c1;

            if(dRateMinus < 0.0)
               dRateMinus = 0.0;
            else if(dRateMinus > 1.0)
               dRateMinus = 1.0;

            if(dRate < 0.0)
               dRate = 0.0;
            else if(dRate > 1.0)
               dRate = 1.0;

            if(dRateMinus > dRate)
               dRateMinus = dRate;

            double dAlpha;

            {
               double dBeginSpanTime = 0.30;
               double dEndSpanTime = 0.05;
               double dTime = (double) m_ptool001->m_iStep / (double) m_ptool001->m_iStepCount;
               double dRate = 8.0;
               double dMiddle = 0.87;
               double dMiddleAlpha = 107.0;

               if(dTime < dMiddle)
               {
                  if(dTime < dBeginSpanTime)
                  {
                     dAlpha = 0.0;
                  }
                  else
                  {
                     dAlpha = dMiddleAlpha * ::pow(2.7182, ((dTime * (1.0 / (1.0 - (dBeginSpanTime + dEndSpanTime)))) - dMiddle) * dRate);
                  }
               }
               else
               {
                  if(dTime > (1.0 - dEndSpanTime))
                  {
                     dAlpha = 255.0;
                  }
                  else
                  {
                     dAlpha = dMiddleAlpha + (255.0 - dMiddleAlpha) * (1.0 - ::pow(2.7182, (dMiddle - (dTime * (1.0 / (1.0 - (dBeginSpanTime + dEndSpanTime))))) * dRate));
                  }
               }
            }




            if(nextc == 0)
            {

               m_ptool001->m_data.m_alphapixelate.m_iSizeIndex = (int) iSizeIndex;

               /* p::image::image_pointer pimage2 = pgcom->get_image(2);

                                 ::image::image_pointer pimage3 = pgcom->get_image(3);

                                 ::image::image_pointer pimage4 = pgcom->get_image(4);

                                 ::image::image_pointer pimage5 = pgcom->get_image(5);

                                 ::image::image_pointer pimage6 = pgcom->get_image(6);

                                 pimage->create(xPixelMod, yPixelMod);

                                 pimage2->create(xPixelMod, yPixelMod);

                                 image3.create(finalW, finalH);

                                 image4.create(finalW, finalH);

                                 image5 = create_image({xPixelMod,  yPixelMod});

                                 image6 = create_image({xPixelMod,  yPixelMod});

                                 image5.get_graphics()->set_interpolation_mode(0);
                                 image6.get_graphics()->set_interpolation_mode(0);
                                 dcBuffer->set_interpolation_mode(0);
                                 dcFrame1.set_interpolation_mode(0);

                                 image5.get_graphics()->StretchBlt(
                                    0, 0,
                                    xPixelMod, yPixelMod,
                                    dcBuffer,
                                    finalX, finalY,
                                    finalW, finalH,
                                    SRCCOPY);

                                 image6.get_graphics()->StretchBlt(
                                    0, 0,
                                    xPixelMod, yPixelMod,
                                    &dcFrame1,
                                    finalX, finalY,
                                    finalW, finalH,
                                    SRCCOPY);


                                 image3.get_graphics()->set_interpolation_mode(0);
                                 image4.get_graphics()->set_interpolation_mode(0);
                                 image5.get_graphics()->set_interpolation_mode(0);
                                 image6.get_graphics()->set_interpolation_mode(0);

                                 image3.get_graphics()->StretchBlt(
                                    0, 0,
                                    finalW, finalH,
                                    image5.get_graphics(),
                                    0, 0,
                                    xPixelMod, yPixelMod,
                                    SRCCOPY);

                                 image4.get_graphics()->StretchBlt(
                                    0, 0,
                                    finalW, finalH,
                                    image6.get_graphics(),
                                    0, 0,
                                    xPixelMod, yPixelMod,
                                    SRCCOPY);

                                 pimage->g()->set_interpolation_mode(0);
                                 pimage2->get_graphics()->set_interpolation_mode(0);
                                 image3.get_graphics()->set_interpolation_mode(0);
                                 image4.get_graphics()->set_interpolation_mode(0);

                                 pimage->g()->StretchBlt(
                                    0, 0,
                                    xPixelMod, yPixelMod,
                                    image3.get_graphics(),
                                    0, 0,
                                    finalW, finalH,
                                    SRCCOPY);

                                 pimage2->get_graphics()->StretchBlt(
                                    0, 0,
                                    xPixelMod, yPixelMod,
                                    image4.get_graphics(),
                                    0, 0,
                                    finalW, finalH,
                                    SRCCOPY);*/

               pimage->create({ finalW, finalH });

               {

                  ::image::image_source imagesource(dcFrame1);

                  double_rectangle rectangle(::double_size(finalW, finalH));

                  ::image::image_drawing_options imagedrawingoptions(rectangle);

                  ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

                  pimage->g()->draw(imagedrawing);

               }

               const int dSpan = 16;

               if(iSizeIndex == m_ptool001->m_ia.get_size() - 1)
               {
                  dAlpha = 255.0;
               }
               else
               {
                  if(dAlpha < dSpan)
                     dAlpha = 0.0;
                  else if(dAlpha > 255 - dSpan)
                     dAlpha = 255.0;
                  else
                  {
                     dAlpha = (dAlpha - dSpan) * 255.0 / (255.0 - dSpan * 2);
                  }
               }

               {

                  ::image::image_source imagesource(dcBuffer, double_rectangle(int_point(finalX, finalY), ::long_long_size(finalW, finalH)));

                  double_rectangle rectangle(::long_long_size(finalW, finalH));

                  ::image::image_drawing_options imagedrawingoptions(rectangle);

                  imagedrawingoptions.opacity(dAlpha);

                  ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

                  pimage->draw(imagedrawing);

               }

               pimage->pixelate(iSize);

            }

            switch(m_etypea[m_iType])
            {
            case TransitionEffectpixelate_TopBottom:
            {
               m_ptool001->GetSliceRect(finalW, finalH, &rectangleDest, ::backimpact::transition_effect::tool_001::AlignTop, dRateMinus, dRate);
            }
            break;
            case TransitionEffectpixelate_BottomTop:
            {
               m_ptool001->GetSliceRect(finalW, finalH, &rectangleDest, ::backimpact::transition_effect::tool_001::AlignBottom, dRateMinus, dRate);
            }
            break;
            case TransitionEffectpixelate_LeftRight:
            {
               m_ptool001->GetSliceRect(finalW, finalH, &rectangleDest, ::backimpact::transition_effect::tool_001::AlignLeft, dRateMinus, dRate);
            }
            break;
            case TransitionEffectpixelate_RightLeft:
            {
               m_ptool001->GetSliceRect(finalW, finalH, &rectangleDest, ::backimpact::transition_effect::tool_001::AlignRight, dRateMinus, dRate);
            }
            break;
            default:
               break;
            }

            rectangleDest.offset(rectangleBound.top_left());

            rectangleDest.right() +=1;
            rectangleDest.bottom() +=1;

            {

               ::image::image_source imagesource(pimage, 
                  double_rectangle_dimension(
                     rectangleDest.left() - rectangleBound.left(),
                     rectangleDest.top() - rectangleBound.top(), 
                     minimum(rectangleDest.width(), finalW),
                     minimum(rectangleDest.height(), finalH))
               );

               auto rectangle = double_rectangle_dimension(rectangleDest.left(),
                                          rectangleDest.top(),
                                          minimum(rectangleDest.width(), finalW),
                                          minimum(rectangleDest.height(), finalH));

               ::image::image_drawing_options imagedrawingoptions(rectangle);

               imagedrawingoptions.opacity(dAlpha);

               ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

               dcBack->draw(imagedrawing);

            }

            recta.add(rectangleDest);

         }
      }
      break;
      case TransitionEffectRadialUnveil:
      {
                  ::image::image_pointer pimageT1 = pgcom->get_image(e_image_radial_center_alpha); // Radial Fill (White, Radius = 256 pixels)
         ::image::image_pointer pimageT2 = pgcom->get_image(e_image_radial_center_alpha); // Radial Fill (White, Radius = 256 pixels)

         ::collection::index iIndex = m_ptool001->m_iStep - 1;
         m_ptool001->m_data.m_radialunveil.m_iRadius += m_ptool001->m_data.m_radialunveil.m_iRadiusIncrement;

         m_timeDelay = ::millisecond_time(m_ptool001->m_pointa[iIndex].y());

         int r = m_ptool001->m_pointa[iIndex].x();
         int d = r * 2;



         int xm = cx / 2;
         int ym = cy / 2;
         int x1 = maximum(0, xm - r);
         int x2 = minimum(cx, xm + r);
         int y1 = maximum(0, ym - r);
         int y2 = minimum(cy, ym + r);

         ::image::image_pointer pimage = pgcom->get_image(e_image_source);


         int xOff = 0;
         int yOff = 0;
         if(r > xm)
            xOff = pimage->width() * (r - xm) / d;

         if(r > ym)
            yOff = pimage->height() * (r - ym) / d;

         int wWindow = minimum(cx, d);
         int hWindow = minimum(cy, d);

//                  double xrate = (double) wWindow / d;
//                  double yrate = (double) hWindow / d;

         ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
         ::image::image_pointer pimage2 = pgcom->get_image(e_image_temp2);
         //::image::image_pointer pimage3 = pgcom->get_image(e_image_temp3);

         pimage1->create({wWindow, hWindow});
         pimage2->create({wWindow, hWindow});
         //image3.create({wWindow, hWindow});

         pimage1->get_graphics()->set_alpha_mode(::draw2d::e_alpha_mode_set);

         pimage1->get_graphics()->set_interpolation_mode(::draw2d::e_interpolation_mode_high_quality_bicubic);

         {

            auto rectangleSource = double_rectangle_dimension(xOff, yOff,
                                    pimage->width() - xOff * 2,
                                    pimage->height() - yOff * 2);

            ::image::image_source imagesource(pimage, rectangleSource);

            double_rectangle rectangleTarget(::int_size(wWindow, hWindow));

            ::image::image_drawing_options imagedrawingoptions(rectangleTarget);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

            pimage1->get_graphics()->draw(imagedrawing);

         }

         pimage2->get_graphics()->set_alpha_mode(::draw2d::e_alpha_mode_set);

         {

            ::image::image_source imagesource(dcBuffer, double_rectangle_dimension(x1, y1, wWindow, hWindow));

            auto rectangle = double_rectangle_dimension(0, 0, wWindow, hWindow);

            ::image::image_drawing_options imagedrawingoptions(rectangle);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

            pimage2->get_graphics()->draw(imagedrawing);

         }

         pimage2->fill_channel(255, ::color::e_channel_opacity);

         pimage2->channel_from(::color::e_channel_opacity,  pimage1);

         pimage2->mult_alpha();

         /*drawpimage->draw(
            pimage1->get_graphics(),
            0, 0,
            wWindow, hWindow,
            imageT2,
            xOff, yOff,
            pimage->width() - 2 * xOff -1,
            pimage->width() - 2 * yOff -1, 0);*/


         /*dc2.BitBlt(0, 0, wWindow, hWindow, pimage1->get_graphics(), 0, 0);
         dc2.BitBlt(0, 0, wWindow, hWindow, nullptr, 0, 0, DSTINVERT);

         dc3.fill_rectangle(0, 0, wWindow, hWindow, 0xff000000);
         dc2.BitBlt(0, 0, wWindow, hWindow, &dc3, 0, 0, SRCAND);*/

         /*StretchDIBits(
            dc2,
            0, 0,
            wWindow, hWindow,
            xOff, yOff,
            pimage->width() - 2 * xOff,
            pimage->height() - 2 * yOff,
            pimage->m_pcolorref,
            &pimage->m_Info,
            DIB_RGB_COLORS,
            SRCCOPY);*/

//                  ::image::image_pointer pimageBuffer = pgcom->get_image(100);
         /*StretchDIBits(
            pimage1->get_graphics(),
            0, 0,
            wWindow, hWindow,
            x1, y1,
            wWindow, hWindow,
            imageBuffer.m_pcolorref,
            &imageBuffer.m_Info,
            DIB_RGB_COLORS,
            SRCPAINT);*/



         //dcBack->BitBlt(x1, y1, wWindow, hWindow, &dcFrame1, x1, y1);
         //dcBack->BitBlt(x1, y1, wWindow, hWindow, pimage1->get_graphics(),
         // x1, y1);

         //dcBack->BitBlt(x1, y1, wWindow, hWindow, pimage1->get_graphics(),
         // x1, y1);

         dcBack->set_alpha_mode(::draw2d::e_alpha_mode_set);

         {

            ::image::image_source imagesource(pimage2);

            auto rectangle = double_rectangle_dimension(x1, y1, wWindow, hWindow);

            ::image::image_drawing_options imagedrawingoptions(rectangle);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

            dcBack->draw(imagedrawing);

         }


         /*drawpimage->draw(
            dcBack,
            x1, y1,
            d, d,
            pimage1,
            0, 0,
            d, d,
            DDF_HALFTONE);*/
         //dcBack->BitBlt(xm - r, ym - r, 2 * r, 2 * r, pimage1->get_graphics(), 0, 0);
         rectangleUpdate.set(x1, y1, x2, y2);
         recta.add(rectangleUpdate);
      }
      break;
      default:
         break;

      }

      synchronous_lock synchronouslock(pgcom->m_pmutex4Transfer, DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);

      ::draw2d::graphics_pointer dcTransfer = pgcom->GetTransferDC();

      //dcBack->set_origin(0, 0);

      //dcTransfer->set_origin(0, 0);

      for(int i = 0; i < recta.get_size(); i++)
      {

         ::int_rectangle rectangleTransfer = recta[i];

         //dcTransfer->draw(rectangleTransfer, dcBack, rectangleTransfer.top_left());

         ::image::image_source imagesource(dcBack, rectangleTransfer);

         ::image::image_drawing_options imagedrawingoptions(rectangleTransfer);

         imagedrawingoptions.m_bIntegerPlacement = true;

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         dcTransfer->draw(imagedrawing);

      }

   }

   int transition_effect::CalcRepeatCount()
   {
      int iStepRepeatCount = m_ptool001->m_iStepRepeatCount;
      switch(get_type())
      {
      case TransitionEffectpixelate_BottomTop:
      case TransitionEffectpixelate_TopBottom:
      case TransitionEffectpixelate_LeftRight:
      case TransitionEffectpixelate_RightLeft:
      {
         const int c1 = m_ptool001->m_data.m_alphapixelate.m_c1;
//               const int c2 = m_ptool001->m_data.m_alphapixelate.m_c2;

         ::collection::index iIndex = m_ptool001->m_iStep - 1;
         int iSizeIndex = m_ptool001->m_pointa[iIndex].x();
         int iSize = m_ptool001->m_ia[iSizeIndex];

         if(iSize < 64)
            iSize = 64;

         ////iStepRepeatCount = 1c1 / (iSize * 8);
         //iStepRepeatCount = 1;
//               int dwTotalTime = 1000;
         int iStepCount = c1 / iSize;
         if(c1 % iSize == 0)
            iStepCount++;
         int iStartStep = m_ptool001->m_pointa2[iIndex].y();
         int iStep = (int) (iIndex - iStartStep);
         iStepRepeatCount = iStepCount / 8;
         if(iStep > iStepCount - iStepCount / 4)
            iStepRepeatCount = iStepCount - iStep - 1;
      }
      break;
      default:
         break;

      }
      iStepRepeatCount = maximum(1, iStepRepeatCount);
      return iStepRepeatCount;

   }


   bool transition_effect::IsActive()
   {

      return m_ptool001->m_iStep > 0;

   }


   void transition_effect::one_second_timer()
   {

      if (m_ptaskRunStep.is_null())
      {

         //auto pmain = helper_get_main();

         m_ptaskRunStep = fork([&]()
         {

            try
            {

               _thread_run_step();

            }
            catch (...)
            {

            }

            m_ptaskRunStep.release();

         });

      }

   }

#ifdef _DEBUG


   long long transition_effect::increment_reference_count()
   {

      return ::matter::increment_reference_count();

   }


   long long transition_effect::decrement_reference_count()
   {

      return ::matter::decrement_reference_count();

   }


   long long transition_effect::release()
   {

      return ::matter::release();

   }


#endif

   void transition_effect::OnTimer()
   {
      return;

//      main * pmain = helper_get_main();
//      auto ::time = ::time::now();
//      if(::time - m_timeTransitionStepLastRunTime >= 5_tick + m_timeDelay
//            && !m_bTransitionStepRunActive
//            && (!m_bTransitionStepPostActive
//                || ::time - m_timeTransitionStepPostActive >= 2000_tick))
//      {
//         /*            if(::time - m_timeTransitionStepLastRunTime >= 200)
//         {
//               informationf("::time - m_timeTransitionStepLastRunTime >= 200 %d\n",  ::time - m_timeTransitionStepLastRunTime);
//               }
//               if(!m_bTransitionStepRunActive)
//               {
//               informationf("!m_bTransitionStepRunActive");
//               }
//               if(!m_bTransitionStepPostActive)
//               {
//               informationf("!m_bTransitionStepPostActive");
//               }
//               if(::time - m_timeTransitionStepPostActive >= 2000)
//               {
//               informationf("!m_bTransitionStepPostActive % d\n", ::time - m_timeTransitionStepPostActive);
//         }*/
//         m_bTransitionStepPostActive = true;
//         m_timeTransitionStepPostActive.Now();
//         pmain->get_user_interaction()->backimpact_post_message(::user::e_message_backimpact_deque, 18, 0);
//      }
//      else
//      {
//         ASSERT(true);
//      }

   }


   void transition_effect::_Init()
   {

      ::draw2d::lock draw2dlock(this);

      synchronous_lock slUserMutex(gcom_pmutex(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);

      main * pmain = helper_get_main();

      gcom * pgcom = pmain->get_gcom();

      single_lock sl1Back(pgcom->m_pmutex1Back, false);

      ::draw2d::graphics_pointer dcFrame1 = pgcom->GetFrame1DC();

      ::draw2d::graphics_pointer dcTransfer = pgcom->GetTransferDC();

      user_interaction * puserinteraction = pmain->get_user_interaction();

      ::int_rectangle rectangleX;

      puserinteraction->backimpact_get_client_rect(rectangleX);

      int cx = rectangleX.width();

      int cy = rectangleX.height();

      {

         synchronous_lock slTransfer(pgcom->m_pmutex4Transfer, DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);

         if (dcTransfer.is_set() && dcTransfer->get_os_data() != nullptr)
         {

            ::image::image_source imagesource(dcTransfer);

            double_rectangle rectangle(rectangleX.size());

            ::image::image_drawing_options imagedrawingoptions(rectangle);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

            dcFrame1->draw(imagedrawing);

         }

      }

      m_ptool001->m_iStep = 1;

      m_ptool001->m_iStepCount = -1;

      m_timeDelay = 200_ms;

      if (m_iInit == 0)
      {
         
         m_iType = 0;

         m_iVisual = 0;

         m_iInit = 1;

      }
      else if(m_iInit == 1)
      {

         m_iInit = 2;

         ::database::client * pdataclient = dynamic_cast < ::database::client * > (pmain->get_user_interaction()->backimpact_get_user_interaction());

         if (pdataclient != nullptr)
         {

            pdataclient->datastream()->get("gcom::backimpact::transition_effect::m_iType", m_iType);

            pdataclient->datastream()->get("gcom::backimpact::transition_effect::m_iVisual", m_iVisual);

         }

         if (m_iType < 0 || m_iVisual < 0)
         {

            m_iType = 0;

            m_iVisual = 0;

         }

         if(m_iType >= m_etypea.get_size())
         {

            m_iType = 0;

         }

         if(m_iVisual >= m_iaVisual.get_size())
         {

            m_iVisual = 0;

         }

      }
      else
      {

         ::pointer<::database::client>pdataclient = pmain->get_user_interaction()->backimpact_get_user_interaction();

         if (pdataclient.is_set())
         {

            pdataclient->datastream()->set("gcom::backimpact::transition_effect::m_iType", m_iType);

            pdataclient->datastream()->set("gcom::backimpact::transition_effect::m_iVisual", m_iVisual);

         }

      }

      m_ptool001->Initialize(get_type(), cx, cy, *this);

   }


   void transition_effect::_thread_run_step()
   {

      srand((unsigned int) time(nullptr));

      //unsigned int uRet = 0;

      while (m_bRun && ::task_get_run())
      {

         while (!m_bDestroy && !m_bActive && m_bRun && ::task_get_run())
         {

            preempt(1_s);

         }

         if (m_bDestroy || !m_bRun || !::task_get_run())
         {

            break;

         }

         _Init();

         if (m_bDestroy || !m_bRun || !::task_get_run())
         {

            break;

         }

         while (!TestEnd() && m_bRun && ::task_get_run() && !m_bDestroy)
         {

            preempt(5_ms);

            RunStepProcLevel2();

         }

         if (m_bDestroy || !m_bRun || !::task_get_run())
         {

            break;

         }

         _Final();

         StepNext();

      }

   }


   ETransitionEffect transition_effect::get_type()
   {

      if (m_iType < 0 || m_iType >= m_etypea.get_size())
      {

         return TransitionEffectInvalid;

      }

      return m_etypea[m_iType];

   }


   ::e_align transition_effect::GetAlign(ETransitionEffect eeffect)
   {

      switch(eeffect)
      {
      case TransitionEffectFlyInBottomTop:
         return ::e_align_top;
      case TransitionEffectFlyInTopBottom:
         return ::e_align_bottom;
      case TransitionEffectFlyInLeftRight:
         return ::e_align_right;
      case TransitionEffectFlyInRightLeft:
         return ::e_align_left;
      case TransitionEffectFlyInLeftTop:
         return ::e_align_top_right;
      case TransitionEffectFlyInLeftBottom:
         return ::e_align_bottom_right;
      case TransitionEffectFlyInRightBottom:
         return ::e_align_bottom_left;
      case TransitionEffectFlyInRightTop:
         return ::e_align_top_left;
      default:
         return ::e_align_none;
      }

   }


   ::gcom::e_direction transition_effect::GetDirection(ETransitionEffect eeffect)
   {
      switch(eeffect)
      {
      case TransitionEffectFlyInBottomTop:
         return ::gcom::direction_top;
      case TransitionEffectFlyInTopBottom:
         return ::gcom::direction_bottom;
      case TransitionEffectFlyInLeftRight:
         return ::gcom::direction_right;
      case TransitionEffectFlyInRightLeft:
         return ::gcom::direction_left;
      case TransitionEffectFlyInLeftTop:
         return ::gcom::direction_top_right;
      case TransitionEffectFlyInLeftBottom:
         return ::gcom::direction_bottom_right;
      case TransitionEffectFlyInRightBottom:
         return ::gcom::direction_bottom_left;
      case TransitionEffectFlyInRightTop:
         return ::gcom::direction_top_left;
      case TransitionEffectWipeTop:
         return ::gcom::direction_top;
      case TransitionEffectWipeBottom:
         return ::gcom::direction_bottom;
      case TransitionEffectWipeRight:
         return ::gcom::direction_right;
      case TransitionEffectWipeLeft:
         return ::gcom::direction_left;
      case TransitionEffectWipeTopLeft:
         return ::gcom::direction_top_left;
      case TransitionEffectWipeTopRight:
         return ::gcom::direction_top_right;
      case TransitionEffectWipeBottomLeft:
         return ::gcom::direction_bottom_left;
      case TransitionEffectWipeBottomRight:
         return ::gcom::direction_bottom_right;
      case TransitionEffectScaleTop:
         return ::gcom::direction_top;
      case TransitionEffectScaleBottom:
         return ::gcom::direction_bottom;
      case TransitionEffectScaleRight:
         return ::gcom::direction_right;
      case TransitionEffectScaleLeft:
         return ::gcom::direction_left;
      case TransitionEffectScaleTopLeft:
         return ::gcom::direction_top_left;
      case TransitionEffectScaleTopRight:
         return ::gcom::direction_top_right;
      case TransitionEffectScaleBottomLeft:
         return ::gcom::direction_bottom_left;
      case TransitionEffectScaleBottomRight:
         return ::gcom::direction_bottom_right;
      default:
         return ::gcom::direction_none;
      }

   }

} // namespace backimpact








