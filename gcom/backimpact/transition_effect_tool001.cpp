#include "framework.h"
#include "transition_effect.h"
#include "gcom.h"
#include "acme/platform/system.h"
#include "acme/prototype/mathematics/mathematics.h"
//#include "acme/prototype/geometry2d/_enhanced.h"
#include "acme/prototype/geometry2d/geometry.h"
//#include "acme/prototype/time/integral/operator.h"
#include "aura/graphics/image/image.h"
#include "acme/prototype/geometry2d/_defer_bounding_box.h"

#include <math.h>


namespace backimpact
{


   transition_effect::tool_001::tool_001()
   {
      
      m_iStep = 0;

   }

   
   void transition_effect::tool_001::initialize_backimpact_helper(main * pmain)
   {

      //auto estatus = 
      
      helper::initialize_backimpact_helper(pmain);

      //if (!estatus)
      //{

      //   return estatus;

      //}


      //return estatus;

   }


   


#define HIDOUBLETOINT(d) (((d) > (int) (d)) ? ((int) (d)) + 1 : (int) (d))

   void transition_effect::tool_001::Start(
   int cx, int cy, ::f64 dRate, ::f64 dRateEx)
   {
      m_cx = cx;
      m_cy = cy;
      m_dRate = dRate;
      m_dRateEx = dRateEx;
   }

