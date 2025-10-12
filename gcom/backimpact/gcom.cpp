#include "framework.h"
#include "user_interaction.h"
#include "gcom.h"
#include "image_change.h"
#include "acme/parallelization/mutex.h"
#include "acme/parallelization/synchronous_lock.h"
#include "aura/graphics/image/image.h"
#include "aura/graphics/image/drawing.h"
#include "aura/windowing/windowing.h"
#include "aura/windowing/display.h"
#include "aura/user/user/user.h"
#include "aura/platform/session.h"


namespace backimpact
{


   gcom::gcom()
   {

   }


   gcom::~gcom()
   {

   }


   void gcom::initialize_backimpact_helper(main * pmain)
   {

      //auto estatus = 
      
      helper::initialize_backimpact_helper(pmain);

      //if (!estatus)
      //{

      //   return estatus;

      //}

      øconstruct(m_dcScreen);

      //return estatus;

   }


   ::draw2d::graphics_pointer gcom::GetScreenDC()
   {

      return m_dcScreen;

   }


   ::draw2d::graphics_pointer gcom::GetBackDC()
   {

      return get_image(e_image_back)->g();

   }


   ::draw2d::graphics_pointer gcom::GetTransferDC()
   {

      get_image(e_image_transfer)->dc_select();

      return get_image(e_image_transfer)->get_graphics();

   }


   ::draw2d::graphics_pointer gcom::GetFrame1DC()
   {

      return get_image(e_image_frame1)->g();

   }


   ::draw2d::graphics_pointer gcom::GetBufferDC()
   {

      return get_image(e_image_buffer)->g();

   }


   ::draw2d::graphics_pointer gcom::GetSourceDC()
   {

      return get_image(e_image_source)->g();

   }


   ::draw2d::bitmap_pointer gcom::GetBackBitmap()
   {

      return get_image(e_image_back)->g();

   }


   ::draw2d::bitmap_pointer gcom::GetTransferBitmap()
   {

      return get_image(e_image_transfer)->get_bitmap();

   }


   ::draw2d::bitmap_pointer gcom::GetFrame1Bitmap()
   {

      return get_image(e_image_frame1)->get_bitmap();

   }


   ::draw2d::bitmap_pointer gcom::GetBufferBitmap()
   {

      return get_image(e_image_buffer)->get_bitmap();

   }


   ::draw2d::bitmap_pointer gcom::GetSourceBitmap()
   {

      return get_image(e_image_source)->get_bitmap();

   }


//#ifdef WINDOWS_DESKTOP
//
//
//   void gcom::LayoutBackBitmap(BITMAP * lpbmBack)
//   {
//
//      __UNREFERENCED_PARAMETER(lpbmBack);
//
//      ::int_rectangle rectangleX;
//
//      auto pmain = helper_get_main();
//
//      if (!pmain)
//      {
//
//         return;
//
//      }
//
//      pmain->get_user_interaction()->backimpact_get_client_rect(rectangleX);
//
//      int cx = rectangleX.width();
//
//      int cy = rectangleX.height();
//
//      single_lock sl1Back(m_pmutex1Back, true);
//
//      if (!get_image(e_image_back)->create({ cx, cy }))
//      {
//
//         return;
//
//      }
//
//      ::draw2d::graphics_pointer dcBack = GetBackDC();
//
//      dcBack->fill_rectangle(::double_rectangle_dimension(0, 0, cx, cy), argb(0, 0, 0, 0));
//
//      sl1Back.unlock();
//
//   }
//
//
//#endif


   void gcom::on_image_loaded(::image::image *pimage)
   {

      single_lock sl3Source(m_pmutex3Source, true);

      main * pmain = helper_get_main();

      //         ::aura::savings & savings = session()->savings();

      bool bOk = false;
      // 2004-08-24
      if(pimage->is_set())
      {

         get_image(e_image_source)->copy_from(pimage);

         bOk = true;

      }

      if(bOk)
         pmain->image_change_post_event(e_event_loaded);
      else
         pmain->image_change_post_event(e_event_load_failed);

   }

   
   void gcom::on_destroy()
   {

      single_lock sl1Back(m_pmutex1Back, true);
      single_lock sl2Buffer(m_pmutex2Buffer, true);
      single_lock sl3Source(m_pmutex3Source, true);

      //return ::success;

   }


