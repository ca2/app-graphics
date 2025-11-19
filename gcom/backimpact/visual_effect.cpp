#include "framework.h"
#include "user_interaction.h"
#include "gcom.h"
#include "acme/graphics/draw2d/color.h"
#include "acme/parallelization/mutex.h"
#include "acme/parallelization/synchronous_lock.h"
#include "acme/platform/keep.h"
#include "aura/graphics/image/image.h"
#include "aura/graphics/image/drawing.h"
#include "aura/graphics/draw2d/brush.h"
#include "aura/graphics/draw2d/pen.h"
#include "transition_effect.h"
#include "visual_effect.h"


#include <math.h>
#include <time.h>
#include <stdio.h>


#ifndef APPLEOS
#define M_PI 3.14159265358979323846
#endif
#define RAND_MAX2 65535
int rand2()
{
   return (int) ((long long) rand() * (long long) RAND_MAX2 / (long long) RAND_MAX);
}

#define rand rand2

#define VISUAL_EFFECT_STRETCH_MODE ::draw2d::e_interpolation_mode_nearest_neighbor


#undef RAND_MAX
#define RAND_MAX RAND_MAX2
#define DRAND_MAX ((double)RAND_MAX)


namespace backimpact
{

   class resizer :
      virtual public ::object
   {
   public:
      
      
#ifdef USE_IPP

      IppiResizeSpec_32f* pSpec = 0;
      int specSize = 0, initSize = 0, bufSize = 0;
      Ipp8u* pBuffer = 0;
      Ipp8u* pInitBuf = 0;
      Ipp32u numChannels = 3;
      IppiPoint dstOffset = { 0, 0 };
      IppStatus status = ippStsNoErr;
      IppiBorderType border = ippBorderRepl;
      IppiSize srcSize = { 0, 0 };
      IppiSize dstSize = { 0,0 };

#endif

      resizer()
      {

      }
      ~resizer()
      {
         destroy();
      }
      
      
#ifdef USE_IPP

      bool set_sizes(::int_size sSrc, ::int_size sDst)
      {

         if (_set_sizes(sSrc, sDst) != ippStsNoErr)
         {
            return false;
         }
         return true;
      }

      IppStatus _set_sizes(::int_size sSrc, ::int_size sDst)
      {
         if(srcSize.width == sSrc.cx()
               && srcSize.height == sSrc.cy()
               && dstSize.width == sDst.cx()
               && dstSize.height == sDst.cy()
           )
            return ippStsNoErr;
         destroy();
         srcSize.width = sSrc.cx();
         srcSize.height = sSrc.cy();
         dstSize.width = sDst.cx();
         dstSize.height = sDst.cy();
//         Ipp8u* pSrc,, Ipp32s srcStep, Ipp8u* pDst, IppiSize dstSize, Ipp32s dstStep
         /* Spec and init buffer sizes */
         status = ippiResizeGetSize_8u(srcSize, dstSize, ippLinear, 0, &specSize, &initSize);

         if (status != ippStsNoErr) return status;

         /* Memory allocation */
         pInitBuf = ippsMalloc_8u(initSize);
         pSpec = (IppiResizeSpec_32f*)ippsMalloc_8u(specSize);

         if (pInitBuf == NULL || pSpec == NULL)
         {
            ippsFree(pInitBuf);
            ippsFree(pSpec);
            return ippStsNoMemErr;
         }

         /* Filter initialization */
         status = ippiResizeLinearInit_8u(srcSize, dstSize, pSpec);
         ippsFree(pInitBuf);

         if (status != ippStsNoErr)
         {
            ippsFree(pSpec);
            return status;
         }

         /* work buffer int_size */
         status = ippiResizeGetBufferSize_8u(pSpec, dstSize, numChannels, &bufSize);
         if (status != ippStsNoErr)
         {
            ippsFree(pSpec);
            return status;
         }

         pBuffer = ippsMalloc_8u(bufSize);
         if (pBuffer == NULL)
         {
            ippsFree(pSpec);
            return ippStsNoMemErr;
         }

         return ippStsNoErr;

      }
      
      #endif

      void destroy()
      {
#ifdef USE_IPP
         if (pSpec)
         {
            ippsFree(pSpec);
         }
         if (pBuffer)
         {
            ippsFree(pBuffer);
         }
#endif

         //return ::success;

      }
      


      bool do_resize(::image::image_pointer & pimageTarget, ::image::image_pointer & pimageSource)
      {

#ifdef USE_IPP
         if (!set_sizes(pimageSource->get_size(), pimageTarget->get_size()))
         {

            return false;

         }
         
         pimageSource->map_base();
         
         pimageTarget->map_base();

         /* Resize processing */
         IppStatus status = ippiResizeLinear_8u_C4R(
            pimageTarget->get_data(), pimageSource->m_pimpl->m_iScan, pimageTarget->get_data(), pimageTarget->m_pimpl->m_iScan, dstOffset, dstSize, border, 0, pSpec, pBuffer);

         return status == ippStsNoErr;
         
#else
         
         pimageTarget->g()->set_interpolation_mode(::draw2d::e_interpolation_mode_nearest_neighbor);

         {

            ::image::image_source imagesource(pimageSource);

            double_rectangle rectangle(pimageTarget->size());

            ::image::image_drawing_options imagedrawingoptions(rectangle);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);
      
            //return 
            pimageTarget->draw(imagedrawing);

            return true;

         }
         
#endif
         
         
         
      }

   };

   visual_effect::visual_effect()
   {


   }


   visual_effect::~visual_effect()
   {

   }


   void visual_effect::initialize_backimpact_helper(main * pmain)
   {

      //auto estatus = 
      
      helper::initialize_backimpact_helper(pmain);

      //if (!estatus)
      //{

      //   return estatus;

      //}


#if USE_IPP

      will_use_ipp();

#endif

      //estatus = 
      
      øconstruct_new(m_presizer);

      //if (!estatus)
      //{

      //   return estatus;

      //}


      //return estatus;

   }




   void visual_effect::on_render_buffer(int_rectangle_array & recta)
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

      //transition_effect * ptransitioneffect = pmain->get_transition_effect();

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {

         srand((unsigned int) time(nullptr));

         InitialRender();

      }

      auto puserinteraction = pmain->get_user_interaction();

      puserinteraction->backimpact_get_client_rect(m_tool1.m_rectangleX);

      m_tool1.cx() = m_tool1.m_rectangleX.width();

      m_tool1.cy() = m_tool1.m_rectangleX.height();

      gcom * pgcom = pmain->get_gcom();

      if (pgcom->GetBufferDC()== nullptr || pgcom->GetBufferDC()->get_os_data() == nullptr)
         return;

      single_lock sl1Back(pgcom->m_pmutex1Back, false);

      single_lock sl2Buffer(pgcom->m_pmutex2Buffer, false);

      ::draw2d::graphics_pointer dcBack = pgcom->GetBackDC();

      ::draw2d::graphics_pointer dcBuffer = pgcom->GetBufferDC();

      pmain->defer_check_layout();

      sl1Back.lock();

      sl2Buffer.lock();

      ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);

      if(!pimage1->is_ok())
      {

         return;

      }

      ::draw2d::save_context k1(dcBack);

      ::draw2d::save_context k2(dcBuffer);

      //dcBack->SelectClipRgn(nullptr);

      //dcBuffer->SelectClipRgn(nullptr);

      //m_iVisual = VisualEffectRotateEx5;
      m_iVisual = ptransitioneffect->m_iaVisual[ptransitioneffect->m_iVisual];
      //      m_iVisual = VisualEffectRotateEx5;
      switch(m_iVisual)
      {
      case VisualEffectPixelExplosion:
         RenderPixelExplosion(recta);
         break;
      case VisualEffectAlphaPixelExplosion:
         RenderAlphaPixelExplosion(recta);
         break;
      case VisualEffectPixelExplosion2:
         RenderPixelExplosion2(recta);
         break;
      case VisualEffectRotateEx8:
         RenderRotateEx8(recta);
         break;
      case VisualEffectRotateEx4:
         RenderRotateEx4(recta);
         break;
      case VisualEffectRain1:
         RenderRain1(recta);
         break;
      case VisualEffectRotateBlend:
      case VisualEffectRotateEx5:
         RenderRotateEx5(recta);
         break;
      case VisualEffectRotateEx6:
         RenderRotateEx5(recta);
         break;
      case VisualEffectRotateEx7:
         RenderRotateEx7(recta);
         break;
         //RenderRotateBlend(recta);
         //RenderNPRotatefill_rectangle_(recta);
         //RenderNPRotateTrackfill_rectangle_(recta);
         break;
      case VisualEffectNoPrecisionRotateBlend:
         RenderNPRotateBlend(recta);
         //RenderNPRotateTrackfill_rectangle_(recta);
         break;
      case VisualEffectNoPrecisionRotateColorBlend:
         RenderNPRotateColorBlend(recta);
         break;
      case VisualEffectNoPrecisionRotateTrackColorBlend:
         RenderNPRotateTrackColorBlend(recta);
         break;
      case VisualEffectRotateEx1:
         RenderRotateEx1(recta);
         break;
      case VisualEffectRotateEx2:
         RenderRotateEx2(recta);
         break;
      case VisualEffectRotateEx3:
         RenderRotateEx3(recta);
         break;
      case VisualEffectExpand4:
         RenderExpand4(recta);
         break;
      case VisualEffectExpand5:
         RenderExpand5(recta);
         break;
      case VisualEffectExpand6:
         RenderExpand5(recta);
         break;
      case VisualEffectExpand7:
         RenderExpand7(recta);
         break;
      case VisualEffectExpand8:
         RenderExpand8(recta);
         break;
      default:
         ASSERT(false);
         break;
      }

      {

         synchronous_lock synchronouslock(pgcom->m_pmutex4Transfer, DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);

         ::image::image_pointer pimageBack = pgcom->get_image(e_image_back);

         ::image::image_pointer pimageTransfer = pgcom->get_image(e_image_transfer);

         //imageTransfer.stretch(pimageBack);

         ::image::image_source imagesource(pimageBack);

         double_rectangle rectangle(pimageTransfer->get_size());

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimageTransfer->g()->draw(imagedrawing);

      }

   }


   void visual_effect::RenderRotateBlend(int_rectangle_array & recta)
   {

      auto pmain = helper_get_main();

      if (!pmain)
      {

         return;

      }

      auto ptransitioneffect = pmain->get_transition_effect();

      ::int_rectangle rectangleX = m_tool1.m_rectangleX;

      gcom * pgcom = pmain->get_gcom();

      ASSERT(pgcom->GetBufferDC()->get_os_data() != nullptr);

      ::image::image_pointer pimageBack = pgcom->get_image(e_image_back);
      ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
      ::image::image_pointer pimage2 = pgcom->get_image(e_image_temp2);
      ::image::image_pointer pimage3 = pgcom->get_image(e_image_temp3);
      ::image::image_pointer pimageBuffer = pgcom->get_image(e_image_buffer);

      int cx1 = pimage1->width();
      int cy1 = pimage1->height();

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         pimage2->create({ cx1, cy1 });
         pimage3->create({ cx1, cy1 });
      }



#ifdef WINDOWS_DESKTOP

      //GdiFlush();

#endif

{

         ::image::image_source imagesource(pimageBuffer);

         double_rectangle rectangle(pimage3->size());

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimage3->draw(imagedrawing);

      }

      StepGrow001(
      m_color,
      m_iGrowColor,
      m_iGrowMax);

      double dAngleOffset = 1.0 * rand() / RAND_MAX;
      if(m_iGrowColor >= 0)
      {
         //m_iGrowColor++;
         m_dAngle += 8.3 + dAngleOffset;
         //if(m_iGrowColor >= 100)
         // m_iGrowColor = -1;
      }
      else
      {
         //m_iGrowColor--;
         m_dAngle -= 8.3 + dAngleOffset;
         //if(m_iGrowColor <= -100)
         // m_iGrowColor = 1;
      }
      pimage2->rotate(pimage3, radians(m_dAngle), 1.009);
         //pimage1->g()->fill_rectangle({ cxm - s1, cym - s2, s3, s4, m_color & ::opacity(bA));


         //// top
         //pimage1->g()->fill_rectangle({ 0, 0, cx1 - 1, 1, m_color & ::opacity(bA));
         //// left
         //pimage1->g()->fill_rectangle({ 0, 0, 1, cy1 - 1, m_color & ::opacity(bA));
         //// bottom
         //pimage1->g()->fill_rectangle({ 0, cy1 - 1, cx1 - 1, 1, m_color & ::opacity(bA));
         //// right
         //pimage1->g()->fill_rectangle({ cx1 - 2, 0, 1, cy1 - 1, m_color & ::opacity(bA));*/


      /*imaging.bitmap_blend(
         pimage1->get_graphics(),
         0, 0, cx1, cy1,
         pimage2->get_graphics(),
         0, 0,
         50);*/

      pimage2->channel_multiply(10.0 / 255.0, ::color::e_channel_opacity);

      {

         ::image::image_source imagesource(pimage2);

         auto rectangle = double_rectangle_dimension(0, 0, cx1, cy1);

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimage1->get_graphics()->draw(imagedrawing);

      }

      pimageBack->g()->set_interpolation_mode(VISUAL_EFFECT_STRETCH_MODE);

      {

         ::image::image_source imagesource(pimage1);

         double_rectangle rectangle(pimageBack->size());

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimageBack->draw(imagedrawing);

      }

      //dcBack->fill_rectangle(0, 0, cx, cy, argb(255, 0, 0, 0));
      //dcBack->fill_rectangle(100, 100, cx, cy, argb(255, 255, 255, 255));

      recta.add(rectangleX);

   }


   void visual_effect::RenderNPRotateBlend(int_rectangle_array & recta)
   {

      main * pmain = helper_get_main();

      transition_effect * ptransitioneffect = pmain->get_transition_effect();

      ::int_rectangle rectangleX = m_tool1.m_rectangleX;

      m_tool1.cx() = rectangleX.width();
      m_tool1.cy() = rectangleX.height();

      //auto psystem = m_papplication;

      //class imaging & imaging = psystem->imaging();

      gcom * pgcom = pmain->get_gcom();

      ASSERT(pgcom->GetBufferDC()->get_os_data() != nullptr);

      ::image::image_pointer pimageBack = pgcom->get_image(e_image_back);
      ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
      ::image::image_pointer pimage2 = pgcom->get_image(e_image_temp2);
      ::image::image_pointer pimage3 = pgcom->get_image(e_image_temp3);

      int cx1 = pimage1->width();
      int cy1 = pimage1->height();
      int cxm = cx1 / 2;
      int cym = cy1 / 2;

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         pimage2->create({cx1, cy1});
         pimage3->create({cx1, cy1});
      }


      int s1 = rand() * 1 / RAND_MAX + 1;
      int s2 = rand() * 1 / RAND_MAX + 1;
      int s3 = rand() * 2 / RAND_MAX + 1;
      int s4 = rand() * 2 / RAND_MAX + 1;

      StepGrow001(
      m_color,
      m_iGrowColor,
      m_iGrowMax);





      //rgb(255, rand() * 255 / RAND_MAX,
      //rand() * 255 / RAND_MAX,
      //rand() * 255 / RAND_MAX);

      unsigned char bA = 50;

#ifdef WINDOWS_DESKTOP
      //GdiFlush();
#endif
      //double dmin = minimum(cx, cy);
      //double dmax = maximum(cx, cy);abs


      // ::draw2d::graphics_pointer pimage3->get_graphics();
      //pimage3->get_graphics()->CreateCompatibleDC(nullptr);
      //pimage3->get_graphics()->set(pimage3->m_hbitmap);

      //   pimage3->stretch(imageBuffer);

      //   pimage3->Paste(pimage1);



      double dAngleOffset = (20.0 * rand() / RAND_MAX) - 5.0;
      double dAngle;
      if(m_iGrowColor >= 0)
      {
         dAngle = 10.111 + dAngleOffset;
      }
      else
      {
         dAngle = -10.111 - dAngleOffset;
      }
      pimage2->rotate(pimage1, radians(dAngle), 1.009);
      pimage1->g()->fill_rectangle({ cxm - s1, cym - s2, s3, s4 }, m_color & ::opacity(bA));


      // top
      pimage1->g()->fill_rectangle({ 0, 0, cx1 - 1, 1 }, m_color & ::opacity(bA));
      // left
      pimage1->g()->fill_rectangle({ 0, 0, 1, cy1 - 1 }, m_color & ::opacity(bA));
      // bottom
      pimage1->g()->fill_rectangle({ 0, cy1 - 1, cx1 - 1, 1 }, m_color & ::opacity(bA));
      // right
      pimage1->g()->fill_rectangle({ cx1 - 2, 0, 1, cy1 - 1 }, m_color & ::opacity(bA));


      /*imaging.bitmap_blend(
         pimage1->get_graphics(),
         0, 0, cx1, cy1,
         pimage2->get_graphics(),
         0, 0,
         50);*/

      /*BLENDFUNCTION bf;

      bf.BlendOp     = AC_SRC_OVER;
      bf.BlendFlags  = 0;
      bf.SourceConstantAlpha = 10;
      //   bf.AlphaFormat = AC_SRC_ALPHA;
      bf.AlphaFormat = 0;*/

      pimage2->channel_multiply(10.0 / 255.0, ::color::e_channel_opacity);

      {

         ::image::image_source imagesource(pimage2);

         double_rectangle rectangle(0, 0, cx1, cy1);

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimage1->get_graphics()->draw(imagedrawing);

      }

      pimageBack->g()->set_interpolation_mode(VISUAL_EFFECT_STRETCH_MODE);

      {

         ::image::image_source imagesource(pimage1);

         double_rectangle rectangle (pimageBack->size());

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimageBack->draw(imagedrawing);

      }

      recta.add(rectangleX);

   }


   void visual_effect::RenderNPRotateColorBlend(int_rectangle_array & recta)
   {
      
      main * pmain = helper_get_main();

      transition_effect * ptransitioneffect = pmain->get_transition_effect();

      ::int_rectangle rectangleX = m_tool1.m_rectangleX;
      m_tool1.cx() = rectangleX.width();
      m_tool1.cy() = rectangleX.height();

      //auto psystem = m_papplication;

      //class imaging & imaging = psystem->imaging();

      gcom * pgcom = pmain->get_gcom();

      ASSERT(pgcom->GetBufferDC()->get_os_data() != nullptr);

      ::image::image_pointer pimageBack = pgcom->get_image(e_image_back);
      ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
      ::image::image_pointer pimage2 = pgcom->get_image(e_image_temp2);
      ::image::image_pointer pimage3 = pgcom->get_image(e_image_temp3);

      int cx1 = pimage1->width();
      int cy1 = pimage1->height();
      int cxm = cx1 / 2;
      int cym = cy1 / 2;

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         pimage2->create({cx1, cy1});
         pimage3->create({cx1, cy1});
      }


      int s1 = rand() * 1 / RAND_MAX + 1;
      int s2 = rand() * 1 / RAND_MAX + 1;
      int s3 = rand() * 2 / RAND_MAX + 1;
      int s4 = rand() * 2 / RAND_MAX + 1;

      int c1 = rand() * (cx1 - 10) / RAND_MAX + 1;
      int c2 = rand() * (cy1 - 10) / RAND_MAX + 1;
      int c3 = rand() * 10 / RAND_MAX + 1;
      int c4 = rand() * 10 / RAND_MAX + 1;

      StepGrow001(
      m_color,
      m_iGrowColor,
      m_iGrowMax);





//            argb(255, rand() * 255 / RAND_MAX,
//            rand() * 255 / RAND_MAX,
//            rand() * 255 / RAND_MAX);

      unsigned char bA = 50;


#ifdef WINDOWS_DESKTOP
      //GdiFlush();
#endif

      //double dmin = minimum(cx, cy);
      //double dmax = maximum(cx, cy);abs


      // ::draw2d::graphics_pointer pimage3->get_graphics();
      //pimage3->get_graphics()->CreateCompatibleDC(nullptr);
      //pimage3->get_graphics()->set(pimage3->m_hbitmap);

      //   pimage3->stretch(imageBuffer);

      //   pimage3->Paste(pimage1);



      double dAngleOffset = (20.0 * rand() / RAND_MAX) - 5.0;
      double dAngle;
      if(m_iGrowColor >= 0)
      {
         dAngle = 10.111 + dAngleOffset;
      }
      else
      {
         dAngle = -10.111 - dAngleOffset;
      }
      pimage2->rotate(pimage1, radians(dAngle), 1.009);
      pimage1->g()->fill_rectangle({ cxm - s1, cym - s2, s3, s4 }, m_color & ::opacity(bA));


      // top
      pimage1->g()->fill_rectangle({ 0, 0, cx1 - 1, 1 }, m_color & ::opacity(bA));
      // left
      pimage1->g()->fill_rectangle({ 0, 0, 1, cy1 - 1 }, m_color & ::opacity(bA));
      // bottom
      pimage1->g()->fill_rectangle({ 0, cy1 - 1, cx1 - 1, 1 }, m_color & ::opacity(bA));
      // right
      pimage1->g()->fill_rectangle({ cx1 - 2, 0, 1, cy1 - 1 }, m_color & ::opacity(bA));


      pimage1->g()->fill_rectangle({ c1, c2, c3, c4 }, m_color & ::opacity(bA));

      pimage1->g()->fill_rectangle({ cx1 - c1, cy1 - c2, c3, c4 }, m_color & ::opacity(bA));

      /*imaging.bitmap_blend(
         pimage1->get_graphics(),
         0, 0, cx1, cy1,
         pimage2->get_graphics(),
         0, 0,
         50);*/

      pimage2->channel_multiply(10.0 / 255.0, ::color::e_channel_opacity);

      {

         ::image::image_source imagesource(pimage2);

         double_rectangle rectangle(0, 0, cx1, cy1);

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimage1->get_graphics()->draw(imagedrawing);

      }

      pimageBack->g()->set_interpolation_mode(VISUAL_EFFECT_STRETCH_MODE);

      {

         ::image::image_source imagesource(pimage1);

         double_rectangle rectangle(pimageBack->size());

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimageBack->draw(imagedrawing);

      }

      recta.add(rectangleX);

   }


   void visual_effect::RenderNPRotateTrackColorBlend(int_rectangle_array & recta)
   {
      main * pmain = helper_get_main();

      transition_effect * ptransitioneffect = pmain->get_transition_effect();

      ::int_rectangle rectangleX = m_tool1.m_rectangleX;
      m_tool1.cx() = rectangleX.width();
      m_tool1.cy() = rectangleX.height();

      //auto psystem = m_papplication;

      //class imaging & imaging = psystem->imaging();

      gcom * pgcom = pmain->get_gcom();

      ASSERT(pgcom->GetBufferDC()->get_os_data() != nullptr);

      ::image::image_pointer pimageBack = pgcom->get_image(e_image_back);
      ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
      ::image::image_pointer pimage2 = pgcom->get_image(e_image_temp2);
      ::image::image_pointer pimage3 = pgcom->get_image(e_image_temp3);

      int cx1 = pimage1->width();
      int cy1 = pimage1->height();
      int cxm = cx1 / 2;
      int cym = cy1 / 2;

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         pimage2->create({cx1, cy1});
         pimage3->create({cx1, cy1});
      }

      int s1 = rand() * 1 / RAND_MAX + 1;
      int s2 = rand() * 1 / RAND_MAX + 1;
      int s3 = rand() * 2 / RAND_MAX + 1;
      int s4 = rand() * 2 / RAND_MAX + 1;

      m_rndgrowVelocity.Step();
      if(m_rndgrowDirectionLatency.Step())
      {
         m_dDirection = ((int) m_dDirection % 360);
         m_dDirectionAddUp = 4.0 * rand() / RAND_MAX - 2.0;

      }
      m_dDirection += m_dDirectionAddUp;

      double dcos = ::cos(m_dDirection * M_PI / 180.0);
      double dsin = ::sin(m_dDirection * M_PI / 180.0);

      StepGrow001(
      m_color,
      m_iGrowColor,
      m_iGrowMax);

      m_pointColorTrack.x += (int)(fabs(m_rndgrowVelocity.i) * dcos);
      m_pointColorTrack.y += (int)(fabs(m_rndgrowVelocity.i) * dsin);

      if(m_pointColorTrack.x > cx1 - 10)
      {
         //      m_dDirection += 180 + ((int) m_dDirection % 360);
         m_dDirection = -m_dDirection;
      }
      else if(m_pointColorTrack.y > cy1 - 10)
      {
         //    m_dDirection += 180 + ((int) m_dDirection % 360);
         m_dDirection = -m_dDirection;
      }
      else if(m_pointColorTrack.x < 0)
      {
         //  m_dDirection += 180 + ((int) m_dDirection % 360);
         m_dDirection = -m_dDirection;
      }
      else if(m_pointColorTrack.y < 0)
      {
         //m_dDirection += 180 + ((int) m_dDirection % 360);
         m_dDirection = -m_dDirection;
      }

      if(m_pointColorTrack.x > cx1 - 10)
      {
         m_pointColorTrack.x = cx1 - 10;
      }
      else if(m_pointColorTrack.x < 0)
      {
         m_pointColorTrack.x = 0;
      }

      if(m_pointColorTrack.y > cy1 - 10)
      {
         m_pointColorTrack.y = cy1 - 10;
      }
      else if(m_pointColorTrack.y < 0)
      {
         m_pointColorTrack.y = 0;
      }