   void transition_effect::tool_001::Initialize(ETransitionEffect eeffect, int cx, int cy, transition_effect & effect)
   {
//         main & viewinterface = helper_get_main();

      m_iStepRepeatCount = 1;
      m_etransitioneffect = eeffect;
      m_cx = cx;
      m_cy = cy;
      m_iFrame = -1;
      //int iGroupCount = -1;
      int iFrameCount = -1;
      int iBarWidth = 5;
      switch(eeffect)
      {
      case TransitionEffectSingleStep:
      {
         m_iStepCount = 200;

         auto ptransitioneffect = helper_get_main()->get_transition_effect();

         if (!ptransitioneffect)
         {

            return;

         }
         ptransitioneffect->m_timeDelay = 50_ms;
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
         const int iFrameCount = 48;
         m_data.m_fly.m_iFrameCount = iFrameCount;
         m_iStepCount = iFrameCount + 1;

         auto ptransitioneffect = helper_get_main()->get_transition_effect();

         if (!ptransitioneffect)
         {

            return;

         }
         ptransitioneffect->m_timeDelay = 3_s / iFrameCount;
      }
      break;
      case TransitionEffectWipeTop:
      case TransitionEffectWipeBottom:
      case TransitionEffectWipeRight:
      case TransitionEffectWipeLeft:
      {

         auto pmain = helper_get_main();

         if (!pmain)
         {

            return;

         }

         auto ptransitioneffect = pmain->get_transition_effect();

         if (!ptransitioneffect)
         {

            return;

         }
         ptransitioneffect->m_timeDelay = 12_ms;
         //int iFrameCount = maximum(4, cy / 16);
         int iFrameCount = 48;
         m_data.m_sliceframe.m_iFrameCount = iFrameCount;
         m_iStepCount = iFrameCount + 1;
      }
      break;
      case TransitionEffectWipeBottomRight:
      case TransitionEffectWipeTopRight:
      case TransitionEffectWipeBottomLeft:
      case TransitionEffectWipeTopLeft:
      {


         auto pmain = helper_get_main();

         if (!pmain)
         {

            return;

         }

         auto ptransitioneffect = pmain->get_transition_effect();

         if (!ptransitioneffect)
         {

            return;

         }
         //const int iFrameCount = maximum(4, cy / 32);
         ptransitioneffect->m_timeDelay = 12_ms;
         int iFrameCount = 48;
         m_data.m_sliceframe.m_iFrameCount = iFrameCount;
         m_iStepCount = iFrameCount + 1;
      }
      break;
      case TransitionEffectWipeCenter:
      case TransitionEffectWipeIn:
      case TransitionEffectWipeMidHorizontal:
      case TransitionEffectWipeMidVertical:
      {

         auto pmain = helper_get_main();

         if (!pmain)
         {

            return;

         }

         auto ptransitioneffect = pmain->get_transition_effect();

         if (!ptransitioneffect)
         {

            return;

         }
         ptransitioneffect->m_timeDelay = 12_ms;
         //const int iFrameCount = maximum(4, cy / 32);
         int iFrameCount = 48;
         m_data.m_sliceframe.m_iFrameCount = iFrameCount;
         m_iStepCount = iFrameCount + 1;
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
      case TransitionEffectScaleCenter:
      {

         int iFrameCount = 48;

         effect.m_timeDelay = 12_ms;

         m_data.m_sliceframe.m_iFrameCount = iFrameCount;

         m_iStepCount = iFrameCount;

         auto pmain = helper_get_main();

         if (!pmain)
         {

            return;

         }

         ::image::image_pointer pimageFinal = pmain->get_gcom()->get_image(e_image_final);

         pimageFinal->set_mipmap(::image::e_mipmap_anisotropic);

         f64_array da;

         da.add(0.95);
         da.add(0.90);
         da.add(0.85);
         da.add(0.80);
         da.add(0.75);
         da.add(0.70);
         da.add(0.65);
         da.add(0.60);
         da.add(0.55);

         pimageFinal->create_isotropic(da, ::e_priority_idle);

      }
      break;
      case TransitionEffectCirclypixelate_:
      case TransitionEffectEllipsoidalpixelate_:
      {

         const int tilesx = maximum(4, cx / 48);
         const int tilesy = maximum(4, cy / 48);
         const int tiles = tilesx * tilesy;

         m_data.m_tiles.m_iTilesX = tilesx;
         m_data.m_tiles.m_iTilesY = tilesy;
         m_data.m_tiles.m_iTiles = tiles;

         m_ia.set_size(tiles);
         for(int i = 0; i < tiles; i++)
         {
            m_ia.element_at(i) = i;
         }
         m_iStepCount = tiles + 1;
         effect.m_timeDelay = 10_ms;
      }
      break;

      case TransitionEffectSquarypixelate_:
      {
         
         m_phi = mathematics()->random(70, 110) * 3.1415 / 180.0;
         
         ::f64_point pointaBigRect[4];
         
         GetRect(pointaBigRect, i32_rectangle(0, 0, cx, cy));
         
         m_size.cx = 64;
         
         m_size.cy = 64;

         auto psystem = system();

         auto pgeometry = psystem->geometry();

         pgeometry->rotate((m_cx / 2), (m_cy / 2), pointaBigRect, 4, m_phi);

         ::i32_rectangle rectangleBigRect;
         
         ::get_bounding_box(rectangleBigRect, pointaBigRect, 4);
         
         int iDimension = maximum(rectangleBigRect.width(), rectangleBigRect.height()) * 2;
         
         int imax = iDimension / m_size.cx;
         
         int jmax = iDimension / m_size.cy;

         ::f64_point pointaRect[4];

         ::i32_rectangle rectangle;

         ::i32_rectangle rectangleX(0, 0, m_cx, m_cy);

         for(int i =  -imax; i <= imax; i++)
         {

            for(int j = -jmax; j <= jmax; j++)
            {
               GetRotateRect(i, j, pointaRect);
               ::get_bounding_box(rectangle, pointaRect, 4);
               if(rectangle.intersect(rectangleX, rectangle))
               {
                  m_pointa.add(i32_point(i, j));
               }
            }
         }
         m_iStepCount = m_pointa.get_size() + 1;
         effect.m_timeDelay = 10_ms;
      }
      break;
      case TransitionEffectHexagonalpixelate_:
      {
         effect.m_timeDelay = 10_ms;
         m_phi = mathematics()->random(70, 110) * 3.1415 / 180.0;
         ::f64_point pointaBigRect[4];
         GetRect(pointaBigRect, i32_rectangle(0, 0, cx, cy));
         
         auto psystem = system();

         auto pgeometry = psystem->geometry();

         pgeometry->rotate((m_cx / 2), (m_cy / 2), pointaBigRect, 4, m_phi);

         ::i32_rectangle rectangleBigRect;
         ::get_bounding_box(rectangleBigRect, pointaBigRect, 4);
         m_iRadius = 48;
         int & iRadius = m_iRadius;
         int iDimension = maximum(rectangleBigRect.width(), rectangleBigRect.height());
         int mCount = ((int) ((iDimension / (2.0 + 3.0 * iRadius)) + 2)) + 2;
         int nCount = ((int) HIDOUBLETOINT(iDimension / (iRadius * sqrt(3.0)))) + 2;
         int mExCount = mCount + 1;
         int nExCount = nCount + 1;
         int imax = (mCount + mExCount) / 2;
         int jmax = (nCount + nExCount) / 2;
         ::f64_point pointaHexagon[6];
         ::i32_rectangle rectangle;
         ::i32_rectangle rectangleX(0, 0, m_cx, m_cy);
         for(int i =  -imax; i <= imax; i++)
         {
            for(int j = -jmax; j <= jmax; j++)
            {
               
               GetRotateHexagon(i, j,  pointaHexagon);
               
               ::get_bounding_box(rectangle, pointaHexagon, 6);
               
               if(rectangle.intersect(rectangleX, rectangle))
               {
                  
                  m_pointa.add(i32_point(i, j));
                  
               }
               
            }
            
         }
         
         m_iStepCount = m_pointa.get_size() + 1;
         
      }
      break;
      case TransitionEffectAccumulLinearFadingBottomTop:
      case TransitionEffectAccumulLinearFadingTopBottom:
      {

         iFrameCount = cy/ iBarWidth;
      }
      case TransitionEffectAccumulLinearFadingLeftRight:
      case TransitionEffectAccumulLinearFadingRightLeft:
      {

         iFrameCount = cx/ iBarWidth;

         m_iStepRepeatCount = 128;

         effect.m_timeDelay = 0_ms;
         int iImageCount = 100;
         //::f64 dPow = 1.0 / 2.5;
         //const int iMaxGroupCount = 256;
         // const int iMaxGroupCount = 127;
         //const int iTileCount = maximum(4, cy / 8);
         const int iTileCount = iFrameCount;
         
         const int iGroupCount = iImageCount;

         //m_dAlpha = ceil(255.0 / pow(iGroupCount, dPow));

         m_data.m_sliceframe.m_iImageCount = iImageCount;
         m_data.m_sliceframe.m_iTileCount = iTileCount;
         m_data.m_sliceframe.m_iFrameCount = iFrameCount;
         m_data.m_sliceframe.m_iGroupCount = iGroupCount;
//               int iTileExCount = maximum(iTileCount - iFrameCount + 1, 0);
         for(int iFrame = - iGroupCount; iFrame < iFrameCount; iFrame++)
         {
            for(int iGroup =  0; iGroup < iGroupCount; iGroup++)
            {
               int iTile = iGroup + iFrame;
               if(iTile >= 0
                     && iTile < iTileCount)
               {
                  m_pointa.add(i32_point(iTile, iGroup));
               }
            }
         }
         m_ia.set_size(iTileCount);
         m_ia.set_each(-1);
         m_iStepCount = m_pointa.get_size() + 1;
      }

      break;
      case TransitionEffectLinearFadingBottomTop:
      case TransitionEffectLinearFadingTopBottom:
      case TransitionEffectLinearFadingLeftRight:
      case TransitionEffectLinearFadingRightLeft:
      {
         effect.m_timeDelay = 5_ms;
         m_iStepRepeatCount = 1;
         const int iTileCount = 1;
         const int iFrameCount = 49;
         m_data.m_sliceframe.m_iTileCount = iTileCount;
         m_data.m_sliceframe.m_iFrameCount = iFrameCount;
         m_iStepCount = iTileCount * iFrameCount + 1;
      }
      break;
      case TransitionEffectpixelate_BottomTop:
      case TransitionEffectpixelate_TopBottom:
      case TransitionEffectpixelate_LeftRight:
      case TransitionEffectpixelate_RightLeft:
      {
         m_iStepRepeatCount = 1;

         effect.m_timeDelay = 90_ms;

         int c1 = 0;
         int c2 = 0;

         switch(eeffect)
         {
         case TransitionEffectpixelate_BottomTop:
         case TransitionEffectpixelate_TopBottom:
            c1 = cy;
            c2 = cx;
            break;
         case TransitionEffectpixelate_LeftRight:
         case TransitionEffectpixelate_RightLeft:
            c1 = cx;
            c2 = cy;
            break;
         default:
            ASSERT(false);
            break;
         }

         m_data.m_alphapixelate.m_c1 = c1;
         m_data.m_alphapixelate.m_c2 = c2;


         int iSize;

         {
            ::f64 dSize;
            ::f64 dMaxSize = c1;
            ::f64 dMinSize = 1.0;
            ::f64 dRate = sqrt(2.0);

            for(dSize = dMinSize; dSize <= dMaxSize; dSize *= dRate)
            {
               m_ia.add((int) dSize);
            }

            for(dSize /= dRate; dSize >= dMinSize; dSize /= dRate)
            {
               m_ia.add((int) dSize);
            }

         }


         for(int i = 0; i < m_ia.get_size(); i++)
         {

            iSize = m_ia[i];

            ASSERT(iSize >= 1);

            if(iSize < 64)
               iSize = 64;

            ::collection::index j = 0;

            ::collection::index iStartIndex = m_pointa.get_size();

            for(; j < c1; j += iSize)
            {
               m_pointa.add(::i32_point((int) i,(int)  j));
               m_pointa2.add(::i32_point((int) iSize,(int)  iStartIndex));
            }

            m_pointa.add(::i32_point((int) i, (int) c1));
            m_pointa2.add(::i32_point((int) iSize,(int)  iStartIndex));

         }

         m_iStepCount = m_pointa.get_size() + 1;
         m_data.m_alphapixelate.m_iSizeIndex = -1;
      }
      break;
      case TransitionEffectRadialUnveil:
      {
         m_data.m_radialunveil.m_iRadiusIncrement = 20;
         m_data.m_radialunveil.m_iRadius = 0;
         m_pointa.erase_all();

         int c1 = maximum(cx, cy) * 3 / 2;
         int dim = 1;
         int ca = 0;
         while(dim < c1)
         {
            m_pointa.add(i32_point(dim, 50));
            dim += m_data.m_radialunveil.m_iRadiusIncrement + ca;
            ca += 10;
         }
         m_iStepCount = m_pointa.get_size();
         m_iStepRepeatCount = 1;

         effect.m_timeDelay = 50_ms;

      }
      break;
      case TransitionEffectVisual:
      {
         m_iStepRepeatCount = 1;

         switch(effect.m_iVisual)
         {
         case VisualEffectRotateBlend:
            m_iStepCount = 180;
            break;
         case VisualEffectNoPrecisionRotateBlend:
            m_iStepCount = 180;
            break;
         case VisualEffectNoPrecisionRotateColorBlend:
            m_iStepCount = 180;
            break;
         case VisualEffectNoPrecisionRotateTrackColorBlend:
            m_iStepCount = 180;
            break;
         case VisualEffectRotateEx1:
            m_iStepCount = 180;
            break;
         case VisualEffectRotateEx2:
            m_iStepCount = 180;
            break;
         case VisualEffectRotateEx3:
            m_iStepCount = 180;
            break;
         case VisualEffectRotateEx4:
            m_iStepCount = 180;
            break;
         case VisualEffectRain1:
            m_iStepCount = 180;
            break;
         case VisualEffectRotateEx5:
            m_iStepCount = 180;
            break;
         case VisualEffectRotateEx6:
            m_iStepCount = 180;
            break;
         case VisualEffectRotateEx7:
            m_iStepCount = 180;
            break;
         case VisualEffectRotateEx8:
            m_iStepCount = 180;
            break;
         case VisualEffectPixelExplosion:
            m_iStepCount = 180;
            break;
         case VisualEffectPixelExplosion2:
            m_iStepCount = 180;
            break;
         case VisualEffectAlphaPixelExplosion:
            m_iStepCount = 180;
            break;
         default:
            m_iStepCount = 180;
            break;
         }

         m_iStepCount *= 2;
         effect.m_timeDelay = 33_ms;

      }
      break;
      default:
         break;
      }
   }


   void transition_effect::tool_001::Go()
   {
      ETransitionEffect etransitioneffect = m_etransitioneffect;

      const int & cx = m_cx;
      const int & cy = m_cy;
//         const ::f64 dRate = m_dRate;
//         const ::f64 dRateEx = m_dRateEx;
      i32_rectangle & rectangleUpdate = m_rectangle;
      i32_point & point = m_point;
      i32_rectangle & rectangleA = m_rectangleA;
      i32_rectangle & rectangleB = m_rectangleB;

      switch(etransitioneffect)
      {
      case TransitionEffectLinearFadingTopBottom:
      {
         GetSliceRect(&rectangleUpdate, AlignTop);
      }
      break;
      case TransitionEffectLinearFadingBottomTop:
      {
         GetSliceRect(&rectangleUpdate, AlignBottom);
      }
      break;
      case TransitionEffectLinearFadingLeftRight:
      {
         GetSliceRect(&rectangleUpdate, AlignLeft);
      }
      break;
      case TransitionEffectLinearFadingRightLeft:
      {
         GetSliceRect(&rectangleUpdate, AlignRight);
      }
      break;
      case TransitionEffectAccumulLinearFadingTopBottom:
      {
         GetSliceRect(&rectangleUpdate, AlignTop);
      }
      break;
      case TransitionEffectAccumulLinearFadingBottomTop:
      {
         GetSliceRect(&rectangleUpdate, AlignBottom);
      }
      break;
      case TransitionEffectAccumulLinearFadingLeftRight:
      {
         GetSliceRect(&rectangleUpdate, AlignLeft);
      }
      break;
      case TransitionEffectAccumulLinearFadingRightLeft:
      {
         GetSliceRect(&rectangleUpdate, AlignRight);
      }
      break;
      case TransitionEffectFlyInTopBottom:
      {
         GetRect(&rectangleUpdate, AlignTop);
         point.x = 0;
         point.y = rectangleUpdate.bottom - cy;
      }
      break;
      case TransitionEffectFlyInBottomTop:
      {
         GetRect(&rectangleUpdate, AlignBottom);
         point.x = 0;
         point.y = 0;
      }
      break;
      case TransitionEffectFlyInLeftRight:
      {
         GetRect(&rectangleUpdate, AlignLeft);
         point.x = rectangleUpdate.right - cx;
         point.y = 0;
      }
      break;
      case TransitionEffectFlyInRightLeft:
      {
         GetRect(&rectangleUpdate, AlignRight);
         point.x = 0;
         point.y = 0;
      }
      break;
      case TransitionEffectFlyInLeftBottom:
      {
         GetRect(&rectangleUpdate, AlignTopRight);
         point.x = rectangleUpdate.right - cx;
         point.y = rectangleUpdate.bottom - cy;
      }
      break;
      case TransitionEffectFlyInLeftTop:
      {
         GetRect(&rectangleUpdate, AlignRightBottom);
         point.x = cx - rectangleUpdate.right;
         point.y = 0;
      }
      break;
      case TransitionEffectFlyInRightBottom:
      {
         GetRect(&rectangleUpdate, AlignLeftTop);
         point.x = 0;
         point.y = rectangleUpdate.bottom - cy;
      }
      break;
      case TransitionEffectFlyInRightTop:
      {
         GetRect(&rectangleUpdate, AlignBottomLeft);
         point.x = 0;
         point.y = 0;
      }
      break;
      case TransitionEffectWipeTopRight:
      case TransitionEffectWipeBottomRight:
      case TransitionEffectWipeTopLeft:
      case TransitionEffectWipeBottomLeft:
      {
         GetRectAB(&rectangleA, &rectangleB);
      }
      break;
      case TransitionEffectWipeCenter:
      case TransitionEffectWipeIn:
      case TransitionEffectWipeMidHorizontal:
      case TransitionEffectWipeMidVertical:
      {
         GetRect(&rectangleUpdate, AlignCenter);
      }
      break;
      case TransitionEffectScaleBottom:
      {
         GetRect(&rectangleA, AlignTop);
         rectangleB.set(
         0, 0,
         cx, cy);
      }
      break;
      case TransitionEffectScaleTop:
      {
         GetRect(&rectangleA, AlignBottom);
         rectangleB.set(
         0, 0,
         cx, cy);
      }
      break;
      case TransitionEffectScaleRight:
      {
         GetRect(&rectangleA, AlignLeft);
         rectangleB.set(
         0, 0,
         cx, cy);
      }
      break;
      case TransitionEffectScaleLeft:
      {
         GetRect(&rectangleA, AlignRight);
         rectangleB.set(
         0, 0,
         cx, cy);
      }
      break;
      case TransitionEffectScaleTopRight:
      {
         GetRect(&rectangleA, AlignTopRight);
         rectangleB.set(
         0, 0,
         cx, cy);
      }
      break;
      case TransitionEffectScaleBottomRight:
      {
         GetRect(&rectangleA, AlignRightBottom);
         rectangleB.set(
         0, 0,
         cx, cy);
      }
      break;
      case TransitionEffectScaleTopLeft:
      {
         GetRect(&rectangleA, AlignLeftTop);
         rectangleB.set(
         0, 0,
         cx, cy);
      }
      break;
      case TransitionEffectScaleBottomLeft:
      {
         GetRect(&rectangleA, AlignBottomLeft);
         rectangleB.set(
         0, 0,
         cx, cy);
      }
      break;
      case TransitionEffectScaleCenter:
      {
         GetRect(&rectangleA, AlignCenter);
         rectangleB.set(
         0, 0,
         cx, cy);
      }
      break;
      case TransitionEffectWipeTop:
      case TransitionEffectWipeBottom:
      case TransitionEffectWipeRight:
      case TransitionEffectWipeLeft:
      {
         GetRect(&rectangleUpdate);
      }
      break;
      case TransitionEffectpixelate_TopBottom:
      {
         GetSliceRect(&rectangleUpdate, AlignTop);
      }
      break;
      case TransitionEffectpixelate_BottomTop:
      {
         GetSliceRect(&rectangleUpdate, AlignBottom);
      }
      break;
      case TransitionEffectpixelate_LeftRight:
      {
         GetSliceRect(&rectangleUpdate, AlignLeft);
      }
      break;
      case TransitionEffectpixelate_RightLeft:
      {
         GetSliceRect(&rectangleUpdate, AlignRight);
      }
      break;
      default:
         ASSERT(false);
         break;
      }
   }

   void transition_effect::tool_001::GetRect(::i32_rectangle * lprect)
   {
      GetRect(lprect, m_dRate, m_dRateEx);
   }

   void transition_effect::tool_001::GetRect(::i32_rectangle * lprect, ::f64 dRate, ::f64 dRateEx)
   {
      const int & cx = m_cx;
      const int & cy = m_cy;
//         i32_rectangle & rectangleUpdate = m_rectangle;
//         i32_point & point = m_point;
//         i32_rectangle & rectangleA = m_rectangleA;
//         i32_rectangle & rectangleB = m_rectangleB;
      switch(m_etransitioneffect)
      {
      case TransitionEffectWipeBottom:
      {
         lprect->left = 0;
         lprect->right = lprect->left + cx;
         lprect->top = (int) (dRate * cy);
         lprect->bottom = (int) (dRateEx * cy);
      }
      break;
      case TransitionEffectWipeTop:
      {
         lprect->left = 0;
         lprect->right = lprect->left + cx;
         lprect->top = (int) ((1.0 - dRateEx) * cy);
         lprect->bottom = (int) (1.0 - dRate) * cy;
      }
      break;
      case TransitionEffectWipeRight:
      {
         lprect->left =  (int) (dRate * cx);
         lprect->right =  (int) (dRateEx * cx);
         lprect->top = 0;
         lprect->bottom = lprect->top + cy;;
      }
      break;
      case TransitionEffectWipeLeft:
      {
         lprect->left =  (int) ((1.0 - dRateEx) * cx);
         lprect->right =  (int) ((1.0 - dRate) * cx);
         lprect->top = 0;
         lprect->bottom = lprect->top + cy;;
      }
      break;
      case TransitionEffectWipeTopRight:
      {
         GetRect(lprect, AlignTopRight, dRate);
      }
      break;
      case TransitionEffectWipeBottomRight:
      {
         GetRect(lprect, AlignRightBottom, dRate);
      }
      break;
      case TransitionEffectWipeTopLeft:
      {
         GetRect(lprect, AlignLeftTop, dRate);
      }
      break;
      case TransitionEffectWipeBottomLeft:
      {
         GetRect(lprect, AlignBottomLeft, dRate);
      }
      break;
      case TransitionEffectWipeCenter:
      {
         GetRect(lprect, AlignCenter, dRate);
      }
      break;
      case TransitionEffectWipeIn:
      {
         GetRect(lprect, AlignCenterOut, dRate);
      }
      break;
      default:
         ASSERT(false);
         break;
      }

   }


   void transition_effect::tool_001::GetRectAB(::i32_rectangle * lprectA, ::i32_rectangle * lprectB)
   {
//         const int & cx = m_cx;
//         const int & cy = m_cy;
      const ::f64 dRate = m_dRate;
      const ::f64 dRateEx = m_dRateEx;
      i32_rectangle & rectangleUpdate = m_rectangle;
//         i32_point & point = m_point;

      ::i32_rectangle rectangleUpdatePrevious;

      GetRect(&rectangleUpdatePrevious, dRate, dRate);
      GetRect(&rectangleUpdate, dRateEx, dRateEx);

      *lprectA = rectangleUpdate;
      *lprectB = rectangleUpdate;


      switch(m_etransitioneffect)
      {
      case TransitionEffectWipeTopRight:
      {
         lprectA->left     = rectangleUpdatePrevious.right;
         lprectB->right    = lprectA->left;
         lprectB->top      = rectangleUpdatePrevious.bottom;
      }
      break;
      case TransitionEffectWipeBottomRight:
      {
         lprectB->bottom   = rectangleUpdatePrevious.top;
         lprectA->left     = rectangleUpdatePrevious.right;;
         lprectA->top      = lprectB->bottom;
      }
      break;
      case TransitionEffectWipeTopLeft:
      {
         lprectA->right = rectangleUpdatePrevious.left;
         lprectB->left = lprectA->right;
         lprectB->top = rectangleUpdatePrevious.bottom;
      }
      break;
      case TransitionEffectWipeBottomLeft:
      {
         lprectB->bottom   = rectangleUpdatePrevious.top;
         lprectA->right    = rectangleUpdatePrevious.left;
         lprectA->top      = lprectB->bottom;
      }
      break;
      default:
         ASSERT(false);
         break;
      }
   }

   void transition_effect::tool_001::GetRectABCD(::i32_rectangle * lprectA, ::i32_rectangle * lprectB, ::i32_rectangle * lprectC, ::i32_rectangle * lprectD)
   {
      i32_rectangle & rectangleUpdate = m_rectangle;
      const ::f64 dRate = m_dRate;
      const ::f64 dRateEx = m_dRateEx;

      ::i32_rectangle rectangleUpdatePrevious;

      GetRect(&rectangleUpdatePrevious, dRate, dRate);
      GetRect(&rectangleUpdate, dRateEx, dRateEx);

      switch(m_etransitioneffect)
      {
      case TransitionEffectWipeCenter:
      case TransitionEffectWipeIn:
      {
         GetSliceRect(lprectA, AlignLeft);
         GetSliceRect(lprectB, AlignTop);
         GetSliceRect(lprectC, AlignRight);
         GetSliceRect(lprectD, AlignBottom);
         ::intersect(*lprectB, *lprectB, rectangleUpdatePrevious);
         ::intersect(*lprectD, *lprectD, rectangleUpdatePrevious);
      }
      break;
      default:
         ASSERT(false);
         break;
      }
   }


   void transition_effect::tool_001::GetRect(::i32_rectangle * lprect, EAlign ealign)
   {

      GetRect(lprect, ealign, m_dRate);

   }


   void transition_effect::tool_001::GetRect(::i32_rectangle * lprect, EAlign ealign, ::f64 dRate)
   {

      GetRect(lprect, ealign, m_cx, m_cy, dRate);

   }


   void transition_effect::tool_001::GetRect(::i32_rectangle * lprect, EAlign ealign, int cx, int cy, ::f64 dRate)
   {

      switch(ealign)
      {
      case AlignTop:
      {
         lprect->left = 0;
         lprect->right = lprect->left + cx;
         lprect->top = (int) (dRate * cy);
         lprect->bottom = cy;
      }
      break;
      case AlignBottom:
      {
         lprect->left = 0;
         lprect->right = lprect->left + cx;
         lprect->top = 0;
         lprect->bottom = (int) (1.0 - dRate) * cy;
      }
      break;
      case AlignLeft:
      {
         lprect->left =  0;
         lprect->right =  (int) (dRate * cx);
         lprect->top = 0;
         lprect->bottom = lprect->top + cy;;
      }
      break;
      case AlignRight:
      {
         lprect->left =  (int) ((1.0 - dRate) * cx);
         lprect->right =  cx;
         lprect->top = 0;
         lprect->bottom = lprect->top + cy;;
      }
      break;
      case AlignTopRight:
      {
         lprect->left = 0;
         lprect->right = (int) (dRate * cx);
         lprect->top = 0;
         lprect->bottom = (int) (dRate * cy);
      }
      break;
      case AlignRightBottom:
      {
         lprect->left = 0;
         lprect->right = (int) (dRate * cx);
         lprect->top = (int) ((1.0 - dRate) * cy);
         lprect->bottom = cy;
      }
      break;
      case AlignLeftTop:
      {
         lprect->left = (int) ((1.0 - dRate) * cx);
         lprect->right =  cx;
         lprect->top = 0;
         lprect->bottom = (int) (dRate * cy);
      }
      break;
      case AlignBottomLeft:
      {
         lprect->left =  (int) ((1.0 - dRate) * cx);
         lprect->right =  cx;
         lprect->top = (int) ((1.0 - dRate) * cy);
         lprect->bottom = cy;;
      }
      break;
      case AlignCenter:
      {
         ::f64 dHalfRate = dRate / 2.0;
         lprect->left =  (int) ((0.5 - dHalfRate) * (cx));
         lprect->right =  (int) (dHalfRate * cx);
         lprect->top = (int) ((0.5 - dHalfRate) * (cy));
         lprect->bottom = (int) (dHalfRate * cy);
      }
      break;
      case AlignCenterOut:
      {
         ::f64 dHalfRate = 0.5 - (dRate / 2.0);
         lprect->left =  (int) ((0.5 - dHalfRate) * (cx));
         lprect->right =  (int) (dHalfRate * cx);
         lprect->top = (int) ((0.5 - dHalfRate) * (cy));
         lprect->bottom = (int) (dHalfRate * cy);
      }
      break;
      default:
         ASSERT(false);
         break;
      }

   }


   void transition_effect::tool_001::GetSliceRect(::i32_rectangle * lprect, EAlign ealign)
   {

      GetSliceRect(lprect, ealign, m_dRate, m_dRateEx);

   }


   void transition_effect::tool_001::GetSliceRect(::i32_rectangle * lprect, EAlign ealign,
      ::f64 dRate, ::f64 dRatePlus)
   {

      GetSliceRect(m_cx, m_cy, lprect, ealign, dRate, dRatePlus);

   }


   void transition_effect::tool_001::GetSliceRect(int cx, int cy, ::i32_rectangle * lprect,
   EAlign ealign, ::f64 dRate, ::f64 dRatePlus)
   {

      switch(ealign)
      {
      case AlignTop:
      {
         lprect->left = 0;
         lprect->right = lprect->left + cx;
         lprect->top = (int) (dRate * cy);
         lprect->bottom = (int) (dRatePlus * cy);
      }
      break;
      case AlignBottom:
      {
         lprect->left = 0;
         lprect->right = lprect->left + cx;
         lprect->top = (int) ((1.0 - dRatePlus) * cy);
         lprect->bottom = (int) ((1.0 - dRate) * cy);
      }
      break;
      case AlignLeft:
      {
         lprect->left =  (int) (dRate * cx);
         lprect->right =  (int) (dRatePlus * cx);
         lprect->top = 0;
         lprect->bottom = lprect->top + cy;;
      }
      break;
      case AlignRight:
      {
         lprect->left =  (int) ((1.0 - dRatePlus) * cx);
         lprect->right =  (int) ((1.0 - dRate) * cx);
         lprect->top = 0;
         lprect->bottom = lprect->top + cy;;
      }
      break;
      case AlignTopRight:
      {
         lprect->left = (int) (dRate * cx);
         lprect->right = (int) (dRatePlus * cx);
         lprect->top = (int) (dRate * cy);
         lprect->bottom = (int) (dRatePlus * cy);
      }
      break;
      case AlignRightBottom:
      {
         lprect->left = (int) (dRate * cx);;
         lprect->right = (int) (dRatePlus * cx);
         lprect->top = (int) ((1.0 - dRatePlus) * cy);
         lprect->bottom = (int) ((1.0 - dRate) * cy);
      }
      break;
      case AlignLeftTop:
      {
         lprect->left = (int) ((1.0 - dRatePlus) * cx);
         lprect->right =  (int) ((1.0 - dRate) * cx);
         lprect->top = (int) (dRate * cy);
         lprect->bottom = (int) (dRatePlus * cy);
      }
      break;
      case AlignBottomLeft:
      {
         lprect->left =  (int) ((1.0 - dRatePlus) * cx);
         lprect->right =  (int) ((1.0 - dRate) * cx);
         lprect->top = (int) ((1.0 - dRatePlus) * cy);
         lprect->bottom = (int) ((1.0 - dRate) * cy);
      }
      break;
      case AlignCenter:
      {
         ::f64 dHalfRate = dRatePlus / 2.0;
         lprect->left =  (int) ((0.5 - dHalfRate) * (cx));
         lprect->right =  (int) (dHalfRate * cx);
         lprect->top = (int) ((0.5 - dHalfRate) * (cy));
         lprect->bottom = (int) (dHalfRate * cy);
      }
      break;
      default:
         ASSERT(false);
         break;
      }

   }


   void transition_effect::tool_001::
   GetHorizontalHexagon(::i32_rectangle * lprect, ::f64_point * lppointa)
   {
      ::f64 y1 = lprect->top;
      ::f64 y3 = lprect->bottom;
      ::f64 y2 = (y1 + y3) / 2.0;
      ::f64 x1 = lprect->left;
      ::f64 x4 = lprect->right;
      ::f64 dr = (x4 - x1) / 2.0;
      ::f64 x2 = (x1 + dr / 2.0);
      ::f64 x3 = x1 + dr * 1.5;

      lppointa[0] = f64_point(x1, y2);
      lppointa[1] = f64_point(x2, y1);
      lppointa[2] = f64_point(x3, y1);
      lppointa[3] = f64_point(x4, y2);
      lppointa[4] = f64_point(x3, y3);
      lppointa[5] = f64_point(x2, y3);

   }


   int transition_effect::tool_001::FindRandomEnglobingEllipse(const ::i32_rectangle * lpcrect, ::i32_rectangle * lprectEllipse, int iMaxRand)
   {

      int iRandX = (int) mathematics()->random(-iMaxRand, iMaxRand);

      int iRandY = (int) mathematics()->random(-iMaxRand, iMaxRand);

      ::i32_rectangle rectangle(*lpcrect);

      ::f64 dMaxSide = maximum(rectangle.width(), rectangle.height());

      ::f64 dAdd = maximum(abs(iRandX), abs(iRandY));

      ::f64 dSide = dMaxSide + dAdd;

      ::f64 dRadius = dSide / sqrt(2.0);

      ::f64 dDiff = dSide - dRadius;

      int iLContrib = iRandX < 0 ? iRandX : 0;
      int iRContrib = iRandX > 0 ? iRandX : 0;
      int iTContrib = iRandY < 0 ? iRandY : 0;
      int iBContrib = iRandY > 0 ? iRandY : 0;

      lprectEllipse->left = (int) (rectangle.left + iLContrib   - dDiff);
      lprectEllipse->top = (int) (rectangle.top + iTContrib - dDiff);
      lprectEllipse->right = (int) (rectangle.right + iRContrib   + dDiff);
      lprectEllipse->bottom = (int) (rectangle.bottom + iBContrib   + dDiff);

      return 1;

   }


   int transition_effect::tool_001::FindRandomEnglobingCircle(const ::i32_rectangle * lpcrect, ::i32_rectangle * lprectCircle, int iMaxRand)
   {

      int iRand = (int) mathematics()->random(-iMaxRand, iMaxRand);

      ::i32_rectangle rectangle(*lpcrect);

      ::f64 dMaxSide = maximum(rectangle.width(), rectangle.height());

      ::f64 dAdd = abs(iRand);

      ::f64 dSide = dMaxSide + dAdd;

      ::f64 dRadius = dSide / sqrt(2.0);

      ::f64 dDiff = dSide - dRadius;

      int iLContrib = iRand < 0 ? iRand : 0;
      int iRContrib = iRand > 0 ? iRand : 0;
      int iTContrib = iRand < 0 ? iRand : 0;
      int iBContrib = iRand > 0 ? iRand : 0;

      lprectCircle->left = (int) (rectangle.left + iLContrib   - dDiff);
      lprectCircle->top = (int) (rectangle.top + iTContrib - dDiff);
      lprectCircle->right = (int) (rectangle.right + iRContrib   + dDiff);
      lprectCircle->bottom = (int) (rectangle.bottom + iBContrib   + dDiff);

      return 1;

   }


   void transition_effect::tool_001::Finalize()
   {

      m_ia.erase_all();
      m_pointa.erase_all();
      m_pointa2.erase_all();

   }


   void transition_effect::tool_001::GetRect(::i32_point * lppoint, const ::i32_rectangle & rectangle)
   {

      lppoint[0].x = (int) rectangle.left;
      lppoint[0].y = (int) rectangle.top;
      lppoint[1].x = (int) rectangle.right;
      lppoint[1].y = (int) rectangle.top;
      lppoint[2].x = (int) rectangle.right;
      lppoint[2].y = (int) rectangle.bottom;
      lppoint[3].x = (int) rectangle.left;
      lppoint[3].y = (int) rectangle.bottom;

   }


   void transition_effect::tool_001::GetRect(::f64_point * lppoint, const ::i32_rectangle & rectangle)
   {

      lppoint[0].x = (int) rectangle.left - 1.0;
      lppoint[0].y = (int) rectangle.top - 1.0;
      lppoint[1].x = (int) rectangle.right + 1.0;
      lppoint[1].y = (int) rectangle.top - 1.0;
      lppoint[2].x = (int) rectangle.right + 1.0;
      lppoint[2].y = (int) rectangle.bottom + 1.0;
      lppoint[3].x = (int) rectangle.left - 1.0;
      lppoint[3].y = (int) rectangle.bottom + 1.0;

   }


   void transition_effect::EnableEffect(ETransitionEffect eeffect)
   {

      if(m_etypea.find_first(eeffect) < 0)
      {

         m_etypea.add(eeffect);

      }

   }


   void transition_effect::DisableEffect(ETransitionEffect eeffect)
   {

      m_etypea.erase(eeffect);

   }


//   void transition_effect::tool_001::GetSimplePolyBox(::i32_rectangle * lprect, ::f64_point * lppoint, int iCount)
//   {
//
//      if (iCount <= 0)
//      {
//
//         return;
//
//      }
//
//      ::f64 left = lppoint[0].x;
//      ::f64 right = left;
//      ::f64 top = lppoint[0].y;
//      ::f64 bottom = top;
//      for(int i = 1; i < iCount; i++)
//      {
//         if(lppoint[i].x < left)
//            left = floor(lppoint[i].x);
//         else if(lppoint[i].x > right)
//            right = ceil(lppoint[i].x);
//         if(lppoint[i].y < top)
//            top= floor(lppoint[i].y);
//         else if(lppoint[i].y > bottom)
//            bottom = ceil(lppoint[i].y);
//      }
//      lprect->left = (int) left;
//      lprect->right = (int)right;
//      lprect->top = (int)top;
//      lprect->bottom = (int)bottom;
//
//   }


   void transition_effect::tool_001::GetRotateRect(int w, int h, int i, int j, ::f64 phi, ::f64_point * lppoint)
   {

      ::i32_rectangle rectangle;
      TranslateRect(&rectangle, w, h, i, j);
      GetRect(lppoint, rectangle);

      auto psystem = system();

      auto pgeometry = psystem->geometry();

      pgeometry->rotate((m_cx / 2), (m_cy / 2), lppoint, 4, phi);

   }


   void transition_effect::tool_001::GetRotateRect(int i, int j, ::f64_point * lppoint)
   {

      GetRotateRect(m_size.cx, m_size.cy, i, j, m_phi, lppoint);

   }


   void transition_effect::tool_001::TranslateRect(::i32_rectangle * lprect, int w, int h, int i, int j)
   {

      int greekdeltax = (m_cx - w) / 2;
      int greekdeltay = (m_cy - h) / 2;
      lprect->left = greekdeltax + w * i;
      lprect->right = lprect->left + w;
      lprect->top = greekdeltay + h * j;
      lprect->bottom = lprect->top + h;

   }


   void transition_effect::tool_001::GetRotateHexagon(int iRadius, int i, int j, ::f64 phi, ::f64_point * lppoint)
   {

      ::f64 d3Sqrt = sqrt(3.0);
      bool bEx = (abs(i) % 2 == 0) ^
                 (abs(j) % 2 == 0);
      i /= 2;
      j /= 2;
      ::f64 dLeft = (m_cx - 3.0 * iRadius) / 2;
      ::f64 dRight;
      ::f64 dTop = (m_cy - iRadius * d3Sqrt) / 2;
      ::f64 dBottom;
      if(bEx)
      {
         dLeft += iRadius * (3.0 * i - 1.5);
         dTop +=  iRadius * (d3Sqrt * (j - 0.5));
      }
      else
      {
         dLeft += iRadius * (3.0 * i);
         dTop +=  iRadius * (d3Sqrt * (j));
      }
      ::i32_rectangle rectangle;
      dRight  = dLeft + iRadius * 2.05  + 1;
      dBottom = dTop + iRadius * sqrt(3.1) + 1;
      rectangle.left = (int) dLeft;
      rectangle.top = (int) dTop;
      rectangle.right = (int) dRight;
      rectangle.bottom = (int) dBottom;

      GetHorizontalHexagon(&rectangle, lppoint);
      
      auto psystem = system();

      auto pgeometry = psystem->geometry();

      pgeometry->rotate((m_cx / 2), (m_cy / 2), lppoint, 6, phi);

   }


   void transition_effect::tool_001::GetRotateHexagon(int i, int j, ::f64_point * lppoint)
   {

      GetRotateHexagon(m_iRadius, i, j, m_phi, lppoint);

   }


} // namespace backimpact