   bool gcom::ApplyImagePlacementSettings()
   {

      synchronous_lock slUserMutex(synchronization(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);

      main * pmain = helper_get_main();

      user_interaction * puserinteraction = pmain->get_user_interaction();

      ::int_rectangle rectangleX;

      puserinteraction->backimpact_get_client_rect(rectangleX);

      ::int_rectangle rectangleScreen = rectangleX;

      puserinteraction->backimpact_client_to_screen()(rectangleScreen);

      int cx = rectangleX.width();

      int cy = rectangleX.height();

      single_lock sl2Buffer(m_pmutex2Buffer, true);

      single_lock sl3Source(m_pmutex3Source, true);

      ::draw2d::graphics_pointer dcBuffer = GetBufferDC();

      ::draw2d::graphics_pointer dcSource = GetSourceDC();

      if (dcSource.is_null())
      {

         return false;

      }

      if (dcSource->get_os_data() == nullptr)
      {

         return false;

      }

      if (dcBuffer.is_null())
      {

         return false;

      }

      if (dcBuffer->get_os_data() == nullptr)
      {

         return false;

      }

      try
      {

         ::draw2d::bitmap_pointer bmpSource = GetSourceBitmap();

         if(bmpSource->m_osdata[0] != nullptr && get_image(e_image_source)->area() > 0)
         {

            if(pmain->is_full_screen())
            {

               dcBuffer->fill_rectangle(rectangleX, get_image(e_image_source)->GetAverageColor());

            }
            else
            {
               
               //dcBuffer->fill_rectangle(rectangleX, get_image(e_image_source)->GetAverageColor());

            }

            ::int_size sizeSource = bmpSource->get_size();


            int finalX = 0;
            int finalY = 0;
            int finalW = 0;
            int finalH = 0;

            image_change * pimagechange = pmain->get_image_change();

            double dRate;
            if(pimagechange->m_eplacement == ImagePlacementZoomAll)
            {
               dRate = minimum((double) cx / sizeSource.cx(), (double) cy / sizeSource.cy());
               finalW = (int) (sizeSource.cx() * dRate);
               finalH = (int)(sizeSource.cy() * dRate);
               finalX = (cx - finalW) / 2;
               finalY = (cy - finalH) / 2;
            }
            else if (pimagechange->m_eplacement == ImagePlacementZoomExtend)
            {
               dRate = maximum((double) cx / sizeSource.cx(), (double) cy / sizeSource.cy());
               finalW = (int) (sizeSource.cx() * dRate);
               finalH = (int) (sizeSource.cy() * dRate);
               finalX = (cx - finalW) / 2;
               finalY = (cy - finalH) / 2;
            }
            else if (pimagechange->m_eplacement == ImagePlacementStretch)
            {
               finalW = (int) cx;
               finalH = (int) cy;
               finalX = 0;
               finalY = 0;
            }


            if(pimagechange->m_eplacement == ImagePlacementTile)
            {

               int iW = sizeSource.cx();
               int iH = sizeSource.cy();
               int iXMod = (cx - iW) / 2;
               int iYMod = (cy - iH) / 2;
               int iXOffset =  iXMod;
               int iYOffset =  iYMod;
               while(iXOffset > 0 && iW > 0)
               {
                  iXOffset -= iW;
               }
               while (iYOffset > 0 && iH > 0)
               {
                  iYOffset -= iH;
               }
               int iCount = (iXMod < 0) ? 1 : cx / (iW <= 0 ? 1 : iW) + (iXOffset == 0 ? 0 : 2);
               int jCount = (iYMod < 0) ? 1 : cy / (iH <= 0 ? 1 : iH) + (iYOffset == 0 ? 0 : 2);
               for(int i = 0; i < iCount; i++)
               {
                  
                  int iX = iXOffset + iW * i;

                  for(int j = 0; j < jCount; j++)
                  {
                     
                     int iY = iYOffset + iH * j;

                     ::image::image_source imagesource(dcBuffer);

                     auto rectangle = float_rectangle_dimension(iX, iY, iW, iH);

                     ::image::image_drawing_options imagedrawingoptions(rectangle);

                     ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);
                     
                     dcBuffer->draw(imagedrawing);

                  }

               }

               m_rectangleFinalPlacement.left()     = iXOffset;
               m_rectangleFinalPlacement.top()      = iYOffset;
               m_rectangleFinalPlacement.right()    = iXOffset + iW * iCount;
               m_rectangleFinalPlacement.bottom()   = iXOffset + iW * jCount;

            }
            else if(pimagechange->m_eplacement == ImagePlacementZoomAll ||
                    pimagechange->m_eplacement == ImagePlacementZoomExtend ||
                    pimagechange->m_eplacement == ImagePlacementStretch)
            {

               int srcX = 0;
               int srcY = 0;
               int srcW = sizeSource.cx();
               int srcH = sizeSource.cy();

               dcBuffer->set_interpolation_mode(::draw2d::e_interpolation_mode_high_quality_bicubic);


               {

                  auto rectangleSource = double_rectangle_dimension(srcX, srcY, srcW, srcH);

                  ::image::image_source imagesource(dcSource, rectangleSource);

                  auto rectangleTarget = double_rectangle_dimension(finalX, finalY, finalW, finalH);

                  ::image::image_drawing_options imagedrawingoptions(rectangleTarget);

                  ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

                  dcBuffer->draw(imagedrawing);

               }

               m_rectangleFinalPlacement.left()     = finalX;
               m_rectangleFinalPlacement.top()      = finalY;
               m_rectangleFinalPlacement.right()    = finalX + finalW;
               m_rectangleFinalPlacement.bottom()   = finalY + finalH;

            }

            get_image(e_image_final)->create(m_rectangleFinalPlacement.size());

            get_image(e_image_final)->g()->set_interpolation_mode(::draw2d::e_interpolation_mode_high_quality_bicubic);

            {

               ::image::image_source imagesource(get_image(e_image_buffer), m_rectangleFinalPlacement);

               ::image::image_drawing_options imagedrawingoptions(m_rectangleFinalPlacement.size());

               ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

               get_image(e_image_final)->g()->draw(imagedrawing);

            }

         }

      }
      catch(int)
      {
      }
      
      return true;

   }