//
//            argb(255, rand() * 255 / RAND_MAX,
//            rand() * 255 / RAND_MAX,
//            rand() * 255 / RAND_MAX);

      unsigned char bA = 50;


#ifdef WINDOWS_DESKTOP
      //GdiFlush();
#endif

      //double dmin = minimum(cx, cy);
      //double dmax = maximum(cx, cy);abs

      // ::draw2d::graphics_pointer pimage3->get_graphics();
      //pimage3->get_graphics()->CreateCompatibleDC(nullptr);
      //pimage3->get_graphics()->set(pimage3->m_hbitmap);

      //   pimage3->stretch(imageBuffer);

      //   pimage3->Paste(pimage1);

//         int c1 = rand() * 3 / RAND_MAX;
//         int c2 = rand() * 3 / RAND_MAX;
      int c3 = rand() * 10 / RAND_MAX + 1;
//         int c4 = rand() * 10 / RAND_MAX + 1;


      double dAngleOffset = (20.0 * rand() / RAND_MAX) - 5.0;
      double dAngle;
      if(m_iGrowColor >= 0)
      {


         dAngle = 10.111 + dAngleOffset;
      }
      else
      {

         dAngle = -10.111 - dAngleOffset;
      }
      pimage2->rotate(pimage1, radians(dAngle), 1.009);
      pimage1->g()->fill_rectangle({ cxm - s1, cym - s2, s3, s4 }, m_color & ::opacity(bA));


      // top
      pimage1->g()->fill_rectangle({ 0, 0, cx1 - 1, 1 }, m_color & ::opacity(bA));
      // left
      pimage1->g()->fill_rectangle({ 0, 0, 1, cy1 - 1 }, m_color & ::opacity(bA));
      // bottom
      pimage1->g()->fill_rectangle({ 0, cy1 - 1, cx1 - 1, 1 }, m_color & ::opacity(bA));
      // right
      pimage1->g()->fill_rectangle({ cx1 - 2, 0, 1, cy1 - 1 }, m_color & ::opacity(bA));


      {
         int x1 = m_pointColorTrack.x;
         int y1 = m_pointColorTrack.y;
         int x2 = cx1 - x1;
         int y2 = cy1 - y1;

         auto pbrush = øcreate < ::draw2d::brush >();

         pbrush->create_solid(m_color.opaque());

         pimage1->get_graphics()->set(pbrush);
         //pimage1->g()->fill_rectangle({ x, y, c3, c4, m_color & ::opacity(bA));
         //pimage1->g()->fill_rectangle({ cx1 - x, cy1 - y, c3, c4, m_color & ::opacity(bA));
         pimage1->get_graphics()->fill_ellipse(x1, y1, x1 + c3, y1 + c3);
         pimage1->get_graphics()->fill_ellipse(x2 - c3, y2 - c3, x2, y2);
      }

      /*imaging.bitmap_blend(
         pimage1->get_graphics(),
         0, 0, cx1, cy1,
         pimage2->get_graphics(),
         0, 0,
         50);*/

      pimage2->channel_multiply(10.0 / 255.0, ::color::e_channel_opacity);

      {

         ::image::image_source imagesource(pimage2);

         auto rectangle = double_rectangle_dimension(0, 0, cx1, cy1);

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimage1->get_graphics()->draw(imagedrawing);

      }

      pimageBack->g()->set_interpolation_mode(VISUAL_EFFECT_STRETCH_MODE);

      {

         ::image::image_source imagesource(pimage1);

         double_rectangle rectangle(pimageBack->size());

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimageBack->draw(imagedrawing);

      }

      recta.add(rectangleX);

   }


   void visual_effect::InitialRender()
   {

      m_tool1.m_iPage = 0;

      main * pmain = helper_get_main();

      user_interaction * puserinteraction = pmain->get_user_interaction();

      ::int_rectangle rectangleX;

      puserinteraction->backimpact_get_client_rect(rectangleX);

      m_tool1.m_rectangleX = rectangleX;

      int cx = rectangleX.width();

      int cy = rectangleX.height();

      gcom * pgcom = pmain->get_gcom();

      if (pgcom->GetBufferDC()== nullptr || pgcom->GetBufferDC()->get_os_data() == nullptr)
         return;

      single_lock sl1Back(pgcom->m_pmutex1Back, false);

      single_lock sl2Buffer(pgcom->m_pmutex2Buffer, false);

      ::draw2d::graphics_pointer dcBack = pgcom->GetBackDC();

      ::draw2d::graphics_pointer dcBuffer = pgcom->GetBufferDC();

      sl1Back.lock();

      sl2Buffer.lock();

      ::draw2d::save_context k1(dcBack);
      ::draw2d::save_context k2(dcBuffer);
      //dcBack->SelectClipRgn(nullptr);
      //dcBuffer->SelectClipRgn(nullptr);

      {

         ::image::image_source imagesource(dcBuffer);

         auto rectangle = double_rectangle_dimension(0, 0, cx, cy);

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         dcBack->draw(imagedrawing);

      }



      ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
//         ::image::image_pointer pimageBuffer = pgcom->get_image(e_image_buffer);
      ::image::image_pointer pimage2 = pgcom->get_image(e_image_temp2);
//         ::image::image_pointer pimageTransfer = pgcom->get_image(e_image_transfer);


      int cx1 = cx / 4 + 1;
      int cy1 = cy / 4 + 1;


      if(m_iVisual == VisualEffectRotateEx5)
      {
         cx1 = cx / 5;
         cy1 = cy / 5;
      }

      pimage1->create({cx1, cy1});
      pimage2->create({cx, cy});

      m_explosion.create(cx1, cy1, pimage1->scan_size(), pimage1->color_indexes());
      m_explosion.Reset();


      int_point      m_pointColorTrack;

      m_rndgrowVelocity.i = 0;
      m_rndgrowVelocity.lbound = 1;
      m_rndgrowVelocity.maxlbound = 1;
      m_rndgrowVelocity.maxubound = 5;
      m_dDirection = 360.0 * rand() / RAND_MAX;
      m_dDirectionAddUp = 0.5;
      m_rndgrowDirectionLatency.i = 0;
      m_rndgrowDirectionLatency.lbound = 0;
      m_rndgrowDirectionLatency.maxlbound = 50;
      m_rndgrowDirectionLatency.maxubound = 100;

      m_colortrack1a.set_size(12);
      for(int i = 0; i < m_colortrack1a.get_size(); i++)
      {
         color_track1 & ct1 = m_colortrack1a[i];
         ct1.m_rndgrowVelocity.i = 0;
         ct1.m_rndgrowVelocity.lbound = 1;
         ct1.m_rndgrowVelocity.maxlbound = 1;
         ct1.m_rndgrowVelocity.maxubound = 5;
         ct1.m_dDirection = 360.0 * rand() / RAND_MAX;
         ct1.m_dDirectionAddUp = 0.5;
         ct1.m_rndgrowDirectionLatency.i = 0;
         ct1.m_rndgrowDirectionLatency.lbound = 0;
         ct1.m_rndgrowDirectionLatency.maxlbound = 50;
         ct1.m_rndgrowDirectionLatency.maxubound = 100;
      }

      m_rndgrowAngleOffsetLatency.i = 0;
      m_rndgrowAngleOffsetLatency.lbound = 0;
      m_rndgrowAngleOffsetLatency.maxlbound = 100;
      m_rndgrowAngleOffsetLatency.maxubound = 200;

      m_dAngle = 0.0;

   }

   int visual_effect::get_effect_count()
   {
      return (int) VisualEffectEnd;
   }

   void visual_effect::StepGrow001(
   ::color::color & color,
   int & iGrowColor,
   int & iGrowMax)
   {
      double dH;
      double dL;
      double dS;
      color.get_hls(dH, dL, dS);
      dL = 0.7;
      dS = 0.7;
      if(iGrowColor >= 0)
      {
         iGrowColor++;
         if(iGrowMax <= 0)
         {
            iGrowMax = 100 * rand() / RAND_MAX + 20;
         }
         else if(iGrowMax > 120)
         {
            iGrowMax = 120;
         }
         if(iGrowColor > iGrowMax)
            iGrowColor = -1;

         dH += rand() * 0.003 / DRAND_MAX;

         if(dH >= 1.0)
         {
            dH = 1.0;
            iGrowColor = -1;
         }
      }
      else
      {
         iGrowColor--;
         if(iGrowMax >= 0)
         {
            iGrowMax = -100 * rand() / RAND_MAX - 20;
         }
         else if(iGrowMax < -120)
         {
            iGrowMax = -120;
         }
         if(iGrowColor < iGrowMax)
            iGrowColor = 1;

         dH -= rand() * 0.003 / DRAND_MAX;

         if(dH < 0.0)
         {
            dH = 0.0;
            iGrowColor = 1;
         }
      }

      color.set_hls(dH, dL, dS);

   }

   void visual_effect::StepGrow034(
   ::color::color & color,
   int & iGrowColor,
   int & iGrowMax)
   {
      double dH;
      double dL;
      double dS;
      color.get_hls(dH, dL, dS);
      dL = 0.7;
      dS = 0.7;
      if(iGrowColor >= 0)
      {
         iGrowColor++;
         if(iGrowMax <= 0)
         {
            iGrowMax = 100 * rand() / RAND_MAX + 20;
         }
         else if(iGrowMax > 120)
         {
            iGrowMax = 120;
         }
         if(iGrowColor > iGrowMax)
            iGrowColor = -1;

         dH += rand() * 0.003 / RAND_MAX;

         if(dH >= 1.0)
         {
            dH = 1.0;
            iGrowColor = -1;
         }
      }
      else
      {
         iGrowColor--;
         if(iGrowMax >= 0)
         {
            iGrowMax = -100 * rand() / RAND_MAX - 20;
         }
         else if(iGrowMax < -120)
         {
            iGrowMax = -120;
         }
         if(iGrowColor < iGrowMax)
            iGrowColor = 1;

         dH -= rand() * 0.003 / RAND_MAX;

         if(dH < 0.0)
         {
            dH = 0.0;
            iGrowColor = 1;
         }
      }

      color.set_hls(dH, dL, dS);

   }

   void visual_effect::StepGrow001(
   ::color::color & color,
   int & iGrowColor,
   int & iGrowMax,
   bool & bGrowColor)
   {
      double dH;
      double dL;
      double dS;
      color.get_hls(dH, dL, dS);
      dL = 0.7;
      dS = 0.7;
      if(iGrowColor >= 0)
      {
         bGrowColor = false;
         iGrowColor++;
         if(iGrowMax <= 0)
         {
            iGrowMax = 100 * rand() / RAND_MAX + 20;
         }
         else if(iGrowMax > 120)
         {
            iGrowMax = 120;
         }
         if(iGrowColor > iGrowMax)
         {
            bGrowColor = true;
            iGrowColor = -1;
         }

         dH += rand() * 0.003 / RAND_MAX;

         if(dH >= 1.0)
         {
            dH = 1.0;
            iGrowColor = -1;
            bGrowColor = true;
         }
      }
      else
      {
         bGrowColor = false;
         iGrowColor--;
         if(iGrowMax >= 0)
         {
            iGrowMax = -100 * rand() / RAND_MAX - 20;
         }
         else if(iGrowMax < -120)
         {
            iGrowMax = -120;
         }
         if(iGrowColor < iGrowMax)
         {
            bGrowColor = true;
            iGrowColor = 1;
         }

         dH -= rand() * 0.003 / RAND_MAX;

         if(dH < 0.0)
         {
            dH = 0.0;
            iGrowColor = 1;
            bGrowColor = true;
         }
      }

      color.set_hls(dH, dL, dS);

   }


   bool visual_effect::random_grow::Step()
   {
      bool b = false;

      if(i >= 0)
      {
         i++;
         if(imax <= 0)
         {
            ASSERT(maxubound >= 0);
            ASSERT(maxlbound >= 0);
            imax = (maxubound - maxlbound) * rand() / RAND_MAX + maxlbound;
         }
         else if(imax > maxubound)
         {
            imax = maxubound;
         }
         if(i > imax)
         {
            i = -lbound;
            b = true;
         }

      }
      else
      {
         i--;
         if(imax >= 0)
         {
            ASSERT(maxubound >= 0);
            ASSERT(maxlbound >= 0);
            imax = (maxlbound - maxubound) * rand() / RAND_MAX - maxlbound;
         }
         else if(imax < -maxubound)
         {
            imax = -maxubound;
         }
         if(i < imax)
         {
            i = lbound;
            b = true;
         }
      }
      return b;
   }
   
   void visual_effect::RenderRotateEx1(int_rectangle_array & recta)
   {
      
      main * pmain = helper_get_main();

      transition_effect * ptransitioneffect = pmain->get_transition_effect();

      ::int_rectangle rectangleX = m_tool1.m_rectangleX;

      m_tool1.cx() = rectangleX.width();
      m_tool1.cy() = rectangleX.height();

      //auto psystem = m_papplication;

      //class imaging & imaging = psystem->imaging();

      gcom * pgcom = pmain->get_gcom();

      ASSERT(pgcom->GetBufferDC()->get_os_data() != nullptr);

      ::image::image_pointer pimageBack = pgcom->get_image(e_image_back);
      ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
      ::image::image_pointer pimage2 = pgcom->get_image(e_image_temp2);
      ::image::image_pointer pimage3 = pgcom->get_image(e_image_temp3);

      int cx1 = pimage1->width();
      int cy1 = pimage1->height();
      int cxm = cx1 / 2;
      int cym = cy1 / 2;

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         pimage2->create({cx1, cy1});
         pimage3->create({cx1, cy1});
      }

      int s1 = rand() * 1 / RAND_MAX + 1;
      int s2 = rand() * 1 / RAND_MAX + 1;
      int s3 = rand() * 2 / RAND_MAX + 1;
      int s4 = rand() * 2 / RAND_MAX + 1;

      m_rndgrowVelocity.Step();
      if(m_rndgrowDirectionLatency.Step())
      {
         m_dDirection = ((int) m_dDirection % 360);
         m_dDirectionAddUp = 4.0 * rand() / RAND_MAX - 2.0;

      }
      m_dDirection += m_dDirectionAddUp;

      double dcos = ::cos(m_dDirection * M_PI / 180.0);
      double dsin = ::sin(m_dDirection * M_PI / 180.0);

      StepGrow001(
      m_color,
      m_iGrowColor,
      m_iGrowMax);

      ::color::color colorStrong = m_color;

      double dH;
      double dL;
      double dS;
      colorStrong.get_hls(dH, dL, dS);
      dL = 0.3;
      dS = 0.8;
      colorStrong.set_hls(dH, dL, dS);




      m_pointColorTrack.x += (int)(fabs(m_rndgrowVelocity.i) * dcos);
      m_pointColorTrack.y += (int)(fabs(m_rndgrowVelocity.i) * dsin);

      if(m_pointColorTrack.x > cx1 - 10)
      {
         m_dDirection += 180 + ((int) m_dDirection % 360);
      }
      else if(m_pointColorTrack.y > cy1 - 10)
      {
         m_dDirection += 180 + ((int) m_dDirection % 360);
      }
      else if(m_pointColorTrack.x < 0)
      {
         m_dDirection += 180 + ((int) m_dDirection % 360);
      }
      else if(m_pointColorTrack.y < 0)
      {
         m_dDirection += 180 + ((int) m_dDirection % 360);
      }

      if(m_pointColorTrack.x > cx1 - 10)
      {
         m_pointColorTrack.x = cx1 - 10;
      }
      else if(m_pointColorTrack.x < 0)
      {
         m_pointColorTrack.x = 0;
      }

      if(m_pointColorTrack.y > cy1 - 10)
      {
         m_pointColorTrack.y = cy1 - 10;
      }
      else if(m_pointColorTrack.y < 0)
      {
         m_pointColorTrack.y = 0;
      }



//            argb(255, rand() * 255 / RAND_MAX,
//            rand() * 255 / RAND_MAX,
//            rand() * 255 / RAND_MAX);

      unsigned char bA = 200;


#ifdef WINDOWS_DESKTOP
      //GdiFlush();
#endif

      //double dmin = minimum(cx, cy);
      //double dmax = maximum(cx, cy);abs

      // ::draw2d::graphics_pointer pimage3->get_graphics();
      //pimage3->get_graphics()->CreateCompatibleDC(nullptr);
      //pimage3->get_graphics()->set(pimage3->m_hbitmap);

      //   pimage3->stretch(imageBuffer);

      //   pimage3->Paste(pimage1);

//         int c1 = rand() * 3 / RAND_MAX;
//         int c2 = rand() * 3 / RAND_MAX;
      int c3 = rand() * 10 / RAND_MAX + 1;
//         int c4 = rand() * 10 / RAND_MAX + 1;


      double dAngleOffset = (20.0 * rand() / RAND_MAX) - 5.0;
      double dAngle;
      if(m_iGrowColor >= 0)
      {


         dAngle = 10.111 + dAngleOffset;
      }
      else
      {

         dAngle = -10.111 - dAngleOffset;
      }
      pimage2->rotate(pimage1, radians(dAngle), 1.009);
      //pimage2->rotate(pimage1, dAngle, 0.9);
      //pimage2->Paste(pimage1);
      pimage1->g()->fill_rectangle({ cxm - s1, cym - s2, s3, s4 }, m_color & ::opacity(bA));


      // top
      pimage1->g()->fill_rectangle({ 0, 0, cx1 - 1, 1 }, m_color & ::opacity(bA));
      // left
      pimage1->g()->fill_rectangle({ 0, 0, 1, cy1 - 1 }, m_color & ::opacity(bA));
      // bottom
      pimage1->g()->fill_rectangle({ 0, cy1 - 1, cx1 - 1, 1 }, m_color & ::opacity(bA));
      // right
      pimage1->g()->fill_rectangle({ cx1 - 2, 0, 1, cy1 - 1 }, m_color & ::opacity(bA));


      {
         int d = c3;
         int r = d / 2;
         int x1 = m_pointColorTrack.x - r;
         int y1 = m_pointColorTrack.y - r;
         int x2 = cx1 - x1 - r;
         int y2 = cy1 - y1 - r;
         auto pbrush = øcreate < ::draw2d::brush >();
         pbrush->create_solid(colorStrong.opaque());
         pimage2->get_graphics()->set(pbrush);
         //pimage1->g()->fill_rectangle({ x, y, c3, c4, m_color & ::opacity(bA));
         //pimage1->g()->fill_rectangle({ cx1 - x, cy1 - y, c3, c4, m_color & ::opacity(bA));
         pimage2->get_graphics()->fill_ellipse(x1, y1, x1 + d, y1 + d);
         pimage2->get_graphics()->fill_ellipse(x2 - d, y1, x2, y1 + d);
         pimage2->get_graphics()->fill_ellipse(x1, y2 - d, x1 + d, y2);
         pimage2->get_graphics()->fill_ellipse(x2 - d, y2 - d, x2, y2);
      }

      /*imaging.bitmap_blend(
         pimage1->get_graphics(),
         0, 0, cx1, cy1,
         pimage2->get_graphics(),
         0, 0,
         50);*/

      pimage2->channel_multiply(10.0 / 255.0, ::color::e_channel_opacity);

      {

         ::image::image_source imagesource(pimage2);

         auto rectangle = double_rectangle_dimension(0, 0, cx1, cy1);

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagedrawing);

         pimage1->get_graphics()->draw(imagedrawing);

      }

      pimageBack->g()->set_interpolation_mode(VISUAL_EFFECT_STRETCH_MODE);

      {

         ::image::image_source imagesource(pimage1);

         double_rectangle rectangle(pimageBack->size());

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimageBack->draw(imagedrawing);

      }

      recta.add(rectangleX);

   }


   void visual_effect::RenderRotateEx2(int_rectangle_array & recta)
   {

      main * pmain = helper_get_main();

      transition_effect * ptransitioneffect = pmain->get_transition_effect();

      gcom * pgcom = pmain->get_gcom();

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
         m_color = pimage1->GetPixel(
                         pimage1->width() / 2,
                         pimage1->height() / 2);
      }



      ::int_rectangle rectangleX = m_tool1.m_rectangleX;
      m_tool1.cx() = rectangleX.width();
      m_tool1.cy() = rectangleX.height();

      ASSERT(pgcom->GetBufferDC()->get_os_data() != nullptr);

      ::image::image_pointer pimageBack = pgcom->get_image(e_image_back);
      ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
      ::image::image_pointer pimage2 = pgcom->get_image(e_image_temp2);
      ::image::image_pointer pimage3 = pgcom->get_image(e_image_temp3);

      int cx1 = pimage1->width();
      int cy1 = pimage1->height();

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         pimage2->create({cx1, cy1});
         pimage3->create({cx1, cy1});
      }


      ::int_rectangle rcTrackBox;

      rcTrackBox.left()   = cx1 * 4 / 9;
      rcTrackBox.top()    = cy1 * 4 / 9;
      rcTrackBox.right()  = cx1 - rcTrackBox.left();
      rcTrackBox.bottom() = cy1 - rcTrackBox.top();

      for(int i = 0; i < m_colortrack1a.get_size(); i++)
      {
         color_track1 & ct1 = m_colortrack1a[i];
         ct1.m_rndgrowVelocity.Step();
         TrackDirection001(
         ct1.m_dDirection,
         ct1.m_dDirectionAddUp,
         ct1.m_rndgrowDirectionLatency);
         ct1.m_dCos = ::cos(ct1.m_dDirection * M_PI / 180.0);
         ct1.m_dSin = ::sin(ct1.m_dDirection * M_PI / 180.0);
         ct1.m_pointColorTrack.x += (int)(fabs(ct1.m_rndgrowVelocity.i) * ct1.m_dCos);
         ct1.m_pointColorTrack.y += (int)(fabs(ct1.m_rndgrowVelocity.i) * ct1.m_dSin);
         Constraint001(
         ct1.m_pointColorTrack,
         rcTrackBox,
         ct1.m_dDirection);
         ct1.m_dSize = rand() * 5 / RAND_MAX + 1;
      }

      StepGrow001(
      m_color,
      m_iGrowColor,
      m_iGrowMax);

      StepGrow001(
      m_color2,
      m_iGrowColor2,
      m_iGrowMax2);

      StepGrow001(
      m_color3,
      m_iGrowColor3,
      m_iGrowMax3);

      StepGrow001(
      m_color4,
      m_iGrowColor4,
      m_iGrowMax4);

      ::color::color colorStrong1 = m_color;
      ::color::color colorStrong2 = m_color2;
      ::color::color colorStrong3 = m_color3;
      ::color::color colorStrong4 = m_color4;
      ::color::color colorStrong5;

      {
         double dH;
         double dL;
         double dS;
         colorStrong1.get_hls(dH, dL, dS);
         dL = 0.3;
         dS = 0.8;
         colorStrong1.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong2.get_hls(dH, dL, dS);
         dL = 0.9;
         dS = 0.5;
         colorStrong2.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong2.get_hls(dH, dL, dS);
         dL = 0.9;
         dS = 0.7;
         colorStrong3.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong4.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.8;
         colorStrong4.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong4.get_hls(dH, dL, dS);
         dL = 0.5;
         dS = 0.5;
         colorStrong5.set_hls(dH, dL, dS);
      }



//            argb(255, rand() * 255 / RAND_MAX,
//            rand() * 255 / RAND_MAX,
//            rand() * 255 / RAND_MAX);

//         unsigned char bA = 200;


#ifdef WINDOWS_DESKTOP
      //GdiFlush();
#endif

      //double dmin = minimum(cx, cy);
      //double dmax = maximum(cx, cy);abs

      // ::draw2d::graphics_pointer pimage3->get_graphics();
      //pimage3->get_graphics()->CreateCompatibleDC(nullptr);
      //pimage3->get_graphics()->set(pimage3->m_hbitmap);

      //   pimage3->stretch(imageBuffer);

      //   pimage3->Paste(pimage1);