   void gcom::update_objects()
   {

//      synchronous_lock slUserMutex(&gcom_pmutex());

      ::int_rectangle rectangleX;
      user_interaction * puserinteraction = helper_get_main()->get_user_interaction();
      puserinteraction->backimpact_get_client_rect(rectangleX);
      int cx = rectangleX.width();
      int cy = rectangleX.height();

      single_lock sl1Back(m_pmutex1Back, true);
      single_lock sl2Buffer(m_pmutex2Buffer, true);
      single_lock sl3Source(m_pmutex3Source, true);


      ::draw2d::graphics_pointer spgraphicsScreen    = GetScreenDC();

      if(spgraphicsScreen->get_os_data() != nullptr)
      {

         spgraphicsScreen->DeleteDC();

      }

      spgraphicsScreen->CreateCompatibleDC(nullptr);

      get_image(e_image_back)->create({ cx, cy }); // Back
      get_image(e_image_back)->clear(::color::transparent);

      get_image(e_image_buffer)->create({ cx, cy }); // buffer
      get_image(e_image_buffer)->clear(::color::transparent);

      get_image(e_image_transfer)->create({ cx, cy }); // Transfer
      get_image(e_image_transfer)->clear(::color::transparent);

      get_image(e_image_frame1)->create({ cx, cy }); // Frame1
      get_image(e_image_frame1)->clear(::color::transparent);

   }


   enum_image_placement gcom::GetDefaultPlacement()
   {

      enum_image_placement eplacement = ImagePlacementZoomAll;

#ifdef WINDOWS_DESKTOP

      ::draw2d::bitmap_pointer bmpSource = GetSourceBitmap();

      if (bmpSource)
      {

         ::int_size size = bmpSource->get_size();

         auto psession = session();

         auto paurasession = psession;

         auto puser = paurasession->user();

         auto pwindowing = system()->windowing();

         auto pdisplay = pwindowing->display();

         auto sizeMonitorHalf = pdisplay->get_main_monitor_size() / 2;

         if (size.cx() < sizeMonitorHalf.cx() && size.cy() < sizeMonitorHalf.cy())
         {

            eplacement = ImagePlacementTile;

         }

      }

#endif

      return eplacement;

   }


   ::image::image_pointer gcom::get_image(enum_image eimage)
   {

      ::image::image_pointer & pimage = m_mapImage[eimage];

      if(pimage->is_set())
      {

         return pimage;

      }

      øconstruct(pimage);

      on_create_image(pimage,  eimage);

      return pimage;

   }


   void gcom::buffer_to_back()
   {

      single_lock sl1Back(m_pmutex1Back, false);

      single_lock sl2Buffer(m_pmutex2Buffer, false);

      if (!sl1Back.lock(25_ms))
      {

         return;

      }

      if (!sl2Buffer.lock(25_ms))
      {

         return;

      }

      if (!get_image(e_image_back))
      {

         return;

      }

      if (!get_image(e_image_buffer))
      {

         return;

      }

      get_image(e_image_back)->copy_from(get_image(e_image_buffer));

   }


   void gcom::back_to_transfer()
   {

      single_lock sl1Back(m_pmutex1Back, false);

      if(!sl1Back.lock(25_ms))
         return;

      get_image(e_image_transfer)->copy_from(get_image(e_image_back));

   }


   void gcom::on_create_image(::image::image *pimage,  enum_image eimage)
   {

      switch(eimage)
      {
      case e_image_radial_center_alpha:
      {
         // Radial Fill (Center Full alpha, Radius = 256 pixels)
         pimage->create({ 512, 512 });
         pimage->RadialFill(255, 0, 0, 0,
                          0, 0, 0, 0,
                          256, 256, 256);
      }
      break;
      case e_image_radial_border_alpha:
      {
         // Radial Fill (Border Full alpha, Radius = 256 pixels)
         pimage->create({ 511, 511 });
         pimage->RadialFill(0, 0, 0, 0,
                          255, 0, 0, 0, 255, 255, 256);
      }
      break;
      default:
         break;
      }
   }
   void gcom::GetFinalPlacement(::int_rectangle * lprect)
   {
      *lprect = m_rectangleFinalPlacement;

   }


} // namespace backimpact