//         int c1 = rand() * 3 / RAND_MAX;
//         int c2 = rand() * 3 / RAND_MAX;
//         int c4 = rand() * 10 / RAND_MAX + 1;


      m_rndgrowAngleOffsetLatency.Step();
      if(m_rndgrowAngleOffsetLatency.i == 0)
      {
         m_dAngleOffsetAddUp = (4.0 * rand() / RAND_MAX) - 2.0;
      }
      m_dAngleOffset += m_dAngleOffsetAddUp;
      m_dAngleOffset = fmod(m_dAngleOffset, 360.0);

      double dAngleOffset = m_dAngleOffset;
      double dAngle;
      if(m_iGrowColor >= 0)
      {


         dAngle = 0.111 + dAngleOffset;
      }
      else
      {

         dAngle = -0.111 - dAngleOffset;
      }
      //pimage2->rotate(pimage1, dAngle, 1.009);
      pimage2->Rotate034(pimage1, dAngle, 0.8);
      //pimage2->Paste(pimage1);
      //   pimage1->g()->fill_rectangle({ cxm - s1, cym - s2, s3, s4}, m_color & ::opacity(bA));


      // top
      /*pimage1->g()->fill_rectangle({ 0, 0, cx1 - 1, 1}, m_color & ::opacity(bA));
      // left
      pimage1->g()->fill_rectangle({ 0, 0, 1, cy1 - 1}, m_color & ::opacity(bA));
      // bottom
      pimage1->g()->fill_rectangle({ 0, cy1 - 1, cx1 - 1, 1}, m_color & ::opacity(bA));
      // right
      pimage1->g()->fill_rectangle({ cx1 - 2, 0, 1, cy1 - 1}, m_color & ::opacity(bA));*/


      {
         auto ppen1 = øcreate < ::draw2d::pen > ();
         ppen1->create_solid(1.0, colorStrong1.opaque());
         ::draw2d::brush_pointer brush1(e_create, this);
         brush1->create_solid(colorStrong1.opaque());
         auto ppen2 = øcreate < ::draw2d::pen > ();
         ppen2->create_solid(1.0, colorStrong2.opaque());
         ::draw2d::brush_pointer brush2(e_create, this);
         brush2->create_solid(colorStrong2.opaque());
         auto ppen3 = øcreate < ::draw2d::pen > ();
         ppen3->create_solid(1.0, colorStrong3.opaque());
         ::draw2d::brush_pointer brush3(e_create, this);
         brush3->create_solid(colorStrong3.opaque());
         auto ppen4 = øcreate < ::draw2d::pen > ();
         ppen4->create_solid(1.0, colorStrong4.opaque());
         ::draw2d::brush_pointer brush4(e_create, this);
         brush4->create_solid(colorStrong4.opaque());
         auto ppen5 = øcreate < ::draw2d::pen > ();
         ppen5->create_solid(1.0, colorStrong5.opaque());
         ::draw2d::brush_pointer brush5(e_create, this);
         brush5->create_solid(colorStrong5.opaque());

         for(int i = 0; i < m_colortrack1a.get_size(); i++)
         {
            color_track1 & ct1 = m_colortrack1a[i];
            int x1 = ct1.m_pointColorTrack.x;
            int y1 = ct1.m_pointColorTrack.y;
            int d = (int) ct1.m_dSize;
            switch(i % 5)
            {
            case 0:
            {
               pimage2->get_graphics()->set(brush1);
               pimage2->get_graphics()->set(ppen1);
            }
            break;
            case 1:
            {
               pimage2->get_graphics()->set(brush2);
               pimage2->get_graphics()->set(ppen2);
            }
            break;
            case 2:
            {
               pimage2->get_graphics()->set(brush3);
               pimage2->get_graphics()->set(ppen3);
            }
            case 3:
            {
               pimage2->get_graphics()->set(brush4);
               pimage2->get_graphics()->set(ppen4);
            }
            case 4:
            {
               pimage2->get_graphics()->set(brush5);
               pimage2->get_graphics()->set(ppen5);
            }
            break;
            }
            pimage2->get_graphics()->ellipse(x1, y1, x1 + d, y1 + d);
         }

      }

      /*imaging.bitmap_blend(
         pimage1->get_graphics(),
         0, 0, cx1, cy1,
         pimage2->get_graphics(),
         0, 0,
         50);*/

      pimage2->channel_multiply(0.7, ::color::e_channel_opacity);

      {

         ::image::image_source imagesource(pimage2);

         auto rectangle = double_rectangle_dimension(0, 0, cx1, cy1);

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimage1->get_graphics()->draw(imagedrawing);

      }

      pimageBack->g()->set_interpolation_mode(VISUAL_EFFECT_STRETCH_MODE);

      {

         ::image::image_source imagesource(pimage1);

         double_rectangle rectangle(pimageBack->size());

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimageBack->draw(imagedrawing);

      }

      recta.add(rectangleX);

   }


   void visual_effect::Constraint001(int_point & point, const ::int_rectangle & rectangle, double & direction)
   {
      if(point.x > rectangle.right())
      {
         direction  += 180 + ((int) direction  % 360);
      }
      else if(point.y > rectangle.bottom())
      {
         direction  += 180 + ((int) direction  % 360);
      }
      else if(point.x < rectangle.left())
      {
         direction  += 180 + ((int) direction  % 360);
      }
      else if(point.y < rectangle.top())
      {
         direction  += 180 + ((int) direction  % 360);
      }

      if(point.x > rectangle.right())
      {
         point.x = rectangle.right();
      }
      else if(point.x < rectangle.left())
      {
         point.x = rectangle.left();
      }

      if(point.y > rectangle.bottom())
      {
         point.y = rectangle.bottom();
      }
      else if(point.y < rectangle.top())
      {
         point.y = rectangle.top();
      }

   }
   void visual_effect::TrackDirection001(double &direction, double & directionAddUp, random_grow & randomgrow)
   {
      if(randomgrow.Step())
      {
         direction = ((int) direction % 360);
         directionAddUp = 4.0 * rand() / RAND_MAX - 2.0;

      }
      direction += directionAddUp;
   }


   void visual_effect::RenderRotateEx3(int_rectangle_array & recta)
   {

      main * pmain = helper_get_main();

      transition_effect * ptransitioneffect = pmain->get_transition_effect();

      gcom * pgcom = pmain->get_gcom();

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
         m_color = pimage1->GetPixel(
                         pimage1->width() / 2,
                         pimage1->height() / 2);
      }



      ::int_rectangle rectangleX = m_tool1.m_rectangleX;
      m_tool1.cx() = rectangleX.width();
      m_tool1.cy() = rectangleX.height();

      //auto psystem = m_papplication;

      //class imaging & imaging = psystem->imaging();


      ASSERT(pgcom->GetBufferDC()->get_os_data() != nullptr);

      ::image::image_pointer pimageBack = pgcom->get_image(e_image_back);
      ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
      ::image::image_pointer pimage2 = pgcom->get_image(e_image_temp2);
      ::image::image_pointer pimage3 = pgcom->get_image(e_image_temp3);

      int cx1 = pimage1->width();
      int cy1 = pimage1->height();

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         pimage2->create({cx1, cy1});
         pimage3->create({cx1, cy1});
      }


//         int s1 = rand() * 1 / RAND_MAX + 1;
//         int s2 = rand() * 1 / RAND_MAX + 1;
//         int s3 = rand() * 2 / RAND_MAX + 1;
//         int s4 = rand() * 2 / RAND_MAX + 1;

      ::int_rectangle rcTrackBox;

      rcTrackBox.left()   = 1;
      rcTrackBox.top()    = 1;
      rcTrackBox.right()  = cx1 - 1 - rcTrackBox.left();
      rcTrackBox.bottom() = cy1 - 1 - rcTrackBox.top();

      ::int_rectangle rcTrackBox2;

      rcTrackBox2.left()   = 0;
      rcTrackBox2.top()    = 0;
      rcTrackBox2.right()  = cx1 - 1 - rcTrackBox2.left();
      rcTrackBox2.bottom() = cy1 - 1 - rcTrackBox2.top();

      /*for(int i = 0; i < m_colortrack1a.get_size(); i++)
      {
         color_track1 & ct1 = m_colortrack1a[i];
         ct1.m_rndgrowVelocity.Step();
         TrackDirection001(
            ct1.m_dDirection,
            ct1.m_dDirectionAddUp,
            ct1.m_rndgrowDirectionLatency);
         ct1.m_dCos = ::cos(ct1.m_dDirection * M_PI / 180.0);
         ct1.m_dSin = ::sin(ct1.m_dDirection * M_PI / 180.0);
         ct1.m_pointColorTrack.x += abs(ct1.m_rndgrowVelocity.i) * ct1.m_dCos;
         ct1.m_pointColorTrack.y += abs(ct1.m_rndgrowVelocity.i) * ct1.m_dSin;
         Constraint001(
            ct1.m_pointColorTrack,
            rcTrackBox,
            rcTrackBox2,
            ct1.m_dDirection);
         ct1.m_dSize = rand() * 5 / RAND_MAX + 1;
      }*/

      StepGrow001(
      m_color,
      m_iGrowColor,
      m_iGrowMax);

      StepGrow001(
      m_color2,
      m_iGrowColor2,
      m_iGrowMax2);

      StepGrow001(
      m_color3,
      m_iGrowColor3,
      m_iGrowMax3);

      StepGrow001(
      m_color4,
      m_iGrowColor4,
      m_iGrowMax4);

      ::color::color colorStrong1 = m_color;
      ::color::color colorStrong3 = m_color2;
      ::color::color colorStrong5 = m_color3;
      ::color::color colorStrong7 = m_color4;
      ::color::color colorStrong2;
      ::color::color colorStrong4;
      ::color::color colorStrong6;
      ::color::color colorStrong8;

      {
         double dH;
         double dL;
         double dS;
         colorStrong1.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.8;
         colorStrong1.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong1.get_hls(dH, dL, dS);
         dL = 0.9;
         dS = 0.7;
         colorStrong2.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong3.get_hls(dH, dL, dS);
         dL = 0.9;
         dS = 0.7;
         colorStrong3.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong3.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.9;
         colorStrong4.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong5.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.8;
         colorStrong5.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong5.get_hls(dH, dL, dS);
         dL = 0.5;
         dS = 0.9;
         colorStrong6.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong7.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.8;
         colorStrong7.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong7.get_hls(dH, dL, dS);
         dL = 0.5;
         dS = 0.9;
         colorStrong8.set_hls(dH, dL, dS);
      }
//
//
//            argb(255, rand() * 255 / RAND_MAX,
//            rand() * 255 / RAND_MAX,
//            rand() * 255 / RAND_MAX);

      unsigned char bA = 200;


#ifdef WINDOWS_DESKTOP
      //GdiFlush();
#endif

      //double dmin = minimum(cx, cy);
      //double dmax = maximum(cx, cy);abs

      // ::draw2d::graphics_pointer pimage3->get_graphics();
      //pimage3->get_graphics()->CreateCompatibleDC(nullptr);
      //pimage3->get_graphics()->set(pimage3->m_hbitmap);

      //   pimage3->stretch(imageBuffer);

      //   pimage3->Paste(pimage1);

//         int c1 = rand() * 3 / RAND_MAX;
//         int c2 = rand() * 3 / RAND_MAX;
//         int c4 = rand() * 10 / RAND_MAX + 1;


      m_rndgrowAngleOffsetLatency.Step();
      if(m_rndgrowAngleOffsetLatency.i == 0)
      {
         m_dAngleOffsetAddUp = (4.0 * rand() / RAND_MAX) - 2.0;
      }
      m_dAngleOffset += m_dAngleOffsetAddUp;
      m_dAngleOffset = fmod(m_dAngleOffset, 360.0);

      double dAngleOffset = m_dAngleOffset;
      double dAngle;
      if(m_iGrowColor >= 0)
      {
         dAngle = 0.111 + dAngleOffset;
      }
      else
      {

         dAngle = -0.111 - dAngleOffset;
      }
      pimage2->rotate(pimage1, radians(dAngle), 1.05);
      //pimage2->rotate(pimage1, dAngle, 1.009);
      //pimage2->Paste(pimage1);
      //pimage1->g()->fill_rectangle({ cxm - s1, cym - s2, s3, s4, m_color & ::opacity(bA));


      // top
      pimage1->g()->fill_rectangle({ 0, 0, cx1 / 2, 1 }, colorStrong1 & ::opacity(bA));
      pimage1->g()->fill_rectangle({ cx1 / 2, 0, cx1 / 2 - 1, 1 }, colorStrong2 & ::opacity(bA));
      // left
      pimage1->g()->fill_rectangle({ 0, 0, 1, cy1 / 2 }, colorStrong3 & ::opacity(bA));
      pimage1->g()->fill_rectangle({ 0, cy1 / 2, 1, cy1 / 2 - 1 }, colorStrong4 & ::opacity(bA));
      // bottom
      pimage1->g()->fill_rectangle({ 0, cy1 - 1, cx1 / 2, 1 }, colorStrong5 & ::opacity(bA));
      pimage1->g()->fill_rectangle({ cx1 / 2, cy1 - 1, cx1 / 2 - 1, 1 }, colorStrong6 & ::opacity(bA));
      // right
      pimage1->g()->fill_rectangle({ cx1 - 1, 0, 1, cy1 / 2 }, colorStrong7 & ::opacity(bA));
      pimage1->g()->fill_rectangle({ cx1 - 1, cy1 / 2, 1, cy1 / 2 - 1 }, colorStrong8 & ::opacity(bA));


      /*   {
            ::draw2d::pen_pointer ppen1(get_app(), PS_SOLID, 0, colorStrong1.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush1(get_app(), colorStrong1.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen2(get_app(), PS_SOLID, 0, colorStrong2.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush2(get_app(), colorStrong2.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen3(get_app(), PS_SOLID, 0, colorStrong3.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush3(get_app(), colorStrong3.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen4(get_app(), PS_SOLID, 0, colorStrong4.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush4(get_app(), colorStrong4.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen5(get_app(), PS_SOLID, 0, colorStrong5.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush5(get_app(), colorStrong5.get_rgb() | (255 << 24));
            for(int i = 0; i < m_colortrack1a.get_size(); i++)
            {
               color_track1 & ct1 = m_colortrack1a[i];
               int x1 = ct1.m_pointColorTrack.x;
               int y1 = ct1.m_pointColorTrack.y;
               int d = ct1.m_dSize;
               switch(i % 5)
               {
               case 0:
                  {
                     pimage1->get_graphics()->set(brush1);
                     pimage1->get_graphics()->set(ppen1);
                  }
                  break;
               case 1:
                  {
                     pimage1->get_graphics()->set(brush2);
                     pimage1->get_graphics()->set(ppen2);
                  }
                  break;
               case 2:
                  {
                     pimage1->get_graphics()->set(brush3);
                     pimage1->get_graphics()->set(ppen3);
                  }
               case 3:
                  {
                     pimage1->get_graphics()->set(brush4);
                     pimage1->get_graphics()->set(ppen4);
                  }
               case 4:
                  {
                     pimage1->get_graphics()->set(brush5);
                     pimage1->get_graphics()->set(ppen5);
                  }
                  break;
               }
               pimage1->get_graphics()->rectangle(x1, y1, x1 + d, y1 + d);
            }

         }*/



      /*imaging.bitmap_blend(
         pimage1->get_graphics(),
         0, 0, cx1, cy1,
         pimage2->get_graphics(),
         0, 0,
         50);*/

      pimage2->channel_multiply(10.0 / 255.0, ::color::e_channel_opacity);

      {

         ::image::image_source imagesource(pimage2);

         auto rectangle = double_rectangle_dimension(0, 0, cx1, cy1);

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimage1->get_graphics()->draw(imagedrawing);

      }

      pimageBack->g()->set_interpolation_mode(VISUAL_EFFECT_STRETCH_MODE);

      {

         ::image::image_source imagesource(pimage1);

         double_rectangle rectangle(pimageBack->size());

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimageBack->draw(imagedrawing);

      }

      recta.add(rectangleX);

   }


   void visual_effect::Constraint001(int_point & point, const ::int_rectangle * lpcrectIn,
   const ::int_rectangle * lpcrectOut, double & direction)
   {

      ::int_rectangle rectangleIn(*lpcrectIn);

      if(rectangleIn.contains(point))
      {

         auto pointCenter = rectangleIn.center();

         direction  += 180 + ((int) direction  % 360);

         if(point.x > pointCenter.x)
         {
            point.x = rectangleIn.right();
         }
         else
         {
            point.x = rectangleIn.left();
         }

         if(point.y > pointCenter.y)
         {
            point.y = rectangleIn.bottom();
         }
         else
         {
            point.y = rectangleIn.left();
         }
      }

      if(point.x > lpcrectOut->right())
      {
         direction  += 180 + ((int) direction  % 360);
      }
      else if(point.y > lpcrectOut->bottom())
      {
         direction  += 180 + ((int) direction  % 360);
      }
      else if(point.x < lpcrectOut->left())
      {
         direction  += 180 + ((int) direction  % 360);
      }
      else if(point.y < lpcrectOut->top())
      {
         direction  += 180 + ((int) direction  % 360);
      }

      if(point.x > lpcrectOut->right())
      {
         point.x = lpcrectOut->right();
      }
      else if(point.x < lpcrectOut->left())
      {
         point.x = lpcrectOut->left();
      }

      if(point.y > lpcrectOut->bottom())
      {
         point.y = lpcrectOut->bottom();
      }
      else if(point.y < lpcrectOut->top())
      {
         point.y = lpcrectOut->top();
      }

   }


   void visual_effect::RenderRotateEx4(int_rectangle_array & recta)
   {
      
      main * pmain = helper_get_main();

      transition_effect * ptransitioneffect = pmain->get_transition_effect();

      gcom * pgcom = pmain->get_gcom();

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
         m_color = pimage1->GetPixel(
                         pimage1->width() / 2,
                         pimage1->height() / 2);
      }



      ::int_rectangle rectangleX = m_tool1.m_rectangleX;
      m_tool1.cx() = rectangleX.width();
      m_tool1.cy() = rectangleX.height();
      
      //auto psystem = m_papplication;

      //class imaging & imaging = psystem->imaging();


      ASSERT(pgcom->GetBufferDC()->get_os_data() != nullptr);

      ::image::image_pointer pimageBack = pgcom->get_image(e_image_back);
      ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
      ::image::image_pointer pimage2 = pgcom->get_image(e_image_temp2);
      ::image::image_pointer pimage3 = pgcom->get_image(e_image_temp3);

      int cx1 = pimage1->width();
      int cy1 = pimage1->height();

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         pimage2->create({cx1, cy1});
         pimage3->create({cx1, cy1});
      }


//         int s1 = rand() * 1 / RAND_MAX + 1;
//         int s2 = rand() * 1 / RAND_MAX + 1;
//         int s3 = rand() * 2 / RAND_MAX + 1;
//         int s4 = rand() * 2 / RAND_MAX + 1;

      ::int_rectangle rcTrackBox;

      rcTrackBox.left()   = 1;
      rcTrackBox.top()    = 1;
      rcTrackBox.right()  = cx1 - 1 - rcTrackBox.left();
      rcTrackBox.bottom() = cy1 - 1 - rcTrackBox.top();

      ::int_rectangle rcTrackBox2;

      rcTrackBox2.left()   = 0;
      rcTrackBox2.top()    = 0;
      rcTrackBox2.right()  = cx1 - 1 - rcTrackBox2.left();
      rcTrackBox2.bottom() = cy1 - 1 - rcTrackBox2.top();

      /*for(int i = 0; i < m_colortrack1a.get_size(); i++)
      {
         color_track1 & ct1 = m_colortrack1a[i];
         ct1.m_rndgrowVelocity.Step();
         TrackDirection001(
            ct1.m_dDirection,
            ct1.m_dDirectionAddUp,
            ct1.m_rndgrowDirectionLatency);
         ct1.m_dCos = ::cos(ct1.m_dDirection * M_PI / 180.0);
         ct1.m_dSin = ::sin(ct1.m_dDirection * M_PI / 180.0);
         ct1.m_pointColorTrack.x += abs(ct1.m_rndgrowVelocity.i) * ct1.m_dCos;
         ct1.m_pointColorTrack.y += abs(ct1.m_rndgrowVelocity.i) * ct1.m_dSin;
         Constraint001(
            ct1.m_pointColorTrack,
            rcTrackBox,
            rcTrackBox2,
            ct1.m_dDirection);
         ct1.m_dSize = rand() * 5 / RAND_MAX + 1;
      }*/

      StepGrow001(
      m_color,
      m_iGrowColor,
      m_iGrowMax);

      StepGrow001(
      m_color2,
      m_iGrowColor2,
      m_iGrowMax2);

      StepGrow001(
      m_color3,
      m_iGrowColor3,
      m_iGrowMax3);

      StepGrow001(
      m_color4,
      m_iGrowColor4,
      m_iGrowMax4);

      ::color::color colorStrong1 = m_color;
      ::color::color colorStrong3 = m_color2;
      ::color::color colorStrong5 = m_color3;
      ::color::color colorStrong7 = m_color4;
      ::color::color colorStrong2;
      ::color::color colorStrong4;
      ::color::color colorStrong6;
      ::color::color colorStrong8;

      {
         double dH;
         double dL;
         double dS;
         colorStrong1.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.8;
         colorStrong1.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong1.get_hls(dH, dL, dS);
         dL = 0.3;
         dS = 0.1;
         colorStrong2.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong3.get_hls(dH, dL, dS);
         dL = 0.9;
         dS = 0.7;
         colorStrong3.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong3.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.9;
         colorStrong4.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong5.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.8;
         colorStrong5.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong5.get_hls(dH, dL, dS);
         dL = 0.5;
         dS = 0.9;
         colorStrong6.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong7.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.8;
         colorStrong7.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong7.get_hls(dH, dL, dS);
         dL = 0.5;
         dS = 0.9;
         colorStrong8.set_hls(dH, dL, dS);
      }


//            argb(255, rand() * 255 / RAND_MAX,
//            rand() * 255 / RAND_MAX,
//            rand() * 255 / RAND_MAX);

      unsigned char bA = 200;


#ifdef WINDOWS_DESKTOP
      //GdiFlush();
#endif

      //double dmin = minimum(cx, cy);
      //double dmax = maximum(cx, cy);abs

      if(m_bGrowColor0)
      {
         //pimage1->get_graphics()->fill_rectangle(0, 0, cx1, cy1, colorStrong1.get_rgb() | (255 << 24));
      }

      // ::draw2d::graphics_pointer pimage3->get_graphics();
      //pimage3->get_graphics()->CreateCompatibleDC(nullptr);
      //pimage3->get_graphics()->set(pimage3->m_hbitmap);

      //   pimage3->stretch(imageBuffer);

      //   pimage3->Paste(pimage1);

//         int c1 = rand() * 3 / RAND_MAX;
//         int c2 = rand() * 3 / RAND_MAX;
//         int c4 = rand() * 10 / RAND_MAX + 1;


      m_rndgrowAngleOffsetLatency.Step();
      if(m_rndgrowAngleOffsetLatency.i == 1)
      {
         m_dAngleOffsetAddUp = (5.1 * rand() / RAND_MAX) - 2.05;
      }
      double dOffset3 = (1.0 * rand() / RAND_MAX) - .5;
      m_dAngleOffset += minimum(10.0, minimum(2.0, m_dAngleOffsetAddUp + dOffset3));
      m_dAngleOffset = fmod(m_dAngleOffset, 360.0);

//         double dAngleOffset = m_dAngleOffset;
      double dAngle;
      if(m_rndgrowAngleOffsetLatency.i > 50)
      {
         //dAngle = 10.111 + dAngleOffset;
         dAngle = m_dAngleOffset;
      }
      else
      {

         //dAngle = -10.111 - dAngleOffset;
         dAngle = -m_dAngleOffset;
      }
      pimage2->rotate(pimage1, radians(dAngle), 1.09);
      //pimage2->rotate(pimage1, dAngle, 1.009);
      //pimage2->Paste(pimage1);
      //pimage1->g()->fill_rectangle({ cxm - s1, cym - s2, s3, s4, m_color & ::opacity(bA));


      // top
      /*   pimage1->g()->fill_rectangle({ 0, 0, cx1 / 2, 1, colorStrong1 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ cx1 / 2, 0, cx1 / 2 - 1, 1, colorStrong2 & ::opacity(bA));
         // left
         pimage1->g()->fill_rectangle({ 0, 0, 1, cy1 / 2, colorStrong3 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ 0, cy1 / 2, 1, cy1 / 2 - 1, colorStrong4 & ::opacity(bA));
         // bottom
         pimage1->g()->fill_rectangle({ 0, cy1 - 1, cx1 / 2, 1, colorStrong5 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ cx1 / 2, cy1 - 1, cx1 / 2 - 1, 1, colorStrong6 & ::opacity(bA));
         // right
         pimage1->g()->fill_rectangle({ cx1 - 1, 0, 1, cy1 / 2, colorStrong7 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ cx1 - 1, cy1 / 2, 1, cy1 / 2 - 1, colorStrong8 & ::opacity(bA));*/

      // top1
      auto ppen1 = øcreate < ::draw2d::pen > ();
      ppen1->create_solid(1, colorStrong1.opaque());
      pimage1->get_graphics()->set(ppen1);
      int margin = 10;
      int w = margin * 2;

      ::int_point pointLast(margin, margin);
      ::int_point pointNow;
      //pimage1->get_graphics()->set_current_point(margin, margin);

      int imax = cx1 - margin;
      int jmax = cx1 / 2;
      int rmax = jmax * RAND_MAX;
      int rhalf = RAND_MAX / 2;
      int j = 0;
      int r1 = 0;
      int r2 = 0;
      int i;
      for(i = margin; i < jmax; i++)
      {
         if((i % 5) == 0)
         {
            r1 = (rand() - rhalf) * j * 3 / rmax;
         }
         if((i % 7) == 0)
         {
            r2 = (rand() - rhalf) * j * 5 / rmax;
         }
         pointNow.x=i;
         pointNow.y = r1 + r2 + margin + ((rand() - rhalf) * j * w / rmax);
         pimage1->get_graphics()->line(pointLast, pointNow);
         pointLast = pointNow;
         j++;
      }
      for(; i < imax; i++)
      {
         if((i % 5) == 0)
         {
            r1 = (rand() - rhalf) * j * 3 / rmax;
         }
         if((i % 7) == 0)
         {
            r2 = (rand() - rhalf) * j * 5 / rmax;
         }
         pointNow.x = i;
         pointNow.y = r1 + r2 + margin + ((rand() - rhalf) * j * w / rmax);
         pimage1->get_graphics()->line(pointLast, pointNow);
         pointLast = pointNow;
         j--;
      }

      pointLast.x =margin;
      pointLast.y =cy1-margin;
      //pimage1->get_graphics()->set_current_point(margin, cy1 - margin);

      for(i = margin; i < jmax; i++)
      {
         pointNow.x = i;
         pointNow.y = cy1 - margin + ((rand() - rhalf) * j * w / rmax);
         pimage1->get_graphics()->line(pointLast, pointNow);
         pointLast = pointNow;
         j++;
      }
      for(; i < imax; i++)
      {
         pointNow.x = i;
         pointNow.y = cy1 - margin + ((rand() - rhalf) * j * w / rmax);
         pimage1->get_graphics()->line(pointLast, pointNow );
         pointLast = pointNow;
         j--;
      }

      pointLast.x = margin;
      pointLast.y = margin;
      //pimage1->get_graphics()->set_current_point(margin, margin);

      // vertical
      imax = cy1 - margin;
      jmax = cy1 / 2;
      rmax = jmax * RAND_MAX;
      rhalf = RAND_MAX / 2;
      j = 0;
      r1 = 0;
      r2 = 0;
      for(i = margin; i < jmax; i++)
      {
         if((i % 5) == 0)
         {
            r1 = (rand() - rhalf) * j * 3 / rmax;
         }
         if((i % 7) == 0)
         {
            r2 = (rand() - rhalf) * j * 5 / rmax;
         }
         pointNow.x = r1 + r2 + margin + ((rand() - rhalf) * j * w / rmax);
         pointNow.y = i;
         pimage1->get_graphics()->line(pointLast, pointNow);
         pointLast = pointNow;
         j++;
      }
      for(; i < imax; i++)
      {
         if((i % 5) == 0)
         {
            r1 = (rand() - rhalf) * j * 3 / rmax;
         }
         if((i % 7) == 0)
         {
            r2 = (rand() - rhalf) * j * 5 / rmax;
         }
         pointNow.x = r1 + r2 + margin + ((rand() - rhalf) * j * w / rmax);
         pointNow.y = i;
         pimage1->get_graphics()->line(pointLast, pointNow);
         pointLast=pointNow;
         j--;
      }

      pointLast.x=cx1-margin;
      pointLast.y = margin;
      //pimage1->get_graphics()->set_current_point(cx1 - margin, margin);

      for(i = margin; i < jmax; i++)
      {
         pointNow.x=cx1 - margin + ((rand() - rhalf) * j * w / rmax);
            pointNow.y=i;
         pimage1->get_graphics()->line(pointLast, pointNow);
         pointLast=pointNow;
         j++;
      }
      for(; i < imax; i++)
      {
         pointNow.x=cx1 - margin + ((rand() - rhalf) * j * w / rmax);
         pointNow.y=i;
         pimage1->get_graphics()->line(pointLast, pointNow);
         pointLast=pointNow;
         j--;
      }

      //pimage1->g()->fill_rectangle({ 0, cy1 * 2 / 3, cx1 / 2, 1, , bA);
      //pimage1->g()->fill_rectangle({ cx1 / 2, cy1 * 2 / 3, cx1 / 2 - 1, 1, colorStrong2 & ::opacity(bA));

      {
         int x2 = cx1 / 2;
         int y2 = cy1 / 2;
         int w2 = cx1 / 2 - 1;
         int h2 = cy1 / 2 - 1;

         // top
         pimage1->g()->fill_rectangle({ 0, 0, w2, 1 }, colorStrong2 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ x2, 0, w2, 1 }, colorStrong2 & ::opacity(bA));
         // left
         pimage1->g()->fill_rectangle({ 0,  0, 1, h2 }, colorStrong2 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ 0, y2, 1, h2 }, colorStrong2 & ::opacity(bA));
         // bottom
         pimage1->g()->fill_rectangle({ 0, cy1 - 1, w2, 1 }, colorStrong2 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ x2, cy1 - 1, w2, 1 }, colorStrong2 & ::opacity(bA));
         // right
         pimage1->g()->fill_rectangle({ cx1 - 1, 0, 1, h2 }, colorStrong2 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ cx1 - 1, y2, 1, h2 }, colorStrong2 & ::opacity(bA));
      }

      /*   {
            ::draw2d::pen_pointer ppen1(get_app(), PS_SOLID, 0, colorStrong1.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush1(get_app(), colorStrong1.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen2(get_app(), PS_SOLID, 0, colorStrong2.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush2(get_app(), colorStrong2.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen3(get_app(), PS_SOLID, 0, colorStrong3.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush3(get_app(), colorStrong3.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen4(get_app(), PS_SOLID, 0, colorStrong4.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush4(get_app(), colorStrong4.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen5(get_app(), PS_SOLID, 0, colorStrong5.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush5(get_app(), colorStrong5.get_rgb() | (255 << 24));
            for(int i = 0; i < m_colortrack1a.get_size(); i++)
            {
               color_track1 & ct1 = m_colortrack1a[i];
               int x1 = ct1.m_pointColorTrack.x;
               int y1 = ct1.m_pointColorTrack.y;
               int d = ct1.m_dSize;
               switch(i % 5)
               {
               case 0:
                  {
                     pimage1->get_graphics()->set(brush1);
                     pimage1->get_graphics()->set(ppen1);
                  }
                  break;
               case 1:
                  {
                     pimage1->get_graphics()->set(brush2);
                     pimage1->get_graphics()->set(ppen2);
                  }
                  break;
               case 2:
                  {
                     pimage1->get_graphics()->set(brush3);
                     pimage1->get_graphics()->set(ppen3);
                  }
               case 3:
                  {
                     pimage1->get_graphics()->set(brush4);
                     pimage1->get_graphics()->set(ppen4);
                  }
               case 4:
                  {
                     pimage1->get_graphics()->set(brush5);
                     pimage1->get_graphics()->set(ppen5);
                  }
                  break;
               }
               pimage1->get_graphics()->rectangle(x1, y1, x1 + d, y1 + d);
            }

         }*/



      /*imaging.bitmap_blend(
         pimage1->get_graphics(),
         0, 0, cx1, cy1,
         pimage2->get_graphics(),
         0, 0,
         50);*/

      pimage2->channel_multiply(10.0 / 255.0, ::color::e_channel_opacity);

      {

         ::image::image_source imagesource(pimage2);

         auto rectangle = double_rectangle_dimension(0, 0, cx1, cy1);

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimage1->get_graphics()->draw(imagedrawing);

      }

      pimageBack->g()->set_interpolation_mode(VISUAL_EFFECT_STRETCH_MODE);

      {

         ::image::image_source imagesource(pimage1);

         double_rectangle rectangle(pimageBack->size());

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimageBack->draw(imagedrawing);

      }

      recta.add(rectangleX);
   }




   void visual_effect::RenderRain1(int_rectangle_array & recta)
   {
      
      main * pmain = helper_get_main();

      transition_effect * ptransitioneffect = pmain->get_transition_effect();

      gcom * pgcom = pmain->get_gcom();

      ASSERT(pgcom->GetBufferDC()->get_os_data() != nullptr);

      ::image::image_pointer pimageBack = pgcom->get_image(e_image_back);
      ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
      ::image::image_pointer pimage2 = pgcom->get_image(e_image_temp2);
      ::image::image_pointer pimage3 = pgcom->get_image(e_image_temp3);
      ::image::image_pointer pimage4 = pgcom->get_image(e_image_temp4);
      ::image::image_pointer pimageBuffer = pgcom->get_image(e_image_buffer);
      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         ::image::image_pointer pimageFrame1 = pgcom->get_image(e_image_frame1);
         m_tool1.m_dMod = 4.2;
         m_tool1.m_dMinSize = 0;
         int cx1 = m_tool1.cx() / 4;
         int cy1 = m_tool1.cy() / 4;
         pimage1->create({cx1, cy1});
         pimage2->create({cx1, cy1});
         pimage3->create({cx1, cy1});
         pimage4->create({cx1, cy1});
         m_tool1.m_dAlpha = 0;
         m_water.create(cx1, cy1, pimageBack->color_indexes());
         pimage4->get_graphics()->set_interpolation_mode(::draw2d::e_interpolation_mode_high_quality_bicubic);

         {

            ::image::image_source imagesource(pimageFrame1);

            double_rectangle rectangle(pimage4->size());

            ::image::image_drawing_options imagedrawingoptions(rectangle);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

            pimage4->draw(imagedrawing);

         }

         pimage3->get_graphics()->set_interpolation_mode(::draw2d::e_interpolation_mode_high_quality_bicubic);

         {

            ::image::image_source imagesource(pimageBuffer);

            double_rectangle rectangle(pimage3->size());

            ::image::image_drawing_options imagedrawingoptions(rectangle);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

            pimage3->draw(imagedrawing);

         }

      }
      int cx1 = pimage1->width();
      int cy1 = pimage1->height();



      //pimage4->get_graphics()->BitBlt(0, 0, cx1, cy1, pimage3->get_graphics(), 0, 0);


      ::collection::index iReturnPoint = ptransitioneffect->m_ptool001->m_iStepCount * 5 / 21;

      if(ptransitioneffect->m_ptool001->m_iStep >= iReturnPoint * 2)
      {

         ::image::image_source imagesource(pimage3);

         double_rectangle rectangle(pimage1->size());

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         imagedrawingoptions.opacity(10);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimage4->draw(imagedrawing);

      }

      pimage4->fill_channel(0, ::color::e_channel_opacity);
      pimage2->fill_channel(0, ::color::e_channel_opacity);

      m_water.to(pimage4->get_data(), pimage2->get_data());

      pimage4->fill_channel(255, ::color::e_channel_opacity);
      pimage2->fill_channel(255, ::color::e_channel_opacity);

      int mod = maximum(1, (int) m_tool1.m_dMod);
      if((m_tool1.m_iFrame % mod) == 0)
      {
         int x = rand() * cx1 / RAND_MAX;
         int y = rand() * cy1 / RAND_MAX;
         int r = (int) (rand() * minimum(m_tool1.m_dMinSize, 60.0) / RAND_MAX + minimum(m_tool1.m_dMinSize / 2, 60.0));
         int h = rand() * 100 / RAND_MAX;
         if(ptransitioneffect->m_ptool001->m_iStep < iReturnPoint)
         {
            m_tool1.m_dMinSize += 0.5;
         }
         else
         {
            m_tool1.m_dMinSize -= 0.5;
         }
         m_water.HeightBlob(x, y,
                            r, h, m_tool1.m_iPage);
      }

      if(m_tool1.m_dMinSize > 20 || m_tool1.m_dAlpha > 0)
         m_tool1.m_dAlpha += 0.5;
      if(ptransitioneffect->m_ptool001->m_iStep < iReturnPoint)
      {
         m_tool1.m_dMod -= 0.01;
         m_tool1.m_iFrame++;
      }
      else
      {
         m_tool1.m_dMod += 0.01;
         m_tool1.m_iFrame--;
      }
      m_tool1.m_iPage ^= 1;

      pimageBack->g()->set_interpolation_mode(VISUAL_EFFECT_STRETCH_MODE);

      pimageBack->stretch_image(pimage2);

      recta.add(m_tool1.m_rectangleX);

   }


   void visual_effect::RenderRotateEx5(int_rectangle_array & recta)
   {

      main * pmain = helper_get_main();

      transition_effect * ptransitioneffect = pmain->get_transition_effect();

      gcom * pgcom = pmain->get_gcom();

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
         m_color = pimage1->GetPixel(
                         pimage1->width() / 2,
                         pimage1->height() / 2);
      }

      ::int_rectangle rectangleX = m_tool1.m_rectangleX;
      m_tool1.cx() = rectangleX.width();
      m_tool1.cy() = rectangleX.height();
      
      //auto psystem = m_papplication;

      //class imaging & imaging = psystem->imaging();


      ASSERT(pgcom->GetBufferDC()->get_os_data() != nullptr);

      ::image::image_pointer pimageBack = pgcom->get_image(e_image_back);
      ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
      ::image::image_pointer pimage2 = pgcom->get_image(e_image_temp2);
      ::image::image_pointer pimage3 = pgcom->get_image(e_image_temp3);
      ::image::image_pointer pimage4 = pgcom->get_image(e_image_temp4);

      int cx1 = pimage1->width();
      int cy1 = pimage1->height();

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         if(m_pointa.get_size() != cx1)
         {
            m_pointa.set_size(cx1);
            m_pointa1.set_size(cx1);
            m_pointa2.set_size(cx1);

            for(int i = 0; i < cx1; i++)
            {
               m_pointa[i].y = cy1 / 2;
               m_pointa1[i].y = 0;
               m_pointa1[i].y = 0;
            }
         }
      }

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         pimage2->create({cx1, cy1});
         pimage3->create({cx1, cy1});
         pimage4->create({ m_tool1.cx(), m_tool1.cy() });
      }

      if(m_iVisual == VisualEffectRotateEx6)
      {
         pimage2->fill_byte(0);
         pimage3->fill_byte(0);
      }

      StepGrow034(
      m_color,
      m_iGrowColor,
      m_iGrowMax);

      ::color::color colorStrong1 = m_color;

      {
         double dH;
         double dL;
         double dS;
         colorStrong1.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.8;
         colorStrong1.set_hls(dH, dL, dS);
      }

      auto ppen1 = øcreate < ::draw2d::pen > ();
      ppen1->create_solid(1, colorStrong1.opaque());
      auto ppenWhite = øcreate < ::draw2d::pen > ();
      ppenWhite->create_solid(3, argb(255, 205, 205, 205));
      int margin1 = cx1 / 8;
      int margin2 = cy1 / 2;
      int w = cy1 / 3;

      double jmul = (double) cy1 / (cx1 * 2.0);

      double delta = w;


      int imax = cx1 - margin1;
      int jmax = cx1 / 2;
      int rmax = jmax * RAND_MAX;
      double rhalf = RAND_MAX / 2.0;
      int j = 0;
//         int r1 = 0;
//         int r2 = 0;


      double offset;

      double jres;
      int i;
      for(i = margin1; i < jmax; i++)
      {
         /*if((i % 5) == 0)
         {
             r1 = (rand() - rhalf) * j * 1 / rmax;
         }
         if((i % 7) == 0)
         {
            r2 = (rand() - rhalf) * j * 2 / rmax;
         }*/
         m_pointa[i].x = i;
         //offset = r1 + r2 + ((rand() - rhalf) * j * delta / rmax);
         jres = j * jmul;
         offset = (rand() - rhalf) * j * delta / rmax;
         m_pointa[i].y = (int) maximum(margin2 - jres, minimum(margin2 + jres, m_pointa[i - 1].y + offset));
         m_pointa[i].y = (m_pointa[i].y + margin2) / 2;
         j++;
      }
      j = 0;
      for(i = imax - 1; i >= jmax; i--)
      {
         /*if((i % 5) == 0)
         {
            r1 = (rand() - rhalf) * j * 1 / rmax;
         }
         if((i % 7) == 0)
         {
            r2 = (rand() - rhalf) * j * 2 / rmax;
         }*/
         m_pointa[i].x = i;
         //offset = r1 + r2 + ((rand() - rhalf) * j * delta / rmax);
         jres = j * jmul;
         offset = ((rand() - rhalf) * j * delta / rmax);
         m_pointa[i].y = (int) maximum(margin2 - jres, minimum(margin2 + jres, m_pointa[i + 1].y + offset));
         m_pointa[i].y = (m_pointa[i].y + margin2) / 2;
         j++;
      }

      //bool bWhite;
      //bWhite = false;
      if(m_iVisual == VisualEffectRotateEx5)
      {
         pimage1->get_graphics()->set(ppenWhite);
         for(i = margin1 + 1; i < imax; i++)
         {
            pimage1->get_graphics()->line(m_pointa[i-1], m_pointa[i]);
         }
      }
      pimage1->get_graphics()->set(ppen1);
      for(i = margin1+1; i < imax; i++)
      {
         pimage1->get_graphics()->line(m_pointa[i-1], m_pointa[i]);
      }

      pimage1->g()->fill_rectangle({ 0, 0, cx1, cy1 }, argb(10, 0, 0, 0));

#ifdef WINDOWS_DESKTOP
      //GdiFlush();
#endif


      if(m_iVisual == VisualEffectRotateEx6)
      {
         KEEP(pmain->get_user_interaction()->m_bTransferVoid);

         pimage4->get_graphics()->set_interpolation_mode(::draw2d::e_interpolation_mode_high_quality_bicubic);

         {

            ::image::image_source imagesource(pimage1);

            double_rectangle rectangle(pimage4->size());

            ::image::image_drawing_options imagedrawingoptions(rectangle);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

            pimage4->draw(imagedrawing);

         }

         pmain->get_user_interaction()->backimpact_feedback(pimage4);
         pimage1->get_graphics()->set_interpolation_mode(::draw2d::e_interpolation_mode_high_quality_bicubic);

         {

            ::image::image_source imagesource(pimage4);

            double_rectangle rectangle(pimage1->size());

            ::image::image_drawing_options imagedrawingoptions(rectangle);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

            pimage1->draw(imagedrawing);

         }

      }



      m_rndgrowAngleOffsetLatency.Step();
      if(m_rndgrowAngleOffsetLatency.i == 0)
      {
         m_dAngleOffsetAddUp = (10.0 * rand() / RAND_MAX) - 5.0;
      }

      m_dAngleOffset = m_dAngleOffsetAddUp;
      m_dAngleOffset = fmod(m_dAngleOffset, 360.0);

      double dAngleOffset = m_dAngleOffset;
      double dAngle;
      if(m_iGrowColor >= 0)
      {
         dAngle = 1.511 + dAngleOffset;
      }
      else
      {

         dAngle = -1.511 - dAngleOffset;
      }
      pimage2->Rotate034(pimage1, dAngle, 0.95);

      pimage2->channel_multiply(127.0 / 255.0, ::color::e_channel_opacity);

      {

         ::image::image_source imagesource(pimage2);

         auto rectangle = double_rectangle_dimension(0, 0, cx1, cy1);

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimage1->get_graphics()->draw(imagedrawing);

      }

      pimageBack->g()->set_interpolation_mode(VISUAL_EFFECT_STRETCH_MODE);

      {

         ::image::image_source imagesource(pimage1);

         double_rectangle rectangle(pimageBack->size());

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimageBack->draw(imagedrawing);

      }

      recta.add(rectangleX);

   }


   void visual_effect::RenderRotateEx7(int_rectangle_array & recta)
   {

      main * pmain = helper_get_main();

      transition_effect * ptransitioneffect = pmain->get_transition_effect();

      gcom * pgcom = pmain->get_gcom();

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
         m_color = pimage1->GetPixel(
                         pimage1->width() / 2,
                         pimage1->height() / 2);
      }



      ::int_rectangle rectangleX = m_tool1.m_rectangleX;
      m_tool1.cx() = rectangleX.width();
      m_tool1.cy() = rectangleX.height();
      //class imaging & imaging = psystem->imaging();


      ASSERT(pgcom->GetBufferDC()->get_os_data() != nullptr);

      ::image::image_pointer pimageBack = pgcom->get_image(e_image_back);
      ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
      ::image::image_pointer pimage2 = pgcom->get_image(e_image_temp2);
      ::image::image_pointer pimage3 = pgcom->get_image(e_image_temp3);

      int cx1 = pimage1->width();
      int cy1 = pimage1->height();

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         if(m_pointa.get_size() != cx1)
         {
            m_pointa.set_size(cx1);
            m_pointa1.set_size(cx1);
            m_pointa2.set_size(cx1);

            for(int i = 0; i < cx1; i++)
            {
               m_pointa[i].y = cy1 / 2;
               m_pointa1[i].y = 0;
               m_pointa1[i].y = 0;
            }
         }
      }

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         pimage2->create({cx1, cy1});
         pimage3->create({cx1, cy1});
      }


//         int s1 = rand() * 1 / RAND_MAX + 1;
//         int s2 = rand() * 1 / RAND_MAX + 1;
//         int s3 = rand() * 2 / RAND_MAX + 1;
//         int s4 = rand() * 2 / RAND_MAX + 1;

      ::int_rectangle rcTrackBox;

      rcTrackBox.left()   = 1;
      rcTrackBox.top()    = 1;
      rcTrackBox.right()  = cx1 - 1 - rcTrackBox.left();
      rcTrackBox.bottom() = cy1 - 1 - rcTrackBox.top();

      ::int_rectangle rcTrackBox2;

      rcTrackBox2.left()   = 0;
      rcTrackBox2.top()    = 0;
      rcTrackBox2.right()  = cx1 - 1 - rcTrackBox2.left();
      rcTrackBox2.bottom() = cy1 - 1 - rcTrackBox2.top();

      /*for(int i = 0; i < m_colortrack1a.get_size(); i++)
      {
         color_track1 & ct1 = m_colortrack1a[i];
         ct1.m_rndgrowVelocity.Step();
         TrackDirection001(
            ct1.m_dDirection,
            ct1.m_dDirectionAddUp,
            ct1.m_rndgrowDirectionLatency);
         ct1.m_dCos = ::cos(ct1.m_dDirection * M_PI / 180.0);
         ct1.m_dSin = ::sin(ct1.m_dDirection * M_PI / 180.0);
         ct1.m_pointColorTrack.x += abs(ct1.m_rndgrowVelocity.i) * ct1.m_dCos;
         ct1.m_pointColorTrack.y += abs(ct1.m_rndgrowVelocity.i) * ct1.m_dSin;
         Constraint001(
            ct1.m_pointColorTrack,
            rcTrackBox,
            rcTrackBox2,
            ct1.m_dDirection);
         ct1.m_dSize = rand() * 5 / RAND_MAX + 1;
      }*/

      StepGrow034(
      m_color,
      m_iGrowColor,
      m_iGrowMax);

      /*StepGrow001(
         m_color2,
         m_iGrowColor2,
         m_iGrowMax2);

      StepGrow001(
         m_color3,
         m_iGrowColor3,
         m_iGrowMax3);

      StepGrow001(
         m_color4,
         m_iGrowColor4,
         m_iGrowMax4);*/

      ::color::color colorStrong1 = m_color;
      ::color::color colorStrong3 = m_color2;
      ::color::color colorStrong5 = m_color3;
      ::color::color colorStrong7 = m_color4;
      ::color::color colorStrong2;
      ::color::color colorStrong4;
      ::color::color colorStrong6;
      ::color::color colorStrong8;

      {
         double dH;
         double dL;
         double dS;
         colorStrong1.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.8;
         colorStrong1.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong1.get_hls(dH, dL, dS);
         dL = 0.3;
         dS = 0.1;
         colorStrong2.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong3.get_hls(dH, dL, dS);
         dL = 0.9;
         dS = 0.7;
         colorStrong3.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong3.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.9;
         colorStrong4.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong5.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.8;
         colorStrong5.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong5.get_hls(dH, dL, dS);
         dL = 0.5;
         dS = 0.9;
         colorStrong6.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong7.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.8;
         colorStrong7.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong7.get_hls(dH, dL, dS);
         dL = 0.5;
         dS = 0.9;
         colorStrong8.set_hls(dH, dL, dS);
      }

//
//            argb(255, rand() * 255 / RAND_MAX,
//            rand() * 255 / RAND_MAX,
//            rand() * 255 / RAND_MAX);

//         unsigned char bA = 200;


#ifdef WINDOWS_DESKTOP
      //GdiFlush();
#endif

      //double dmin = minimum(cx, cy);
      //double dmax = maximum(cx, cy);abs

      if(m_bGrowColor0)
      {
         //pimage1->get_graphics()->fill_rectangle(0, 0, cx1, cy1, colorStrong1.get_rgb() | (255 << 24));
      }

      // ::draw2d::graphics_pointer pimage3->get_graphics();
      //pimage3->get_graphics()->CreateCompatibleDC(nullptr);
      //pimage3->get_graphics()->set(pimage3->m_hbitmap);

      //   pimage3->stretch(imageBuffer);

      //   pimage3->Paste(pimage1);

//         int c1 = rand() * 3 / RAND_MAX;
//         int c2 = rand() * 3 / RAND_MAX;
//         int c4 = rand() * 10 / RAND_MAX + 1;
















      /*   m_rndgrowAngleOffsetLatency.Step();
      if(m_rndgrowAngleOffsetLatency.i == 1)
      {
         m_dAngleOffsetAddUp = (5.1 * rand() / RAND_MAX) - 2.05;
      }
      double dOffset3 = (1.0 * rand() / RAND_MAX) - .5;
      m_dAngleOffset += minimum(10.0, minimum(2.0, m_dAngleOffsetAddUp + dOffset3));
      m_dAngleOffset = fmod(m_dAngleOffset, 360.0);

      double dAngleOffset = m_dAngleOffset;
      double dAngle;
      if(m_rndgrowAngleOffsetLatency.i > 50)
      {
         //dAngle = 10.111 + dAngleOffset;
         dAngle = m_dAngleOffset;
      }
      else
      {

         //dAngle = -10.111 - dAngleOffset;
         dAngle = -m_dAngleOffset;
      }
      pimage2->rotate(pimage1, dAngle, 0.95);*/
























      //pimage2->rotate(pimage1, dAngle, 1.009);
      //pimage2->Paste(pimage1);
      //pimage1->g()->fill_rectangle({ cxm - s1, cym - s2, s3, s4, m_color & ::opacity(bA));


      // top
      /*   pimage1->g()->fill_rectangle({ 0, 0, cx1 / 2, 1, colorStrong1 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ cx1 / 2, 0, cx1 / 2 - 1, 1, colorStrong2 & ::opacity(bA));
         // left
         pimage1->g()->fill_rectangle({ 0, 0, 1, cy1 / 2, colorStrong3 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ 0, cy1 / 2, 1, cy1 / 2 - 1, colorStrong4 & ::opacity(bA));
         // bottom
         pimage1->g()->fill_rectangle({ 0, cy1 - 1, cx1 / 2, 1, colorStrong5 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ cx1 / 2, cy1 - 1, cx1 / 2 - 1, 1, colorStrong6 & ::opacity(bA));
         // right
         pimage1->g()->fill_rectangle({ cx1 - 1, 0, 1, cy1 / 2, colorStrong7 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ cx1 - 1, cy1 / 2, 1, cy1 / 2 - 1, colorStrong8 & ::opacity(bA));*/

      // top1

      /*
      auto ppen1 = øcreate < ::draw2d::pen > ();
      ppen1->create_solid(1, colorStrong1.get_rgb() | (255 << 24));
      auto ppenWhite = øcreate < ::draw2d::pen > ();
      ppenWhite->create_solid(3, argb(255, 205, 205, 205));
      int margin1 = 10;
         int margin2 = cy1 / 2;
      int w = cy1 / 3;

      int delta = 5;


      int imax = cx1 - margin1;
      int jmax = cx1 / 2;
      int rmax = jmax * RAND_MAX;
      int rhalf = RAND_MAX / 2;
      int j = 0;
      int r1 = 0;
      int r2 = 0;


      int offset;
      int i;
      for(i = margin1; i < jmax; i++)
      {
         if((i % 5) == 0)
         {
            r1 = (rand() - rhalf) * j * 1 / rmax;
         }
         if((i % 7) == 0)
         {
            r2 = (rand() - rhalf) * j * 2 / rmax;
         }
         m_pointa[i].x = i;
         offset = r1 + r2 + ((rand() - rhalf) * j * delta / rmax);
         m_pointa[i].y = maximum(- j * w, minimum( j * w, m_pointa[i].y + offset));
         j++;
      }
      for(; i < imax; i++)
      {
         if((i % 5) == 0)
         {
            r1 = (rand() - rhalf) * j * 1 / rmax;
         }
         if((i % 7) == 0)
         {
            r2 = (rand() - rhalf) * j * 2 / rmax;
         }
         m_pointa[i].x = i;
         offset = r1 + r2 + ((rand() - rhalf) * j * delta / rmax);
         m_pointa[i].y = maximum(- j * w, minimum( j * w, m_pointa[i].y + offset));
         j--;
      }
      pimage1->get_graphics()->set(ppenWhite);
      pimage1->get_graphics()->set_current_point(m_pointa[margin1].x, m_pointa[margin1].y);
      for(i = margin1; i < imax; i++)
      {
         pimage1->get_graphics()->line_to(m_pointa[i].x, m_pointa[i].y);
      }
      pimage1->get_graphics()->set(ppen1);
      pimage1->get_graphics()->set_current_point(m_pointa[margin1].x, m_pointa[margin1].y);
      for(i = margin1; i < imax; i++)
      {
         pimage1->get_graphics()->line_to(m_pointa[i].x, m_pointa[i].y);
      }

      pimage1->g()->fill_rectangle({ 0, 0, cx1, cy1, argb(255, 34, 34, 34), 10);

         pimage1->get_graphics()->set_current_point(margin1, cy1 - margin2);

         for(i = margin1; i < jmax; i++)
         {
            pimage1->get_graphics()->line_to(i, cy1 - margin2 + ((rand() - rhalf) * j * w / rmax));
            j++;
         }
         for(; i < imax; i++)
         {
            pimage1->get_graphics()->line_to(i, cy1 - margin2 + ((rand() - rhalf) * j * w / rmax));
            j--;
         }

         pimage1->get_graphics()->set_current_point(margin2, margin1);

         // vertical
         imax = cy1 - margin1;
         jmax = cy1 / 2;
         rmax = jmax * RAND_MAX;
         rhalf = RAND_MAX / 2;
         j = 0;
         r1 = 0;
         r2 = 0;
         for(i = margin1; i < jmax; i++)
         {
            if((i % 5) == 0)
            {
               r1 = (rand() - rhalf) * j * 3 / rmax;
            }
            if((i % 7) == 0)
            {
               r2 = (rand() - rhalf) * j * 5 / rmax;
            }
            pimage1->get_graphics()->line_to(r1 + r2 + margin2 + ((rand() - rhalf) * j * w / rmax), i);
            j++;
         }
         for(; i < imax; i++)
         {
            if((i % 5) == 0)
            {
               r1 = (rand() - rhalf) * j * 3 / rmax;
            }
            if((i % 7) == 0)
            {
               r2 = (rand() - rhalf) * j * 5 / rmax;
            }
            pimage1->get_graphics()->line_to(r1 + r2 + margin2 + ((rand() - rhalf) * j * w / rmax), i);
            j--;
         }

         pimage1->get_graphics()->set_current_point(cx1 - margin2, margin1);

         for(i = margin1; i < jmax; i++)
         {
            pimage1->get_graphics()->line_to(cx1 - margin2 + ((rand() - rhalf) * j * w / rmax), i);
            j++;
         }
         for(; i < imax; i++)
         {
            pimage1->get_graphics()->line_to(cx1 - margin2 + ((rand() - rhalf) * j * w / rmax), i);
            j--;
         }*/

      //pimage1->g()->fill_rectangle({ 0, cy1 * 2 / 3, cx1 / 2, 1, , bA);
      //pimage1->g()->fill_rectangle({ cx1 / 2, cy1 * 2 / 3, cx1 / 2 - 1, 1, colorStrong2 & ::opacity(bA));

      // top
      /* pimage1->g()->fill_rectangle({ 0, 0, cx1 / 2, 1, colorStrong2 & ::opacity(bA));
       pimage1->g()->fill_rectangle({ cx1 / 2, 0, cx1 / 2 - 1, 1, colorStrong2 & ::opacity(bA));
       // left
       pimage1->g()->fill_rectangle({ 0, 0, 1, cy1 / 2, colorStrong2 & ::opacity(bA));
       pimage1->g()->fill_rectangle({ 0, cy1 / 2, 1, cy1 / 2 - 1, colorStrong2 & ::opacity(bA));
       // bottom
       pimage1->g()->fill_rectangle({ 0, cy1 - 1, cx1 / 2, 1, colorStrong2 & ::opacity(bA));
       pimage1->g()->fill_rectangle({ cx1 / 2, cy1 - 1, cx1 / 2 - 1, 1, colorStrong2 & ::opacity(bA));
       // right
       pimage1->g()->fill_rectangle({ cx1 - 1, 0, 1, cy1 / 2, colorStrong2 & ::opacity(bA));
       pimage1->g()->fill_rectangle({ cx1 - 1, cy1 / 2, 1, cy1 / 2 - 1, colorStrong2 & ::opacity(bA));


      //   pimage1->get_graphics()->fill_rectangle(0, 0, cx1 / 2, cy1 / 2, 0);*/
      /*   {
            ::draw2d::pen_pointer ppen1(get_app(), PS_SOLID, 0, colorStrong1.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush1(get_app(), colorStrong1.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen2(get_app(), PS_SOLID, 0, colorStrong2.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush2(get_app(), colorStrong2.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen3(get_app(), PS_SOLID, 0, colorStrong3.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush3(get_app(), colorStrong3.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen4(get_app(), PS_SOLID, 0, colorStrong4.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush4(get_app(), colorStrong4.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen5(get_app(), PS_SOLID, 0, colorStrong5.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush5(get_app(), colorStrong5.get_rgb() | (255 << 24));
            for(int i = 0; i < m_colortrack1a.get_size(); i++)
            {
               color_track1 & ct1 = m_colortrack1a[i];
               int x1 = ct1.m_pointColorTrack.x;
               int y1 = ct1.m_pointColorTrack.y;
               int d = ct1.m_dSize;
               switch(i % 5)
               {
               case 0:
                  {
                     pimage1->get_graphics()->set(brush1);
                     pimage1->get_graphics()->set(ppen1);
                  }
                  break;
               case 1:
                  {
                     pimage1->get_graphics()->set(brush2);
                     pimage1->get_graphics()->set(ppen2);
                  }
                  break;
               case 2:
                  {
                     pimage1->get_graphics()->set(brush3);
                     pimage1->get_graphics()->set(ppen3);
                  }
               case 3:
                  {
                     pimage1->get_graphics()->set(brush4);
                     pimage1->get_graphics()->set(ppen4);
                  }
               case 4:
                  {
                     pimage1->get_graphics()->set(brush5);
                     pimage1->get_graphics()->set(ppen5);
                  }
                  break;
               }
               pimage1->get_graphics()->rectangle(x1, y1, x1 + d, y1 + d);
            }

         }*/

      m_rndgrowAngleOffsetLatency.Step();
      if(m_rndgrowAngleOffsetLatency.i == 0)
      {
         m_dAngleOffsetAddUp = (10.0 * rand() / RAND_MAX) - 5.0;
      }
      //m_dAngleOffset += m_dAngleOffsetAddUp;
      m_dAngleOffset = m_dAngleOffsetAddUp;
      m_dAngleOffset = fmod(m_dAngleOffset, 360.0);

      double dAngleOffset = m_dAngleOffset;
      double dAngle;
      if(m_iGrowColor >= 0)
      {
         dAngle = 1.511 + dAngleOffset;
      }
      else
      {

         dAngle = -1.511 - dAngleOffset;
      }
      pimage2->Rotate034(pimage1, dAngle, 0.95);


      /*imaging.bitmap_blend(
         pimage1->get_graphics(),
         0, 0, cx1, cy1,
         pimage2->get_graphics(),
         0, 0,
         50);*/

      pimage2->channel_multiply(127.0 / 255.0, ::color::e_channel_opacity);

      {

         ::image::image_source imagesource(pimage2);

         auto rectangle = double_rectangle_dimension(0, 0, cx1, cy1);

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimage1->get_graphics()->draw(imagedrawing);

      }

      pimageBack->g()->set_interpolation_mode(VISUAL_EFFECT_STRETCH_MODE);

      {

         ::image::image_source imagesource(pimage1);

         double_rectangle rectangle(pimageBack->size());

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimageBack->draw(imagedrawing);

      }

      recta.add(rectangleX);
   }





   void visual_effect::RenderAlphaPixelExplosion(int_rectangle_array & recta)
   {
      main * pmain = helper_get_main();

      transition_effect * ptransitioneffect = pmain->get_transition_effect();
      gcom * pgcom = pmain->get_gcom();

      ASSERT(pgcom->GetBufferDC()->get_os_data() != nullptr);

      ::draw2d::graphics_pointer dcBuffer = pgcom->GetBufferDC();
      ::image::image_pointer pimageBack = pgcom->get_image(e_image_back);
      ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
      ::image::image_pointer pimage2 = pgcom->get_image(e_image_temp2);
      ::image::image_pointer pimage3 = pgcom->get_image(e_image_temp3);
      ::image::image_pointer pimage4 = pgcom->get_image(e_image_temp4);
      ::image::image_pointer pimageBuffer = pgcom->get_image(e_image_buffer);
      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         ::image::image_pointer pimageFrame1 = pgcom->get_image(e_image_frame1);

         m_tool1.m_dMod = 4.2;
         m_tool1.m_dMinSize = 0;
         int cx1 = m_tool1.cx() / 4;
         int cy1 = m_tool1.cy() / 4;
         pimage1->create({cx1, cy1});
         pimage2->create({cx1, cy1});
         pimage3->create({cx1, cy1});
         pimage4->create({cx1, cy1});
         m_tool1.m_dAlpha = 0;

         m_explosion.m_density = 1.5;
         m_explosion.m_minradius = 0.0;
         m_explosion.create(cx1, cy1, pimage1->scan_size(), pimage1->color_indexes());
         m_explosion.Reset();

         pimage1->get_graphics()->set_interpolation_mode(::draw2d::e_interpolation_mode_high_quality_bicubic);

         {

            ::image::image_source imagesource(pimageFrame1);

            double_rectangle rectangle(pimage1->size());

            ::image::image_drawing_options imagedrawingoptions(rectangle);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

            pimage1->draw(imagedrawing);

         }

         pimage3->get_graphics()->set_interpolation_mode(::draw2d::e_interpolation_mode_high_quality_bicubic);
         pimage3->clear(::color::transparent);
         pimage4->get_graphics()->set_interpolation_mode(::draw2d::e_interpolation_mode_high_quality_bicubic);

         {

            ::image::image_source imagesource(pimageBuffer);

            double_rectangle rectangle(pimage4->size());

            ::image::image_drawing_options imagedrawingoptions(rectangle);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

            pimage4->draw(imagedrawing);

         }


      }

      int cx1 = pimage1->width();
      int cy1 = pimage1->height();



      ::collection::index iReturnPoint = ptransitioneffect->m_ptool001->m_iStepCount * 5 / 21;

      m_explosion.to(pimage4->get_data(), pimage3->get_data());

      /*int mod = maximum(1, (int) m_tool1.m_dMod);
      if((m_tool1.m_iFrame % mod) == 0)
      {
         int x = rand() * cx1 / RAND_MAX;
         int y = rand() * cy1 / RAND_MAX;
         int r = rand() * minimum(m_tool1.m_dMinSize, 60.0) / RAND_MAX + minimum(m_tool1.m_dMinSize / 2, 60.0);
         int h = rand() * 100 / RAND_MAX;
         if(ptransitioneffect->m_ptool001->m_iStep < iReturnPoint)
         {
            m_tool1.m_dMinSize += 0.5;
         }
         else
         {
            m_tool1.m_dMinSize -= 0.5;
         }
         m_water.HeightBlob(x, y,
                     r, h, m_tool1.m_iPage);
      }*/

      //m_explostion.CalcPixelExplosion(0, 0);


      if(m_tool1.m_dMinSize > 20 || m_tool1.m_dAlpha > 0)
         m_tool1.m_dAlpha += 0.5;
      if(ptransitioneffect->m_ptool001->m_iStep < iReturnPoint)
      {
         m_tool1.m_dMod -= 0.001;
         m_tool1.m_iFrame++;
      }
      else
      {
         m_tool1.m_dMod += 0.001;
         m_tool1.m_iFrame--;
      }
      m_tool1.m_iPage ^= 1;
      //dcBack->fill_rectangle(0, 0, cx, cy, argb(255, 0, 0, 0));
      //dcBack->fill_rectangle(100, 100, cx, cy, argb(255, 255, 255, 255));
      //pimageBack->stretch(::double_rectangle(pimage3->size()), pimage2);

      //pimage2->channel_multiply(::color::e_channel_opacity, 50.0 / 255.0);

      {

         ::image::image_source imagesource(pimage3);

         double_rectangle rectangle(::long_long_size(cx1, cy1));

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         imagedrawingoptions.opacity(25);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimage1->draw(imagedrawing);

      }

      pimageBack->g()->set_interpolation_mode(VISUAL_EFFECT_STRETCH_MODE);

      {

         ::image::image_source imagesource(pimage1);

         double_rectangle rectangle(pimageBack->size());

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimageBack->draw(imagedrawing);

      }

      recta.add(m_tool1.m_rectangleX);

   }


   void visual_effect::RenderPixelExplosion(int_rectangle_array & recta)
   {
      main * pmain = helper_get_main();

      transition_effect * ptransitioneffect = pmain->get_transition_effect();
      gcom * pgcom = pmain->get_gcom();

      ASSERT(pgcom->GetBufferDC()->get_os_data() != nullptr);

      ::image::image_pointer pimageBack = pgcom->get_image(e_image_back);
      ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
      ::image::image_pointer pimage2 = pgcom->get_image(e_image_temp2);
      ::image::image_pointer pimage3 = pgcom->get_image(e_image_temp3);
      ::image::image_pointer pimage4 = pgcom->get_image(e_image_temp4);
      ::image::image_pointer pimageBuffer = pgcom->get_image(e_image_buffer);
      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
//            ::image::image_pointer pimageFrame1 = pgcom->get_image(e_image_frame1);

         m_tool1.m_dMod = 4.2;
         m_tool1.m_dMinSize = 0;
         int cx1 = m_tool1.cx() / 4;
         int cy1 = m_tool1.cy() / 4;
         pimage1->create({cx1, cy1});
         pimage2->create({cx1, cy1});
         pimage3->create({cx1, cy1});
         pimage4->create({cx1, cy1});
         m_tool1.m_dAlpha = 0;

         m_explosion.m_density = 1.5;
         m_explosion.m_minradius = 0.0;
         m_explosion.create(cx1, cy1, pimage1->scan_size(), pimage1->color_indexes());
         m_explosion.Reset();

         pimage1->get_graphics()->fill_rectangle(::double_rectangle_dimension(0, 0, cx1, cy1), argb(0, 0, 0, 0));
         pimage3->get_graphics()->set_interpolation_mode(::draw2d::e_interpolation_mode_high_quality_bicubic);

         {

            ::image::image_source imagesource(pimageBuffer);

            double_rectangle rectangle(pimage3->size());

            ::image::image_drawing_options imagedrawingoptions(rectangle);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

            pimage3->draw(imagedrawing);

         }


      }

      {

         ::image::image_source imagesource(pimage3);

         double_rectangle rectangle(pimage1->size());

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimage4->get_graphics()->draw(imagedrawing);


      }

      ::image::image_source imagesource(pimage1);

      double_rectangle rectangle(pimage1->size());

      ::image::image_drawing_options imagedrawingoptions(rectangle);

      imagedrawingoptions.opacity(maximum(minimum(255, m_tool1.m_dAlpha), 0));

      ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);
      
      //imagedrawing.set(, pimage1);



//      pimage4->draw(imagedrawing);
//      psystem->imaging().bitmap_blend(
//      pimage4->get_graphics(),
//      ::int_point(), ,
//      pimage1->get_graphics(),
//      ::int_point(),
//      (unsigned char) );

      ::collection::index iReturnPoint = ptransitioneffect->m_ptool001->m_iStepCount * 5 / 21;

      m_explosion.to(pimage4->get_data(), pimage2->get_data());

      /*int mod = maximum(1, (int) m_tool1.m_dMod);
      if((m_tool1.m_iFrame % mod) == 0)
      {
         int x = rand() * cx1 / RAND_MAX;
         int y = rand() * cy1 / RAND_MAX;
         int r = rand() * minimum(m_tool1.m_dMinSize, 60.0) / RAND_MAX + minimum(m_tool1.m_dMinSize / 2, 60.0);
         int h = rand() * 100 / RAND_MAX;
         if(ptransitioneffect->m_ptool001->m_iStep < iReturnPoint)
         {
            m_tool1.m_dMinSize += 0.5;
         }
         else
         {
            m_tool1.m_dMinSize -= 0.5;
         }
         m_water.HeightBlob(x, y,
                     r, h, m_tool1.m_iPage);
      }*/

      //   CalcPixelExplosion(0, 0);


      if(m_tool1.m_dMinSize > 20 || m_tool1.m_dAlpha > 0)
         m_tool1.m_dAlpha += 0.5;
      if(ptransitioneffect->m_ptool001->m_iStep < iReturnPoint)
      {
         m_tool1.m_dMod -= 0.01;
         m_tool1.m_iFrame++;
      }
      else
      {
         m_tool1.m_dMod += 0.01;
         m_tool1.m_iFrame--;
      }
      m_tool1.m_iPage ^= 1;
      //dcBack->fill_rectangle(0, 0, cx, cy, argb(255, 0, 0, 0));
      //dcBack->fill_rectangle(100, 100, cx, cy, argb(255, 255, 255, 255));

      pimageBack->g()->set_interpolation_mode(VISUAL_EFFECT_STRETCH_MODE);

      {

         ::image::image_source imagesource(pimage2);

         double_rectangle rectangle(pimage3->size());

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimageBack->draw(imagedrawing);

      }

      recta.add(m_tool1.m_rectangleX);

   }


   void visual_effect::RenderPixelExplosion2(int_rectangle_array & recta)
   {

      main * pmain = helper_get_main();

      transition_effect * ptransitioneffect = pmain->get_transition_effect();
      gcom * pgcom = pmain->get_gcom();

      ASSERT(pgcom->GetBufferDC()->get_os_data() != nullptr);

      ::draw2d::graphics_pointer dcBuffer = pgcom->GetBufferDC();

      ::image::image_pointer pimageBack = pgcom->get_image(e_image_back);
      ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
      ::image::image_pointer pimage2 = pgcom->get_image(e_image_temp2);
      ::image::image_pointer pimage3 = pgcom->get_image(e_image_temp3);
      ::image::image_pointer pimage4 = pgcom->get_image(e_image_temp4);
      ::image::image_pointer pimageBuffer = pgcom->get_image(e_image_buffer);
      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         ::image::image_pointer pimageFrame1 = pgcom->get_image(e_image_frame1);

         m_tool1.m_dMod = 4.2;
         m_tool1.m_dMinSize = 0;
         int cx1 = m_tool1.cx() / 4;
         int cy1 = m_tool1.cy() / 4;
         pimage1->create({cx1, cy1});
         pimage2->create({cx1, cy1});
         pimage3->create({cx1, cy1});
         pimage4->create({cx1, cy1});
         m_tool1.m_dAlpha = 0;

         m_explosion.m_density = 1.8;
         m_explosion.m_minradius = 1.7;
         m_explosion.create(cx1, cy1, pimage1->scan_size(), pimage1->color_indexes());
         m_explosion.Reset();

         pimage1->get_graphics()->set_interpolation_mode(::draw2d::e_interpolation_mode_high_quality_bicubic);

         {

            ::image::image_source imagesource(pimageBuffer);

            double_rectangle rectangle(pimage1->size());

            ::image::image_drawing_options imagedrawingoptions(rectangle);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

            pimage1->draw(imagedrawing);

         }

         pimage3->get_graphics()->set_interpolation_mode(::draw2d::e_interpolation_mode_high_quality_bicubic);

         {

            ::image::image_source imagesource(pimageFrame1);

            double_rectangle rectangle(pimage3->size());

            ::image::image_drawing_options imagedrawingoptions(rectangle);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

            pimage3->draw(imagedrawing);

         }

      }

      //int cx1 = pimage1->width();
      //int cy1 = pimage1->height();

      pimage3->clear(::color::transparent);

      {

         ::image::image_source imagesource(pimageBuffer);

         double_rectangle rectangle(pimage4->size());

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimage4->draw(imagedrawing);

      }

#ifdef WINDOWS_DESKTOP
      //GdiFlush();
#endif

      /*psystem->imaging().bitmap_blend(
         pimage4->get_graphics(),
         0, 0, cx1, cy1,
         pimage1->get_graphics(),
         0, 0,
         (unsigned char) maximum(minimum(255, m_tool1.m_dAlpha), 0));*/

      ::collection::index iReturnPoint = ptransitioneffect->m_ptool001->m_iStepCount * 5 / 21;

      m_explosion.to(pimage4->get_data(), pimage3->get_data());

      /*int mod = maximum(1, (int) m_tool1.m_dMod);
      if((m_tool1.m_iFrame % mod) == 0)
      {
         int x = rand() * cx1 / RAND_MAX;
         int y = rand() * cy1 / RAND_MAX;
         int r = rand() * minimum(m_tool1.m_dMinSize, 60.0) / RAND_MAX + minimum(m_tool1.m_dMinSize / 2, 60.0);
         int h = rand() * 100 / RAND_MAX;
         if(ptransitioneffect->m_ptool001->m_iStep < iReturnPoint)
         {
            m_tool1.m_dMinSize += 0.5;
         }
         else
         {
            m_tool1.m_dMinSize -= 0.5;
         }
         m_water.HeightBlob(x, y,
                     r, h, m_tool1.m_iPage);
      }*/

      //m_explostion.CalcPixelExplosion(0, 0);


      if(m_tool1.m_dMinSize > 20 || m_tool1.m_dAlpha > 0)
         m_tool1.m_dAlpha += 0.5;
      if(ptransitioneffect->m_ptool001->m_iStep < iReturnPoint)
      {
         m_tool1.m_dMod -= 0.01;
         m_tool1.m_iFrame++;
      }
      else
      {
         m_tool1.m_dMod += 0.01;
         m_tool1.m_iFrame--;
      }
      m_tool1.m_iPage ^= 1;

      pimageBack->g()->set_interpolation_mode(VISUAL_EFFECT_STRETCH_MODE);

      {

         ::image::image_source imagesource(pimage3);

         double_rectangle rectangle(pimageBack->size());

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimageBack->draw(imagedrawing);

      }

      recta.add(m_tool1.m_rectangleX);

   }


   void visual_effect::RenderRotateEx8(int_rectangle_array & recta)
   {
      main * pmain = helper_get_main();

      transition_effect * ptransitioneffect = pmain->get_transition_effect();

      gcom * pgcom = pmain->get_gcom();

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
         pimage1->map();
         m_color = pimage1->GetPixel(
                         pimage1->width() / 2,
                         pimage1->height() / 2);
      }



      ::int_rectangle rectangleX = m_tool1.m_rectangleX;
      m_tool1.cx() = rectangleX.width();
      m_tool1.cy() = rectangleX.height();

      //auto psystem = m_papplication;

      //class imaging & imaging = psystem->imaging();


      ASSERT(pgcom->GetBufferDC()->get_os_data() != nullptr);

      ::image::image_pointer pimageBack = pgcom->get_image(e_image_back);
      ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
      ::image::image_pointer pimage2 = pgcom->get_image(e_image_temp2);
      ::image::image_pointer pimage3 = pgcom->get_image(e_image_temp3);

      int cx1 = pimage1->width();
      int cy1 = pimage1->height();

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         pimage2->create({cx1, cy1});
         pimage3->create({cx1, cy1});
         pimage2->clear(::color::transparent);
         pimage3->clear(::color::transparent);
      }

//      int iRand123 = rand();
//         int s1 = rand() * 1 / RAND_MAX + 1;
//         int s2 = rand() * 1 / RAND_MAX + 1;
//         int s3 = rand() * 2 / RAND_MAX + 1;
//         int s4 = rand() * 2 / RAND_MAX + 1;

      ::int_rectangle rcTrackBox;

      rcTrackBox.left()   = 1;
      rcTrackBox.top()    = 1;
      rcTrackBox.right()  = cx1 - 1 - rcTrackBox.left();
      rcTrackBox.bottom() = cy1 - 1 - rcTrackBox.top();

      ::int_rectangle rcTrackBox2;

      rcTrackBox2.left()   = 0;
      rcTrackBox2.top()    = 0;
      rcTrackBox2.right()  = cx1 - 1 - rcTrackBox2.left();
      rcTrackBox2.bottom() = cy1 - 1 - rcTrackBox2.top();

      /*for(int i = 0; i < m_colortrack1a.get_size(); i++)
      {
         color_track1 & ct1 = m_colortrack1a[i];
         ct1.m_rndgrowVelocity.Step();
         TrackDirection001(
            ct1.m_dDirection,
            ct1.m_dDirectionAddUp,
            ct1.m_rndgrowDirectionLatency);
         ct1.m_dCos = ::cos(ct1.m_dDirection * M_PI / 180.0);
         ct1.m_dSin = ::sin(ct1.m_dDirection * M_PI / 180.0);
         ct1.m_pointColorTrack.x += abs(ct1.m_rndgrowVelocity.i) * ct1.m_dCos;
         ct1.m_pointColorTrack.y += abs(ct1.m_rndgrowVelocity.i) * ct1.m_dSin;
         Constraint001(
            ct1.m_pointColorTrack,
            rcTrackBox,
            rcTrackBox2,
            ct1.m_dDirection);
         ct1.m_dSize = rand() * 5 / RAND_MAX + 1;
      }*/

      StepGrow001(
      m_color,
      m_iGrowColor,
      m_iGrowMax);

      StepGrow001(
      m_color2,
      m_iGrowColor2,
      m_iGrowMax2);

      StepGrow001(
      m_color3,
      m_iGrowColor3,
      m_iGrowMax3);

      StepGrow001(
      m_color4,
      m_iGrowColor4,
      m_iGrowMax4);

      ::color::color colorStrong1 = m_color;
      ::color::color colorStrong3 = m_color2;
      ::color::color colorStrong5 = m_color3;
      ::color::color colorStrong7 = m_color4;
      ::color::color colorStrong2;
      ::color::color colorStrong4;
      ::color::color colorStrong6;
      ::color::color colorStrong8;

      {
         double dH;
         double dL;
         double dS;
         colorStrong1.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.8;
         colorStrong1.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong1.get_hls(dH, dL, dS);
         dL = 0.3;
         dS = 0.1;
         colorStrong2.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong3.get_hls(dH, dL, dS);
         dL = 0.9;
         dS = 0.7;
         colorStrong3.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong3.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.9;
         colorStrong4.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong5.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.8;
         colorStrong5.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong5.get_hls(dH, dL, dS);
         dL = 0.5;
         dS = 0.9;
         colorStrong6.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong7.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.8;
         colorStrong7.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong7.get_hls(dH, dL, dS);
         dL = 0.5;
         dS = 0.9;
         colorStrong8.set_hls(dH, dL, dS);
      }


//            argb(255, rand() * 255 / RAND_MAX,
//            rand() * 255 / RAND_MAX,
//            rand() * 255 / RAND_MAX);

//         unsigned char bA = 200;


#ifdef WINDOWS_DESKTOP
      //GdiFlush();
#endif

      //double dmin = minimum(cx, cy);
      //double dmax = maximum(cx, cy);abs

      if(m_bGrowColor0)
      {
         //pimage1->get_graphics()->fill_rectangle(0, 0, cx1, cy1, colorStrong1.get_rgb() | (255 << 24));
      }

//         int c1 = rand() * 3 / RAND_MAX;
//         int c2 = rand() * 3 / RAND_MAX;
//         int c4 = rand() * 10 / RAND_MAX + 1;


      m_rndgrowAngleOffsetLatency.Step();
      if(m_rndgrowAngleOffsetLatency.i == 1)
      {
         m_dAngleOffsetAddUp = (5.1 * rand() / RAND_MAX) - 2.05;
      }
      double dOffset3 = (1.0 * rand() / RAND_MAX) - .5;
      m_dAngleOffset += minimum(10.0, minimum(2.0, m_dAngleOffsetAddUp + dOffset3));
      m_dAngleOffset = fmod(m_dAngleOffset, 360.0);

//         double dAngleOffset = m_dAngleOffset;
      double dAngle;
      if(m_rndgrowAngleOffsetLatency.i > 50)
      {
         //dAngle = 10.111 + dAngleOffset;
         dAngle = m_dAngleOffset;
      }
      else
      {

         //dAngle = -10.111 - dAngleOffset;
         dAngle = -m_dAngleOffset;
      }
      pimage2->Rotate034(pimage1, dAngle, 1.09);
      //pimage2->rotate(pimage1, dAngle, 1.009);
      //pimage2->Paste(pimage1);
      //pimage1->g()->fill_rectangle({ cxm - s1, cym - s2, s3, s4, m_color & ::opacity(bA));


      // top
      /*   pimage1->g()->fill_rectangle({ 0, 0, cx1 / 2, 1, colorStrong1 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ cx1 / 2, 0, cx1 / 2 - 1, 1, colorStrong2 & ::opacity(bA));
         // left
         pimage1->g()->fill_rectangle({ 0, 0, 1, cy1 / 2, colorStrong3 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ 0, cy1 / 2, 1, cy1 / 2 - 1, colorStrong4 & ::opacity(bA));
         // bottom
         pimage1->g()->fill_rectangle({ 0, cy1 - 1, cx1 / 2, 1, colorStrong5 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ cx1 / 2, cy1 - 1, cx1 / 2 - 1, 1, colorStrong6 & ::opacity(bA));
         // right
         pimage1->g()->fill_rectangle({ cx1 - 1, 0, 1, cy1 / 2, colorStrong7 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ cx1 - 1, cy1 / 2, 1, cy1 / 2 - 1, colorStrong8 & ::opacity(bA));*/

      // top1
      auto ppen1 = øcreate < ::draw2d::pen > ();
      ppen1->create_solid(1, colorStrong1.opaque());
      pimage1->get_graphics()->set(ppen1);
      int marginx = cx1 / 12;
      int marginy = cy1 * 5 / 11;
      int w = marginx * 2;
      ::int_point pointLast (marginx, marginy);
      //pimage1->get_graphics()->set_current_point(marginx, marginy);

      int imax = cx1 - marginx;
      int jmax = cx1 / 2;
      int rmax = jmax * RAND_MAX;
      int rhalf = RAND_MAX / 2;
      int j = 0;
      int r1 = 0;
      int r2 = 0;
      int i;
      for(i = marginx; i < jmax; i++)
      {
         if((i % 5) == 0)
         {
            r1 = (rand() - rhalf) * j * 3 / rmax;
         }
         if((i % 7) == 0)
         {
            r2 = (rand() - rhalf) * j * 5 / rmax;
         }
         pimage1->get_graphics()->process_line(pointLast, ::int_point(i, r1 + r2 + marginy + ((rand() - rhalf) * j * w / rmax)));
         j++;
      }
      for(; i < imax; i++)
      {
         if((i % 5) == 0)
         {
            r1 = (rand() - rhalf) * j * 3 / rmax;
         }
         if((i % 7) == 0)
         {
            r2 = (rand() - rhalf) * j * 5 / rmax;
         }
         pimage1->get_graphics()->process_line(pointLast, ::int_point(i, r1 + r2 + marginy + ((rand() - rhalf) * j * w / rmax)));
         j--;
      }

      //pimage1->get_graphics()->set_current_point(marginx, cy1 - marginy);
      pointLast.x= marginx;
      pointLast.y = cy1-marginy;

      for(i = marginx; i < jmax; i++)
      {
         pimage1->get_graphics()->process_line(pointLast, ::int_point(i, cy1 - marginy + ((rand() - rhalf) * j * w / rmax)));
         j++;
      }
      for(; i < imax; i++)
      {
         pimage1->get_graphics()->process_line(pointLast, ::int_point(i, cy1 - marginy + ((rand() - rhalf) * j * w / rmax)));
         j--;
      }

      //pimage1->get_graphics()->set_current_point(marginx, marginy);
      pointLast.x=marginx;
      pointLast.y =marginy;

      // vertical
      //   imax = cy1 - margin;
      jmax = cy1 / 2;
      rmax = jmax * RAND_MAX;
      rhalf = RAND_MAX / 2;
      j = 0;
      r1 = 0;
      r2 = 0;
      /* for(i = margin; i < jmax; i++)
       {
          if((i % 5) == 0)
          {
             r1 = (rand() - rhalf) * j * 3 / rmax;
          }
          if((i % 7) == 0)
          {
             r2 = (rand() - rhalf) * j * 5 / rmax;
          }
          pimage1->get_graphics()->line_to(r1 + r2 + marginy + ((rand() - rhalf) * j * w / rmax), i);
          j++;
       }
       for(; i < imax; i++)
       {
          if((i % 5) == 0)
          {
             r1 = (rand() - rhalf) * j * 3 / rmax;
          }
          if((i % 7) == 0)
          {
             r2 = (rand() - rhalf) * j * 5 / rmax;
          }
          pimage1->get_graphics()->line_to(r1 + r2 + marginy + ((rand() - rhalf) * j * w / rmax), i);
          j--;
       }*/

      /*
         pimage1->get_graphics()->set_current_point(cx1 - margin, margin);

         for(i = margin; i < jmax; i++)
         {
            pimage1->get_graphics()->line_to(cx1 - margin + ((rand() - rhalf) * j * w / rmax), i);
            j++;
         }
         for(; i < imax; i++)
         {
            pimage1->get_graphics()->line_to(cx1 - margin + ((rand() - rhalf) * j * w / rmax), i);
            j--;
         }*/

      //pimage1->g()->fill_rectangle({ 0, cy1 * 2 / 3, cx1 / 2, 1, , bA);
      //pimage1->g()->fill_rectangle({ cx1 / 2, cy1 * 2 / 3, cx1 / 2 - 1, 1, colorStrong2 & ::opacity(bA));

      // top
      /*pimage1->g()->fill_rectangle({ 0, 0, cx1 / 2, 1, colorStrong2 & ::opacity(bA));
      pimage1->g()->fill_rectangle({ cx1 / 2, 0, cx1 / 2 - 1, 1, colorStrong2 & ::opacity(bA));
      // left
      pimage1->g()->fill_rectangle({ 0, 0, 1, cy1 / 2, colorStrong2 & ::opacity(bA));
      pimage1->g()->fill_rectangle({ 0, cy1 / 2, 1, cy1 / 2 - 1, colorStrong2 & ::opacity(bA));
      // bottom
      pimage1->g()->fill_rectangle({ 0, cy1 - 1, cx1 / 2, 1, colorStrong2 & ::opacity(bA));
      pimage1->g()->fill_rectangle({ cx1 / 2, cy1 - 1, cx1 / 2 - 1, 1, colorStrong2 & ::opacity(bA));
      // right
      pimage1->g()->fill_rectangle({ cx1 - 1, 0, 1, cy1 / 2, colorStrong2 & ::opacity(bA));
      pimage1->g()->fill_rectangle({ cx1 - 1, cy1 / 2, 1, cy1 / 2 - 1, colorStrong2 & ::opacity(bA));*/

      pimage1->g()->fill_rectangle({ 0, 0, cx1, cy1 }, argb(10, 0, 0, 0));

      /*   {
            ::draw2d::pen_pointer ppen1(get_app(), PS_SOLID, 0, colorStrong1.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush1(get_app(), colorStrong1.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen2(get_app(), PS_SOLID, 0, colorStrong2.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush2(get_app(), colorStrong2.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen3(get_app(), PS_SOLID, 0, colorStrong3.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush3(get_app(), colorStrong3.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen4(get_app(), PS_SOLID, 0, colorStrong4.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush4(get_app(), colorStrong4.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen5(get_app(), PS_SOLID, 0, colorStrong5.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush5(get_app(), colorStrong5.get_rgb() | (255 << 24));
            for(int i = 0; i < m_colortrack1a.get_size(); i++)
            {
               color_track1 & ct1 = m_colortrack1a[i];
               int x1 = ct1.m_pointColorTrack.x;
               int y1 = ct1.m_pointColorTrack.y;
               int d = ct1.m_dSize;
               switch(i % 5)
               {
               case 0:
                  {
                     pimage1->get_graphics()->set(brush1);
                     pimage1->get_graphics()->set(ppen1);
                  }
                  break;
               case 1:
                  {
                     pimage1->get_graphics()->set(brush2);
                     pimage1->get_graphics()->set(ppen2);
                  }
                  break;
               case 2:
                  {
                     pimage1->get_graphics()->set(brush3);
                     pimage1->get_graphics()->set(ppen3);
                  }
               case 3:
                  {
                     pimage1->get_graphics()->set(brush4);
                     pimage1->get_graphics()->set(ppen4);
                  }
               case 4:
                  {
                     pimage1->get_graphics()->set(brush5);
                     pimage1->get_graphics()->set(ppen5);
                  }
                  break;
               }
               pimage1->get_graphics()->rectangle(x1, y1, x1 + d, y1 + d);
            }

         }*/



      /*imaging.bitmap_blend(
         pimage1->get_graphics(),
         0, 0, cx1, cy1,
         pimage2->get_graphics(),
         0, 0,
         50);*/

      pimage2->channel_multiply(127.0 / 255.0, ::color::e_channel_opacity);

      {

         ::image::image_source imagesource(pimage2);

         auto rectangle = double_rectangle_dimension(0, 0, cx1, cy1);

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimage1->get_graphics()->draw(imagedrawing);

      }


      // call ipp here
      //pimageBack->g()->set_interpolation_mode(VISUAL_EFFECT_STRETCH_MODE);

      //pimageBack->stretch(::double_rectangle(pimageBack->size()), pimage1);

      m_presizer->do_resize(pimageBack, pimage1);

      recta.add(rectangleX);

   }


   void visual_effect::RenderExpand4(int_rectangle_array & recta)
   {
      main * pmain = helper_get_main();

      transition_effect * ptransitioneffect = pmain->get_transition_effect();

      gcom * pgcom = pmain->get_gcom();

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
         m_color = pimage1->GetPixel(
                         pimage1->width() / 2,
                         pimage1->height() / 2);
      }



      ::int_rectangle rectangleX = m_tool1.m_rectangleX;
      m_tool1.cx() = rectangleX.width();
      m_tool1.cy() = rectangleX.height();

      //auto psystem = m_papplication;

      //class imaging & imaging = psystem->imaging();


      ASSERT(pgcom->GetBufferDC()->get_os_data() != nullptr);

      ::image::image_pointer pimageBack = pgcom->get_image(e_image_back);
      ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
      ::image::image_pointer pimage2 = pgcom->get_image(e_image_temp2);
      ::image::image_pointer pimage3 = pgcom->get_image(e_image_temp3);

      int cx1 = pimage1->width();
      int cy1 = pimage1->height();

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         pimage2->create({cx1, cy1});
         pimage3->create({cx1, cy1});
      }


      ::int_rectangle rcTrackBox;

      rcTrackBox.left()   = 1;
      rcTrackBox.top()    = 1;
      rcTrackBox.right()  = cx1 - 1 - rcTrackBox.left();
      rcTrackBox.bottom() = cy1 - 1 - rcTrackBox.top();

      ::int_rectangle rcTrackBox2;

      rcTrackBox2.left()   = 0;
      rcTrackBox2.top()    = 0;
      rcTrackBox2.right()  = cx1 - 1 - rcTrackBox2.left();
      rcTrackBox2.bottom() = cy1 - 1 - rcTrackBox2.top();

      /*for(int i = 0; i < m_colortrack1a.get_size(); i++)
      {
         color_track1 & ct1 = m_colortrack1a[i];
         ct1.m_rndgrowVelocity.Step();
         TrackDirection001(
            ct1.m_dDirection,
            ct1.m_dDirectionAddUp,
            ct1.m_rndgrowDirectionLatency);
         ct1.m_dCos = ::cos(ct1.m_dDirection * M_PI / 180.0);
         ct1.m_dSin = ::sin(ct1.m_dDirection * M_PI / 180.0);
         ct1.m_pointColorTrack.x += abs(ct1.m_rndgrowVelocity.i) * ct1.m_dCos;
         ct1.m_pointColorTrack.y += abs(ct1.m_rndgrowVelocity.i) * ct1.m_dSin;
         Constraint001(
            ct1.m_pointColorTrack,
            rcTrackBox,
            rcTrackBox2,
            ct1.m_dDirection);
         ct1.m_dSize = rand() * 5 / RAND_MAX + 1;
      }*/

      StepGrow001(
      m_color,
      m_iGrowColor,
      m_iGrowMax);

      StepGrow001(
      m_color2,
      m_iGrowColor2,
      m_iGrowMax2);

      StepGrow001(
      m_color3,
      m_iGrowColor3,
      m_iGrowMax3);

      StepGrow001(
      m_color4,
      m_iGrowColor4,
      m_iGrowMax4);

      ::color::color colorStrong1 = m_color;
      ::color::color colorStrong3 = m_color2;
      ::color::color colorStrong5 = m_color3;
      ::color::color colorStrong7 = m_color4;
      ::color::color colorStrong2;
      ::color::color colorStrong4;
      ::color::color colorStrong6;
      ::color::color colorStrong8;

      {
         double dH;
         double dL;
         double dS;
         colorStrong1.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.8;
         colorStrong1.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong1.get_hls(dH, dL, dS);
         dL = 0.3;
         dS = 0.1;
         colorStrong2.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong3.get_hls(dH, dL, dS);
         dL = 0.9;
         dS = 0.7;
         colorStrong3.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong3.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.9;
         colorStrong4.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong5.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.8;
         colorStrong5.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong5.get_hls(dH, dL, dS);
         dL = 0.5;
         dS = 0.9;
         colorStrong6.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong7.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.8;
         colorStrong7.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong7.get_hls(dH, dL, dS);
         dL = 0.5;
         dS = 0.9;
         colorStrong8.set_hls(dH, dL, dS);
      }


//            argb(255, rand() * 255 / RAND_MAX,
//            rand() * 255 / RAND_MAX,
//            rand() * 255 / RAND_MAX);

      unsigned char bA = 200;


#ifdef WINDOWS_DESKTOP
      //GdiFlush();
#endif

      //double dmin = minimum(cx, cy);
      //double dmax = maximum(cx, cy);abs

      if(m_bGrowColor0)
      {
         //pimage1->get_graphics()->fill_rectangle(0, 0, cx1, cy1, colorStrong1.get_rgb() | (255 << 24));
      }

      // ::draw2d::graphics_pointer pimage3->get_graphics();
      //pimage3->get_graphics()->CreateCompatibleDC(nullptr);
      //pimage3->get_graphics()->set(pimage3->m_hbitmap);

      //   pimage3->stretch(imageBuffer);

      //   pimage3->Paste(pimage1);

//         int c1 = rand() * 3 / RAND_MAX;
//         int c2 = rand() * 3 / RAND_MAX;
//         int c4 = rand() * 10 / RAND_MAX + 1;


      m_rndgrowAngleOffsetLatency.Step();
      if(m_rndgrowAngleOffsetLatency.i == 1)
      {
         m_dAngleOffsetAddUp = (5.1 * rand() / RAND_MAX) - 2.05;
      }
      double dOffset3 = (1.0 * rand() / RAND_MAX) - .5;
      m_dAngleOffset += minimum(10.0, minimum(2.0, m_dAngleOffsetAddUp + dOffset3));
      m_dAngleOffset = fmod(m_dAngleOffset, 360.0);

//         double dAngleOffset = m_dAngleOffset;
      //double dAngle;
      //if(m_rndgrowAngleOffsetLatency.i > 50)
      //{
      //   //dAngle = 10.111 + dAngleOffset;
      //   dAngle = m_dAngleOffset;
      //}
      //else
      //{

      //   //dAngle = -10.111 - dAngleOffset;
      //   dAngle = -m_dAngleOffset;
      //}
      //pimage2->rotate(pimage1, dAngle, 1.09);
      //pimage2->rotate(pimage1, dAngle, 1.009);
      //pimage2->Paste(pimage1);
      //pimage1->g()->fill_rectangle({ cxm - s1, cym - s2, s3, s4, m_color & ::opacity(bA));


      // top
      /*   pimage1->g()->fill_rectangle({ 0, 0, cx1 / 2, 1, colorStrong1 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ cx1 / 2, 0, cx1 / 2 - 1, 1, colorStrong2 & ::opacity(bA));
         // left
         pimage1->g()->fill_rectangle({ 0, 0, 1, cy1 / 2, colorStrong3 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ 0, cy1 / 2, 1, cy1 / 2 - 1, colorStrong4 & ::opacity(bA));
         // bottom
         pimage1->g()->fill_rectangle({ 0, cy1 - 1, cx1 / 2, 1, colorStrong5 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ cx1 / 2, cy1 - 1, cx1 / 2 - 1, 1, colorStrong6 & ::opacity(bA));
         // right
         pimage1->g()->fill_rectangle({ cx1 - 1, 0, 1, cy1 / 2, colorStrong7 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ cx1 - 1, cy1 / 2, 1, cy1 / 2 - 1, colorStrong8 & ::opacity(bA));*/

      // top1
      auto ppen1 = øcreate < ::draw2d::pen > ();
      ppen1->create_solid(1, colorStrong1.opaque());
      pimage1->get_graphics()->set(ppen1);
      int margin = 10;
      int w = margin * 2;
      ::int_point pointLast;
      pointLast.x=margin;
      pointLast.y=margin;
      //pimage1->get_graphics()->set_current_point(margin, margin);

      int imax = cx1 - margin;
      int jmax = cx1 / 2;
      int rmax = jmax * RAND_MAX;
      int rhalf = RAND_MAX / 2;
      int j = 0;
      int r1 = 0;
      int r2 = 0;
      int i;
      for(i = margin; i < jmax; i++)
      {
         if((i % 5) == 0)
         {
            r1 = (rand() - rhalf) * j * 3 / rmax;
         }
         if((i % 7) == 0)
         {
            r2 = (rand() - rhalf) * j * 5 / rmax;
         }
         pimage1->get_graphics()->process_line(pointLast, ::int_point(i, r1 + r2 + margin + ((rand() - rhalf) * j * w / rmax)));
         j++;
      }
      for(; i < imax; i++)
      {
         if((i % 5) == 0)
         {
            r1 = (rand() - rhalf) * j * 3 / rmax;
         }
         if((i % 7) == 0)
         {
            r2 = (rand() - rhalf) * j * 5 / rmax;
         }
         pimage1->get_graphics()->process_line(pointLast, ::int_point(i, r1 + r2 + margin + ((rand() - rhalf) * j * w / rmax)));
         j--;
      }

      pointLast.x=margin;
      pointLast.y = cy1-margin;
      //pimage1->get_graphics()->set_current_point(margin, cy1 - margin);

      for(i = margin; i < jmax; i++)
      {
         pimage1->get_graphics()->process_line(pointLast, ::int_point(i, cy1 - margin + ((rand() - rhalf) * j * w / rmax)));
         j++;
      }
      for(; i < imax; i++)
      {
         pimage1->get_graphics()->process_line(pointLast, ::int_point(i, cy1 - margin + ((rand() - rhalf) * j * w / rmax)));
         j--;
      }

      pointLast.x=margin;
      pointLast.y=margin;
      //pimage1->get_graphics()->set_current_point(margin, margin);

      // vertical
      imax = cy1 - margin;
      jmax = cy1 / 2;
      rmax = jmax * RAND_MAX;
      rhalf = RAND_MAX / 2;
      j = 0;
      r1 = 0;
      r2 = 0;
      for(i = margin; i < jmax; i++)
      {
         if((i % 5) == 0)
         {
            r1 = (rand() - rhalf) * j * 3 / rmax;
         }
         if((i % 7) == 0)
         {
            r2 = (rand() - rhalf) * j * 5 / rmax;
         }
         pimage1->get_graphics()->process_line(pointLast, ::int_point(r1 + r2 + margin + ((rand() - rhalf) * j * w / rmax), i));
         j++;
      }
      for(; i < imax; i++)
      {
         if((i % 5) == 0)
         {
            r1 = (rand() - rhalf) * j * 3 / rmax;
         }
         if((i % 7) == 0)
         {
            r2 = (rand() - rhalf) * j * 5 / rmax;
         }
         pimage1->get_graphics()->process_line(pointLast, ::int_point(r1 + r2 + margin + ((rand() - rhalf) * j * w / rmax), i));
         j--;
      }

      pointLast.x = cx1-margin;
      pointLast.y =margin;
      //pimage1->get_graphics()->set_current_point(cx1 - margin, margin);

      for(i = margin; i < jmax; i++)
      {
         pimage1->get_graphics()->process_line(pointLast, ::int_point(cx1 - margin + ((rand() - rhalf) * j * w / rmax), i));
         j++;
      }
      for(; i < imax; i++)
      {
         pimage1->get_graphics()->process_line(pointLast, ::int_point(cx1 - margin + ((rand() - rhalf) * j * w / rmax), i));
         j--;
      }

      //pimage1->g()->fill_rectangle({ 0, cy1 * 2 / 3, cx1 / 2, 1, , bA);
      //pimage1->g()->fill_rectangle({ cx1 / 2, cy1 * 2 / 3, cx1 / 2 - 1, 1, colorStrong2 & ::opacity(bA));

      {
         int x2 = cx1 / 2;
         int y2 = cy1 / 2;
         int w2 = cx1 / 2 - 1;
         int h2 = cy1 / 2 - 1;

         // top
         pimage1->g()->fill_rectangle({ 0, 0, w2, 1 } , colorStrong2 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ x2, 0, w2, 1 }, colorStrong2 & ::opacity(bA));
         // left
         pimage1->g()->fill_rectangle({ 0,  0, 1, h2 }, colorStrong2 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ 0, y2, 1, h2 }, colorStrong2 & ::opacity(bA));
         // bottom
         pimage1->g()->fill_rectangle({ 0, cy1 - 1, w2, 1 }, colorStrong2 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ x2, cy1 - 1, w2, 1 }, colorStrong2 & ::opacity(bA));
         // right
         pimage1->g()->fill_rectangle({ cx1 - 1, 0, 1, h2 }, colorStrong2 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ cx1 - 1, y2, 1, h2 }, colorStrong2 & ::opacity(bA));
      }

      /*   {
            ::draw2d::pen_pointer ppen1(get_app(), PS_SOLID, 0, colorStrong1.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush1(get_app(), colorStrong1.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen2(get_app(), PS_SOLID, 0, colorStrong2.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush2(get_app(), colorStrong2.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen3(get_app(), PS_SOLID, 0, colorStrong3.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush3(get_app(), colorStrong3.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen4(get_app(), PS_SOLID, 0, colorStrong4.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush4(get_app(), colorStrong4.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen5(get_app(), PS_SOLID, 0, colorStrong5.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush5(get_app(), colorStrong5.get_rgb() | (255 << 24));
            for(int i = 0; i < m_colortrack1a.get_size(); i++)
            {
               color_track1 & ct1 = m_colortrack1a[i];
               int x1 = ct1.m_pointColorTrack.x;
               int y1 = ct1.m_pointColorTrack.y;
               int d = ct1.m_dSize;
               switch(i % 5)
               {
               case 0:
                  {
                     pimage1->get_graphics()->set(brush1);
                     pimage1->get_graphics()->set(ppen1);
                  }
                  break;
               case 1:
                  {
                     pimage1->get_graphics()->set(brush2);
                     pimage1->get_graphics()->set(ppen2);
                  }
                  break;
               case 2:
                  {
                     pimage1->get_graphics()->set(brush3);
                     pimage1->get_graphics()->set(ppen3);
                  }
               case 3:
                  {
                     pimage1->get_graphics()->set(brush4);
                     pimage1->get_graphics()->set(ppen4);
                  }
               case 4:
                  {
                     pimage1->get_graphics()->set(brush5);
                     pimage1->get_graphics()->set(ppen5);
                  }
                  break;
               }
               pimage1->get_graphics()->rectangle(x1, y1, x1 + d, y1 + d);
            }

         }*/



      /*imaging.bitmap_blend(
         pimage1->get_graphics(),
         0, 0, cx1, cy1,
         pimage2->get_graphics(),
         0, 0,
         50);*/

      pimage2->channel_multiply(127.0 / 255.0, ::color::e_channel_opacity);

      {

         ::image::image_source imagesource(pimage2);

         auto rectangle = double_rectangle_dimension(0, 0, cx1, cy1);

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimage1->get_graphics()->draw(imagedrawing);


      }

      pimageBack->g()->set_interpolation_mode(VISUAL_EFFECT_STRETCH_MODE);

      {


         ::image::image_source imagesource(pimage1);

         double_rectangle rectangle(pimageBack->size());

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimageBack->draw(imagedrawing);

      }

      recta.add(rectangleX);

   }


   void visual_effect::RenderExpand5(int_rectangle_array & recta)
   {
      main * pmain = helper_get_main();

      transition_effect * ptransitioneffect = pmain->get_transition_effect();

      gcom * pgcom = pmain->get_gcom();

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
         m_color = pimage1->GetPixel(
                         pimage1->width() / 2,
                         pimage1->height() / 2);
      }

      ::int_rectangle rectangleX = m_tool1.m_rectangleX;
      m_tool1.cx() = rectangleX.width();
      m_tool1.cy() = rectangleX.height();

      //auto psystem = m_papplication;

      //class imaging & imaging = psystem->imaging();


      ASSERT(pgcom->GetBufferDC()->get_os_data() != nullptr);

      ::image::image_pointer pimageBack = pgcom->get_image(e_image_back);
      ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
      ::image::image_pointer pimage2 = pgcom->get_image(e_image_temp2);
      ::image::image_pointer pimage3 = pgcom->get_image(e_image_temp3);
      ::image::image_pointer pimage4 = pgcom->get_image(e_image_temp4);

      int cx1 = pimage1->width();
      int cy1 = pimage1->height();

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         if(m_pointa.get_size() != cx1)
         {
            m_pointa.set_size(cx1);
            m_pointa1.set_size(cx1);
            m_pointa2.set_size(cx1);

            for(int i = 0; i < cx1; i++)
            {
               m_pointa[i].y = cy1 / 2;
               m_pointa1[i].y = 0;
               m_pointa1[i].y = 0;
            }
         }
      }

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         pimage2->create({cx1, cy1});
         pimage3->create({cx1, cy1});
         pimage4->create({ m_tool1.cx(), m_tool1.cy() });
      }

      if(m_iVisual == VisualEffectExpand6)
      {
         pimage2->fill_byte(0);
         pimage3->fill_byte(0);
      }

      StepGrow034(
      m_color,
      m_iGrowColor,
      m_iGrowMax);

      ::color::color colorStrong1 = m_color;

      {
         double dH;
         double dL;
         double dS;
         colorStrong1.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.8;
         colorStrong1.set_hls(dH, dL, dS);
      }

      auto ppen1 = øcreate < ::draw2d::pen > ();
      ppen1->create_solid(1, colorStrong1.opaque());
      auto ppenWhite = øcreate < ::draw2d::pen > ();
      ppenWhite->create_solid(3, argb(255, 205, 205, 205));
      int margin1 = cx1 / 8;
      int margin2 = cy1 / 2;
      int w = cy1 / 3;

      double jmul = (double) cy1 / (cx1 * 2.0);

      double delta = w;


      int imax = cx1 - margin1;
      int jmax = cx1 / 2;
      int rmax = jmax * RAND_MAX;
      double rhalf = RAND_MAX / 2.0;
      int j = 0;
//         int r1 = 0;
//         int r2 = 0;


      double offset;

      double jres;
      int i;
      for(i = margin1; i < jmax; i++)
      {
         /*if((i % 5) == 0)
         {
             r1 = (rand() - rhalf) * j * 1 / rmax;
         }
         if((i % 7) == 0)
         {
            r2 = (rand() - rhalf) * j * 2 / rmax;
         }*/
         m_pointa[i].x = i;
         //offset = r1 + r2 + ((rand() - rhalf) * j * delta / rmax);
         jres = j * jmul;
         offset = (rand() - rhalf) * j * delta / rmax;
         m_pointa[i].y = (int) maximum(margin2 - jres, minimum(margin2 + jres, m_pointa[i - 1].y + offset));
         m_pointa[i].y = (int) (m_pointa[i].y + margin2) / 2;
         j++;
      }
      j = 0;
      for(i = imax - 1; i >= jmax; i--)
      {
         /*if((i % 5) == 0)
         {
            r1 = (rand() - rhalf) * j * 1 / rmax;
         }
         if((i % 7) == 0)
         {
            r2 = (rand() - rhalf) * j * 2 / rmax;
         }*/
         m_pointa[i].x = i;
         //offset = r1 + r2 + ((rand() - rhalf) * j * delta / rmax);
         jres = j * jmul;
         offset = ((rand() - rhalf) * j * delta / rmax);
         m_pointa[i].y = (int) maximum(margin2 - jres, minimum(margin2 + jres, m_pointa[i + 1].y + offset));
         m_pointa[i].y = (int) (m_pointa[i].y + margin2) / 2;
         j++;
      }

      //bool bWhite;
      //bWhite = false;
      if(m_iVisual == VisualEffectRotateEx5)
      {
         pimage1->get_graphics()->set(ppenWhite);
//         pimage1->get_graphics()->set_current_point(m_pointa[margin1].x, m_pointa[margin1].y);
         //for(i = margin1+1; i < imax; i++)
         //{
            pimage1->get_graphics()->polyline(&m_pointa[margin1], imax-margin1-1);
         //}
      }
      pimage1->get_graphics()->set(ppen1);
      //pimage1->get_graphics()->set_current_point(m_pointa[margin1].x, m_pointa[margin1].y);
      //for(i = margin1; i < imax; i++)
      //{
         pimage1->get_graphics()->polyline(&m_pointa[margin1], imax-margin1-1);
      //}

      pimage1->g()->fill_rectangle({ 0, 0, cx1, cy1 }, argb(10, 0, 0, 0));

#ifdef WINDOWS_DESKTOP
      //GdiFlush();
#endif

      if(m_iVisual == VisualEffectRotateEx6)
      {

         KEEP(pmain->get_user_interaction()->m_bTransferVoid);

         {

            ::image::image_source imagesource(pimage1);

            double_rectangle rectangle(pimage4->size());

            ::image::image_drawing_options imagedrawingoptions(rectangle);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

            pimage4->draw(imagedrawing);

         }

         pmain->get_user_interaction()->backimpact_feedback(pimage4);

         pimage1->get_graphics()->set_interpolation_mode(::draw2d::e_interpolation_mode_high_quality_bicubic);

         {

            ::image::image_source imagesource(pimage4);

            double_rectangle rectangle(pimage1->size());

            ::image::image_drawing_options imagedrawingoptions(rectangle);

            ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

            pimage1->draw(imagedrawing);

         }

      }



      m_rndgrowAngleOffsetLatency.Step();
      if(m_rndgrowAngleOffsetLatency.i == 0)
      {
         m_dAngleOffsetAddUp = (10.0 * rand() / RAND_MAX) - 5.0;
      }

      m_dAngleOffset = m_dAngleOffsetAddUp;
      m_dAngleOffset = fmod(m_dAngleOffset, 360.0);

      //double dAngleOffset = m_dAngleOffset;
      //double dAngle;
      //if(m_iGrowColor >= 0)
      //{
      //   dAngle = 1.511 + dAngleOffset;
      //}
      //else
      //{

      //   dAngle = -1.511 - dAngleOffset;
      //}
      //pimage2->Rotate034(pimage1, dAngle, 0.95);


      pimage2->channel_multiply(127.0 / 255.0, ::color::e_channel_opacity);

      {

         ::image::image_source imagesource(pimage2);

         auto rectangle = double_rectangle_dimension(0, 0, cx1, cy1);

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimage1->get_graphics()->draw(imagedrawing);

      }

      pimageBack->g()->set_interpolation_mode(VISUAL_EFFECT_STRETCH_MODE);

      {

         ::image::image_source imagesource(pimage1);

         double_rectangle rectangle(pimageBack->size());

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimageBack->draw(imagedrawing);

      }

      recta.add(rectangleX);

   }


   void visual_effect::RenderExpand7(int_rectangle_array & recta)
   {
      main * pmain = helper_get_main();

      transition_effect * ptransitioneffect = pmain->get_transition_effect();

      gcom * pgcom = pmain->get_gcom();

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
         m_color = pimage1->GetPixel(
                         pimage1->width() / 2,
                         pimage1->height() / 2);
      }



      ::int_rectangle rectangleX = m_tool1.m_rectangleX;
      m_tool1.cx() = rectangleX.width();
      m_tool1.cy() = rectangleX.height();

      //auto psystem = m_papplication;

      //class imaging & imaging = psystem->imaging();


      ASSERT(pgcom->GetBufferDC()->get_os_data() != nullptr);

      ::image::image_pointer pimageBack = pgcom->get_image(e_image_back);
      ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
      ::image::image_pointer pimage2 = pgcom->get_image(e_image_temp2);
      ::image::image_pointer pimage3 = pgcom->get_image(e_image_temp3);

      int cx1 = pimage1->width();
      int cy1 = pimage1->height();

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         if(m_pointa.get_size() != cx1)
         {
            m_pointa.set_size(cx1);
            m_pointa1.set_size(cx1);
            m_pointa2.set_size(cx1);

            for(int i = 0; i < cx1; i++)
            {
               m_pointa[i].y = cy1 / 2;
               m_pointa1[i].y = 0;
               m_pointa1[i].y = 0;
            }
         }
      }

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         pimage2->create({cx1, cy1});
         pimage3->create({cx1, cy1});
      }


//         int s1 = rand() * 1 / RAND_MAX + 1;
//         int s2 = rand() * 1 / RAND_MAX + 1;
//         int s3 = rand() * 2 / RAND_MAX + 1;
//         int s4 = rand() * 2 / RAND_MAX + 1;

      ::int_rectangle rcTrackBox;

      rcTrackBox.left()   = 1;
      rcTrackBox.top()    = 1;
      rcTrackBox.right()  = cx1 - 1 - rcTrackBox.left();
      rcTrackBox.bottom() = cy1 - 1 - rcTrackBox.top();

      ::int_rectangle rcTrackBox2;

      rcTrackBox2.left()   = 0;
      rcTrackBox2.top()    = 0;
      rcTrackBox2.right()  = cx1 - 1 - rcTrackBox2.left();
      rcTrackBox2.bottom() = cy1 - 1 - rcTrackBox2.top();

      /*for(int i = 0; i < m_colortrack1a.get_size(); i++)
      {
         color_track1 & ct1 = m_colortrack1a[i];
         ct1.m_rndgrowVelocity.Step();
         TrackDirection001(
            ct1.m_dDirection,
            ct1.m_dDirectionAddUp,
            ct1.m_rndgrowDirectionLatency);
         ct1.m_dCos = ::cos(ct1.m_dDirection * M_PI / 180.0);
         ct1.m_dSin = ::sin(ct1.m_dDirection * M_PI / 180.0);
         ct1.m_pointColorTrack.x += abs(ct1.m_rndgrowVelocity.i) * ct1.m_dCos;
         ct1.m_pointColorTrack.y += abs(ct1.m_rndgrowVelocity.i) * ct1.m_dSin;
         Constraint001(
            ct1.m_pointColorTrack,
            rcTrackBox,
            rcTrackBox2,
            ct1.m_dDirection);
         ct1.m_dSize = rand() * 5 / RAND_MAX + 1;
      }*/

      StepGrow034(
      m_color,
      m_iGrowColor,
      m_iGrowMax);

      /*StepGrow001(
         m_color2,
         m_iGrowColor2,
         m_iGrowMax2);

      StepGrow001(
         m_color3,
         m_iGrowColor3,
         m_iGrowMax3);

      StepGrow001(
         m_color4,
         m_iGrowColor4,
         m_iGrowMax4);*/

      ::color::color colorStrong1 = m_color;
      ::color::color colorStrong3 = m_color2;
      ::color::color colorStrong5 = m_color3;
      ::color::color colorStrong7 = m_color4;
      ::color::color colorStrong2;
      ::color::color colorStrong4;
      ::color::color colorStrong6;
      ::color::color colorStrong8;

      {
         double dH;
         double dL;
         double dS;
         colorStrong1.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.8;
         colorStrong1.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong1.get_hls(dH, dL, dS);
         dL = 0.3;
         dS = 0.1;
         colorStrong2.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong3.get_hls(dH, dL, dS);
         dL = 0.9;
         dS = 0.7;
         colorStrong3.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong3.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.9;
         colorStrong4.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong5.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.8;
         colorStrong5.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong5.get_hls(dH, dL, dS);
         dL = 0.5;
         dS = 0.9;
         colorStrong6.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong7.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.8;
         colorStrong7.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong7.get_hls(dH, dL, dS);
         dL = 0.5;
         dS = 0.9;
         colorStrong8.set_hls(dH, dL, dS);
      }


//            argb(255, rand() * 255 / RAND_MAX,
//            rand() * 255 / RAND_MAX,
//            rand() * 255 / RAND_MAX);

//         unsigned char bA = 200;


#ifdef WINDOWS_DESKTOP
      //GdiFlush();
#endif

      //double dmin = minimum(cx, cy);
      //double dmax = maximum(cx, cy);abs

      if(m_bGrowColor0)
      {
         //pimage1->get_graphics()->fill_rectangle(0, 0, cx1, cy1, colorStrong1.get_rgb() | (255 << 24));
      }

      // ::draw2d::graphics_pointer pimage3->get_graphics();
      //pimage3->get_graphics()->CreateCompatibleDC(nullptr);
      //pimage3->get_graphics()->set(pimage3->m_hbitmap);

      //   pimage3->stretch(imageBuffer);

      //   pimage3->Paste(pimage1);

//         int c1 = rand() * 3 / RAND_MAX;
//         int c2 = rand() * 3 / RAND_MAX;
//         int c4 = rand() * 10 / RAND_MAX + 1;
















      /*   m_rndgrowAngleOffsetLatency.Step();
      if(m_rndgrowAngleOffsetLatency.i == 1)
      {
         m_dAngleOffsetAddUp = (5.1 * rand() / RAND_MAX) - 2.05;
      }
      double dOffset3 = (1.0 * rand() / RAND_MAX) - .5;
      m_dAngleOffset += minimum(10.0, minimum(2.0, m_dAngleOffsetAddUp + dOffset3));
      m_dAngleOffset = fmod(m_dAngleOffset, 360.0);

      double dAngleOffset = m_dAngleOffset;
      double dAngle;
      if(m_rndgrowAngleOffsetLatency.i > 50)
      {
         //dAngle = 10.111 + dAngleOffset;
         dAngle = m_dAngleOffset;
      }
      else
      {

         //dAngle = -10.111 - dAngleOffset;
         dAngle = -m_dAngleOffset;
      }
      pimage2->rotate(pimage1, dAngle, 0.95);*/
























      //pimage2->rotate(pimage1, dAngle, 1.009);
      //pimage2->Paste(pimage1);
      //pimage1->g()->fill_rectangle({ cxm - s1, cym - s2, s3, s4, m_color & ::opacity(bA));


      // top
      /*   pimage1->g()->fill_rectangle({ 0, 0, cx1 / 2, 1, colorStrong1 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ cx1 / 2, 0, cx1 / 2 - 1, 1, colorStrong2 & ::opacity(bA));
         // left
         pimage1->g()->fill_rectangle({ 0, 0, 1, cy1 / 2, colorStrong3 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ 0, cy1 / 2, 1, cy1 / 2 - 1, colorStrong4 & ::opacity(bA));
         // bottom
         pimage1->g()->fill_rectangle({ 0, cy1 - 1, cx1 / 2, 1, colorStrong5 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ cx1 / 2, cy1 - 1, cx1 / 2 - 1, 1, colorStrong6 & ::opacity(bA));
         // right
         pimage1->g()->fill_rectangle({ cx1 - 1, 0, 1, cy1 / 2, colorStrong7 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ cx1 - 1, cy1 / 2, 1, cy1 / 2 - 1, colorStrong8 & ::opacity(bA));*/

      // top1
      auto ppen1 = øcreate < ::draw2d::pen > ();
      ppen1->create_solid(1, colorStrong1.opaque());
      auto ppenWhite = øcreate < ::draw2d::pen > ();
      ppenWhite->create_solid(3, argb(255, 205, 205, 205));
      int margin1 = 10;
//         int margin2 = cy1 / 2;
      int w = cy1 / 3;

      int delta = 5;


      int imax = cx1 - margin1;
      int jmax = cx1 / 2;
      int rmax = jmax * RAND_MAX;
      int rhalf = RAND_MAX / 2;
      int j = 0;
      int r1 = 0;
      int r2 = 0;


      int offset;
      int i;
      for(i = margin1; i < jmax; i++)
      {
         if((i % 5) == 0)
         {
            r1 = (rand() - rhalf) * j * 1 / rmax;
         }
         if((i % 7) == 0)
         {
            r2 = (rand() - rhalf) * j * 2 / rmax;
         }
         m_pointa[i].x = i;
         offset = r1 + r2 + ((rand() - rhalf) * j * delta / rmax);
         m_pointa[i].y = maximum(- j * w, minimum( j * w, m_pointa[i].y + offset));
         j++;
      }
      for(; i < imax; i++)
      {
         if((i % 5) == 0)
         {
            r1 = (rand() - rhalf) * j * 1 / rmax;
         }
         if((i % 7) == 0)
         {
            r2 = (rand() - rhalf) * j * 2 / rmax;
         }
         m_pointa[i].x = i;
         offset = r1 + r2 + ((rand() - rhalf) * j * delta / rmax);
         m_pointa[i].y = maximum(- j * w, minimum( j * w, m_pointa[i].y + offset));
         j--;
      }
      pimage1->get_graphics()->set(ppenWhite);
      //pimage1->get_graphics()->set_current_point(m_pointa[margin1].x, m_pointa[margin1].y);
      //for(i = margin1; i < imax; i++)
      //{
         pimage1->get_graphics()->polyline(&m_pointa[margin1], imax-margin1-1);
      //}
      pimage1->get_graphics()->set(ppen1);
      //pimage1->get_graphics()->set_current_point(m_pointa[margin1].x, m_pointa[margin1].y);
      //for(i = margin1; i < imax; i++)
      //{
         pimage1->get_graphics()->polyline(&m_pointa[margin1], imax-margin1-1);
      //}

      pimage1->g()->fill_rectangle({ 0, 0, cx1, cy1 }, argb(10, 34, 34, 34));

      /*
         pimage1->get_graphics()->set_current_point(margin1, cy1 - margin2);

         for(i = margin1; i < jmax; i++)
         {
            pimage1->get_graphics()->line_to(i, cy1 - margin2 + ((rand() - rhalf) * j * w / rmax));
            j++;
         }
         for(; i < imax; i++)
         {
            pimage1->get_graphics()->line_to(i, cy1 - margin2 + ((rand() - rhalf) * j * w / rmax));
            j--;
         }

         pimage1->get_graphics()->set_current_point(margin2, margin1);

         // vertical
         imax = cy1 - margin1;
         jmax = cy1 / 2;
         rmax = jmax * RAND_MAX;
         rhalf = RAND_MAX / 2;
         j = 0;
         r1 = 0;
         r2 = 0;
         for(i = margin1; i < jmax; i++)
         {
            if((i % 5) == 0)
            {
               r1 = (rand() - rhalf) * j * 3 / rmax;
            }
            if((i % 7) == 0)
            {
               r2 = (rand() - rhalf) * j * 5 / rmax;
            }
            pimage1->get_graphics()->line_to(r1 + r2 + margin2 + ((rand() - rhalf) * j * w / rmax), i);
            j++;
         }
         for(; i < imax; i++)
         {
            if((i % 5) == 0)
            {
               r1 = (rand() - rhalf) * j * 3 / rmax;
            }
            if((i % 7) == 0)
            {
               r2 = (rand() - rhalf) * j * 5 / rmax;
            }
            pimage1->get_graphics()->line_to(r1 + r2 + margin2 + ((rand() - rhalf) * j * w / rmax), i);
            j--;
         }

         pimage1->get_graphics()->set_current_point(cx1 - margin2, margin1);

         for(i = margin1; i < jmax; i++)
         {
            pimage1->get_graphics()->line_to(cx1 - margin2 + ((rand() - rhalf) * j * w / rmax), i);
            j++;
         }
         for(; i < imax; i++)
         {
            pimage1->get_graphics()->line_to(cx1 - margin2 + ((rand() - rhalf) * j * w / rmax), i);
            j--;
         }*/

      //pimage1->g()->fill_rectangle({ 0, cy1 * 2 / 3, cx1 / 2, 1, , bA);
      //pimage1->g()->fill_rectangle({ cx1 / 2, cy1 * 2 / 3, cx1 / 2 - 1, 1, colorStrong2 & ::opacity(bA));

      // top
      /* pimage1->g()->fill_rectangle({ 0, 0, cx1 / 2, 1, colorStrong2 & ::opacity(bA));
       pimage1->g()->fill_rectangle({ cx1 / 2, 0, cx1 / 2 - 1, 1, colorStrong2 & ::opacity(bA));
       // left
       pimage1->g()->fill_rectangle({ 0, 0, 1, cy1 / 2, colorStrong2 & ::opacity(bA));
       pimage1->g()->fill_rectangle({ 0, cy1 / 2, 1, cy1 / 2 - 1, colorStrong2 & ::opacity(bA));
       // bottom
       pimage1->g()->fill_rectangle({ 0, cy1 - 1, cx1 / 2, 1, colorStrong2 & ::opacity(bA));
       pimage1->g()->fill_rectangle({ cx1 / 2, cy1 - 1, cx1 / 2 - 1, 1, colorStrong2 & ::opacity(bA));
       // right
       pimage1->g()->fill_rectangle({ cx1 - 1, 0, 1, cy1 / 2, colorStrong2 & ::opacity(bA));
       pimage1->g()->fill_rectangle({ cx1 - 1, cy1 / 2, 1, cy1 / 2 - 1, colorStrong2 & ::opacity(bA));


      //   pimage1->get_graphics()->fill_rectangle(0, 0, cx1 / 2, cy1 / 2, 0);*/
      /*   {
            ::draw2d::pen_pointer ppen1(get_app(), PS_SOLID, 0, colorStrong1.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush1(get_app(), colorStrong1.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen2(get_app(), PS_SOLID, 0, colorStrong2.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush2(get_app(), colorStrong2.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen3(get_app(), PS_SOLID, 0, colorStrong3.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush3(get_app(), colorStrong3.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen4(get_app(), PS_SOLID, 0, colorStrong4.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush4(get_app(), colorStrong4.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen5(get_app(), PS_SOLID, 0, colorStrong5.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush5(get_app(), colorStrong5.get_rgb() | (255 << 24));
            for(int i = 0; i < m_colortrack1a.get_size(); i++)
            {
               color_track1 & ct1 = m_colortrack1a[i];
               int x1 = ct1.m_pointColorTrack.x;
               int y1 = ct1.m_pointColorTrack.y;
               int d = ct1.m_dSize;
               switch(i % 5)
               {
               case 0:
                  {
                     pimage1->get_graphics()->set(brush1);
                     pimage1->get_graphics()->set(ppen1);
                  }
                  break;
               case 1:
                  {
                     pimage1->get_graphics()->set(brush2);
                     pimage1->get_graphics()->set(ppen2);
                  }
                  break;
               case 2:
                  {
                     pimage1->get_graphics()->set(brush3);
                     pimage1->get_graphics()->set(ppen3);
                  }
               case 3:
                  {
                     pimage1->get_graphics()->set(brush4);
                     pimage1->get_graphics()->set(ppen4);
                  }
               case 4:
                  {
                     pimage1->get_graphics()->set(brush5);
                     pimage1->get_graphics()->set(ppen5);
                  }
                  break;
               }
               pimage1->get_graphics()->rectangle(x1, y1, x1 + d, y1 + d);
            }

         }*/

      m_rndgrowAngleOffsetLatency.Step();
      if(m_rndgrowAngleOffsetLatency.i == 0)
      {
         m_dAngleOffsetAddUp = (10.0 * rand() / RAND_MAX) - 5.0;
      }
      //m_dAngleOffset += m_dAngleOffsetAddUp;
      m_dAngleOffset = m_dAngleOffsetAddUp;
      m_dAngleOffset = fmod(m_dAngleOffset, 360.0);

      //double dAngleOffset = m_dAngleOffset;
      //double dAngle;
      //if(m_iGrowColor >= 0)
      //{
      //   dAngle = 1.511 + dAngleOffset;
      //}
      //else
      //{

      //   dAngle = -1.511 - dAngleOffset;
      //}
      //pimage2->Rotate034(pimage1, dAngle, 0.95);


      /*imaging.bitmap_blend(
         pimage1->get_graphics(),
         0, 0, cx1, cy1,
         pimage2->get_graphics(),
         0, 0,
         50);*/

      pimage2->channel_multiply(127.0 / 255.0, ::color::e_channel_opacity);

      {

         ::image::image_source imagesource(pimage1);

         auto rectangle = double_rectangle_dimension(0, 0, cx1, cy1);

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimage1->get_graphics()->draw(imagedrawing);

      }

      pimageBack->g()->set_interpolation_mode(VISUAL_EFFECT_STRETCH_MODE);

      {

         ::image::image_source imagesource(pimage1);

         double_rectangle rectangle(pimageBack->size());

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimageBack->draw(imagedrawing);


      }

      recta.add(rectangleX);

   }


   void visual_effect::RenderExpand8(int_rectangle_array & recta)
   {
      main * pmain = helper_get_main();

      transition_effect * ptransitioneffect = pmain->get_transition_effect();

      gcom * pgcom = pmain->get_gcom();

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
         m_color = pimage1->GetPixel(
                         pimage1->width() / 2,
                         pimage1->height() / 2);
      }



      ::int_rectangle rectangleX = m_tool1.m_rectangleX;
      m_tool1.cx() = rectangleX.width();
      m_tool1.cy() = rectangleX.height();

//      auto psystem = m_papplication;

      //class imaging & imaging = psystem->imaging();


      ASSERT(pgcom->GetBufferDC()->get_os_data() != nullptr);

      ::image::image_pointer pimageBack = pgcom->get_image(e_image_back);
      ::image::image_pointer pimage1 = pgcom->get_image(e_image_temp1);
      ::image::image_pointer pimage2 = pgcom->get_image(e_image_temp2);
      ::image::image_pointer pimage3 = pgcom->get_image(e_image_temp3);

      int cx1 = pimage1->width();
      int cy1 = pimage1->height();

      if(ptransitioneffect->m_ptool001->m_iStep == 1)
      {
         pimage2->create({cx1, cy1});
         pimage3->create({cx1, cy1});
      }

//         int s1 = rand() * 1 / RAND_MAX + 1;
//         int s2 = rand() * 1 / RAND_MAX + 1;
//         int s3 = rand() * 2 / RAND_MAX + 1;
//         int s4 = rand() * 2 / RAND_MAX + 1;

      ::int_rectangle rcTrackBox;

      rcTrackBox.left()   = 1;
      rcTrackBox.top()    = 1;
      rcTrackBox.right()  = cx1 - 1 - rcTrackBox.left();
      rcTrackBox.bottom() = cy1 - 1 - rcTrackBox.top();

      ::int_rectangle rcTrackBox2;

      rcTrackBox2.left()   = 0;
      rcTrackBox2.top()    = 0;
      rcTrackBox2.right()  = cx1 - 1 - rcTrackBox2.left();
      rcTrackBox2.bottom() = cy1 - 1 - rcTrackBox2.top();

      /*for(int i = 0; i < m_colortrack1a.get_size(); i++)
      {
         color_track1 & ct1 = m_colortrack1a[i];
         ct1.m_rndgrowVelocity.Step();
         TrackDirection001(
            ct1.m_dDirection,
            ct1.m_dDirectionAddUp,
            ct1.m_rndgrowDirectionLatency);
         ct1.m_dCos = ::cos(ct1.m_dDirection * M_PI / 180.0);
         ct1.m_dSin = ::sin(ct1.m_dDirection * M_PI / 180.0);
         ct1.m_pointColorTrack.x += abs(ct1.m_rndgrowVelocity.i) * ct1.m_dCos;
         ct1.m_pointColorTrack.y += abs(ct1.m_rndgrowVelocity.i) * ct1.m_dSin;
         Constraint001(
            ct1.m_pointColorTrack,
            rcTrackBox,
            rcTrackBox2,
            ct1.m_dDirection);
         ct1.m_dSize = rand() * 5 / RAND_MAX + 1;
      }*/

      StepGrow001(
      m_color,
      m_iGrowColor,
      m_iGrowMax);

      StepGrow001(
      m_color2,
      m_iGrowColor2,
      m_iGrowMax2);

      StepGrow001(
      m_color3,
      m_iGrowColor3,
      m_iGrowMax3);

      StepGrow001(
      m_color4,
      m_iGrowColor4,
      m_iGrowMax4);

      ::color::color colorStrong1 = m_color;
      ::color::color colorStrong3 = m_color2;
      ::color::color colorStrong5 = m_color3;
      ::color::color colorStrong7 = m_color4;
      ::color::color colorStrong2;
      ::color::color colorStrong4;
      ::color::color colorStrong6;
      ::color::color colorStrong8;

      {
         double dH;
         double dL;
         double dS;
         colorStrong1.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.8;
         colorStrong1.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong1.get_hls(dH, dL, dS);
         dL = 0.3;
         dS = 0.1;
         colorStrong2.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong3.get_hls(dH, dL, dS);
         dL = 0.9;
         dS = 0.7;
         colorStrong3.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong3.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.9;
         colorStrong4.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong5.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.8;
         colorStrong5.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong5.get_hls(dH, dL, dS);
         dL = 0.5;
         dS = 0.9;
         colorStrong6.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong7.get_hls(dH, dL, dS);
         dL = 0.7;
         dS = 0.8;
         colorStrong7.set_hls(dH, dL, dS);
      }
      {
         double dH;
         double dL;
         double dS;
         colorStrong7.get_hls(dH, dL, dS);
         dL = 0.5;
         dS = 0.9;
         colorStrong8.set_hls(dH, dL, dS);
      }


//            argb(255, rand() * 255 / RAND_MAX,
//            rand() * 255 / RAND_MAX,
//            rand() * 255 / RAND_MAX);

//         unsigned char bA = 200;


#ifdef WINDOWS_DESKTOP
      //GdiFlush();
#endif

      //double dmin = minimum(cx, cy);
      //double dmax = maximum(cx, cy);abs

      if(m_bGrowColor0)
      {
         //pimage1->get_graphics()->fill_rectangle(0, 0, cx1, cy1, colorStrong1.get_rgb() | (255 << 24));
      }

//         int c1 = rand() * 3 / RAND_MAX;
//         int c2 = rand() * 3 / RAND_MAX;
//         int c4 = rand() * 10 / RAND_MAX + 1;


      m_rndgrowAngleOffsetLatency.Step();
      if(m_rndgrowAngleOffsetLatency.i == 1)
      {
         m_dAngleOffsetAddUp = (5.1 * rand() / RAND_MAX) - 2.05;
      }
      double dOffset3 = (1.0 * rand() / RAND_MAX) - .5;
      m_dAngleOffset += minimum(10.0, minimum(2.0, m_dAngleOffsetAddUp + dOffset3));
      m_dAngleOffset = fmod(m_dAngleOffset, 360.0);

//         double dAngleOffset = m_dAngleOffset;
      //double dAngle;
      //if(m_rndgrowAngleOffsetLatency.i > 50)
      //{
      //   //dAngle = 10.111 + dAngleOffset;
      //   dAngle = m_dAngleOffset;
      //}
      //else
      //{

      //   //dAngle = -10.111 - dAngleOffset;
      //   dAngle = -m_dAngleOffset;
      //}
      //pimage2->Rotate034(pimage1, dAngle, 1.09);
      //pimage2->rotate(pimage1, dAngle, 1.009);
      //pimage2->Paste(pimage1);
      //pimage1->g()->fill_rectangle({ cxm - s1, cym - s2, s3, s4, m_color & ::opacity(bA));


      // top
      /*   pimage1->g()->fill_rectangle({ 0, 0, cx1 / 2, 1, colorStrong1 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ cx1 / 2, 0, cx1 / 2 - 1, 1, colorStrong2 & ::opacity(bA));
         // left
         pimage1->g()->fill_rectangle({ 0, 0, 1, cy1 / 2, colorStrong3 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ 0, cy1 / 2, 1, cy1 / 2 - 1, colorStrong4 & ::opacity(bA));
         // bottom
         pimage1->g()->fill_rectangle({ 0, cy1 - 1, cx1 / 2, 1, colorStrong5 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ cx1 / 2, cy1 - 1, cx1 / 2 - 1, 1, colorStrong6 & ::opacity(bA));
         // right
         pimage1->g()->fill_rectangle({ cx1 - 1, 0, 1, cy1 / 2, colorStrong7 & ::opacity(bA));
         pimage1->g()->fill_rectangle({ cx1 - 1, cy1 / 2, 1, cy1 / 2 - 1, colorStrong8 & ::opacity(bA));*/

      // top1
      auto ppen1 = øcreate < ::draw2d::pen > ();
      ppen1->create_solid(1, colorStrong1.opaque());
      pimage1->get_graphics()->set(ppen1);
      int marginx = cx1 / 12;
      int marginy = cy1 * 5 / 11;
      int w = marginx * 2;
      ::double_point pointLast(marginx, marginy);
      //pimage1->get_graphics()->set_current_point(marginx, marginy);

      int imax = cx1 - marginx;
      int jmax = cx1 / 2;
      int rmax = jmax * RAND_MAX;
      int rhalf = RAND_MAX / 2;
      int j = 0;
      int r1 = 0;
      int r2 = 0;
      int i;
      for(i = marginx; i < jmax; i++)
      {
         if((i % 5) == 0)
         {
            r1 = (rand() - rhalf) * j * 3 / rmax;
         }
         if((i % 7) == 0)
         {
            r2 = (rand() - rhalf) * j * 5 / rmax;
         }
         pimage1->get_graphics()->process_line(pointLast, ::double_point(i, r1 + r2 + marginy + ((rand() - rhalf) * j * w / rmax)));
         j++;
      }
      for(; i < imax; i++)
      {
         if((i % 5) == 0)
         {
            r1 = (rand() - rhalf) * j * 3 / rmax;
         }
         if((i % 7) == 0)
         {
            r2 = (rand() - rhalf) * j * 5 / rmax;
         }
         pimage1->get_graphics()->process_line(pointLast, ::double_point(i, r1 + r2 + marginy + ((rand() - rhalf) * j * w / rmax)));
         j--;
      }

      pointLast.x = marginx;
      pointLast.y = cy1-marginy;
      //pimage1->get_graphics()->set_current_point(marginx, cy1 - marginy);

      for(i = marginx; i < jmax; i++)
      {
         pimage1->get_graphics()->process_line(pointLast, ::double_point(i, cy1 - marginy + ((rand() - rhalf) * j * w / rmax)));
         j++;
      }
      for(; i < imax; i++)
      {
         pimage1->get_graphics()->process_line(pointLast, ::double_point(i, cy1 - marginy + ((rand() - rhalf) * j * w / rmax)));
         j--;
      }

      //pimage1->get_graphics()->set_current_point(marginx, marginy);

      pointLast.x= marginx;
      pointLast.y = marginy;

      // vertical
      //   imax = cy1 - margin;
      jmax = cy1 / 2;
      rmax = jmax * RAND_MAX;
      rhalf = RAND_MAX / 2;
      j = 0;
      r1 = 0;
      r2 = 0;
      /* for(i = margin; i < jmax; i++)
       {
          if((i % 5) == 0)
          {
             r1 = (rand() - rhalf) * j * 3 / rmax;
          }
          if((i % 7) == 0)
          {
             r2 = (rand() - rhalf) * j * 5 / rmax;
          }
          pimage1->get_graphics()->line_to(r1 + r2 + marginy + ((rand() - rhalf) * j * w / rmax), i);
          j++;
       }
       for(; i < imax; i++)
       {
          if((i % 5) == 0)
          {
             r1 = (rand() - rhalf) * j * 3 / rmax;
          }
          if((i % 7) == 0)
          {
             r2 = (rand() - rhalf) * j * 5 / rmax;
          }
          pimage1->get_graphics()->line_to(r1 + r2 + marginy + ((rand() - rhalf) * j * w / rmax), i);
          j--;
       }*/

      /*
         pimage1->get_graphics()->set_current_point(cx1 - margin, margin);

         for(i = margin; i < jmax; i++)
         {
            pimage1->get_graphics()->line_to(cx1 - margin + ((rand() - rhalf) * j * w / rmax), i);
            j++;
         }
         for(; i < imax; i++)
         {
            pimage1->get_graphics()->line_to(cx1 - margin + ((rand() - rhalf) * j * w / rmax), i);
            j--;
         }*/

      //pimage1->g()->fill_rectangle({ 0, cy1 * 2 / 3, cx1 / 2, 1, , bA);
      //pimage1->g()->fill_rectangle({ cx1 / 2, cy1 * 2 / 3, cx1 / 2 - 1, 1, colorStrong2 & ::opacity(bA));

      // top
      /*pimage1->g()->fill_rectangle({ 0, 0, cx1 / 2, 1, colorStrong2 & ::opacity(bA));
      pimage1->g()->fill_rectangle({ cx1 / 2, 0, cx1 / 2 - 1, 1, colorStrong2 & ::opacity(bA));
      // left
      pimage1->g()->fill_rectangle({ 0, 0, 1, cy1 / 2, colorStrong2 & ::opacity(bA));
      pimage1->g()->fill_rectangle({ 0, cy1 / 2, 1, cy1 / 2 - 1, colorStrong2 & ::opacity(bA));
      // bottom
      pimage1->g()->fill_rectangle({ 0, cy1 - 1, cx1 / 2, 1, colorStrong2 & ::opacity(bA));
      pimage1->g()->fill_rectangle({ cx1 / 2, cy1 - 1, cx1 / 2 - 1, 1, colorStrong2 & ::opacity(bA));
      // right
      pimage1->g()->fill_rectangle({ cx1 - 1, 0, 1, cy1 / 2, colorStrong2 & ::opacity(bA));
      pimage1->g()->fill_rectangle({ cx1 - 1, cy1 / 2, 1, cy1 / 2 - 1, colorStrong2 & ::opacity(bA));*/

      pimage1->g()->fill_rectangle({ 0, 0, cx1, cy1 }, argb(10, 0, 0, 0));

      /*   {
            ::draw2d::pen_pointer ppen1(get_app(), PS_SOLID, 0, colorStrong1.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush1(get_app(), colorStrong1.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen2(get_app(), PS_SOLID, 0, colorStrong2.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush2(get_app(), colorStrong2.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen3(get_app(), PS_SOLID, 0, colorStrong3.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush3(get_app(), colorStrong3.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen4(get_app(), PS_SOLID, 0, colorStrong4.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush4(get_app(), colorStrong4.get_rgb() | (255 << 24));
            ::draw2d::pen_pointer ppen5(get_app(), PS_SOLID, 0, colorStrong5.get_rgb() | (255 << 24));
            ::draw2d::brush_pointer brush5(get_app(), colorStrong5.get_rgb() | (255 << 24));
            for(int i = 0; i < m_colortrack1a.get_size(); i++)
            {
               color_track1 & ct1 = m_colortrack1a[i];
               int x1 = ct1.m_pointColorTrack.x;
               int y1 = ct1.m_pointColorTrack.y;
               int d = ct1.m_dSize;
               switch(i % 5)
               {
               case 0:
                  {
                     pimage1->get_graphics()->set(brush1);
                     pimage1->get_graphics()->set(ppen1);
                  }
                  break;
               case 1:
                  {
                     pimage1->get_graphics()->set(brush2);
                     pimage1->get_graphics()->set(ppen2);
                  }
                  break;
               case 2:
                  {
                     pimage1->get_graphics()->set(brush3);
                     pimage1->get_graphics()->set(ppen3);
                  }
               case 3:
                  {
                     pimage1->get_graphics()->set(brush4);
                     pimage1->get_graphics()->set(ppen4);
                  }
               case 4:
                  {
                     pimage1->get_graphics()->set(brush5);
                     pimage1->get_graphics()->set(ppen5);
                  }
                  break;
               }
               pimage1->get_graphics()->rectangle(x1, y1, x1 + d, y1 + d);
            }

         }*/



      /*imaging.bitmap_blend(
         pimage1->get_graphics(),
         0, 0, cx1, cy1,
         pimage2->get_graphics(),
         0, 0,
         50);*/

      pimage2->channel_multiply(127.0 / 255.0, ::color::e_channel_opacity);

      {

         ::image::image_source imagesource(pimage2);

         auto rectangle = double_rectangle_dimension(0, 0, cx1, cy1);

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimage1->get_graphics()->draw(imagedrawing);

      }

      pimageBack->g()->set_interpolation_mode(VISUAL_EFFECT_STRETCH_MODE);

      {

         ::image::image_source imagesource(pimage1);

         double_rectangle rectangle(pimageBack->size());

         ::image::image_drawing_options imagedrawingoptions(rectangle);

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pimageBack->draw(imagedrawing);

      }

      recta.add(rectangleX);

   }


} // namespace backimpact





