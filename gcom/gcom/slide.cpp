#include "framework.h"
#include "slide.h"
#include "slide_drawer.h"
#include "acme/parallelization/synchronous_lock.h"


namespace gcom
{


   slide::slide()
   {

      m_bActive = false;
      m_bEnable = false;
      m_bFadeIn = false;
      m_bFadeOut = false;
      //m_opacityStart = 0;

   }

   slide::slide(const ::scoped_string & scopedstrPath) :
      slide()
   {

      m_strPath = scopedstrPath;

   }

   slide::~slide()
   {
   }


   bool slide::gcom_slide_step()
   {

      auto bAlphaShift = byte_clip(255.0 * m_timeStart.ramp_up(m_pshow->m_timeFadePeriod));

      if (m_iDrawable == m_pshow->m_iFadeIn && m_bEnable)
      {

         m_bFadeOut = false;

         if (!m_bFadeIn)
         {

            m_bFadeIn = true;

            m_timeStart.Now();

            m_opacityStart = m_opacitySlide;

            bAlphaShift = 0;

            printf_line("Starting fading in");

         }

         m_opacitySlide = m_opacityStart + bAlphaShift;

         //printf_line(".");

 /*        if (ended())
         {

            m_pshow->m_iFadeIn++;

         }*/

      }
      else
      {

         m_bFadeIn = false;

         if (!m_bFadeOut)
         {

            m_bFadeOut = true;

            m_timeStart.Now();

            m_opacityStart = m_opacitySlide;

            bAlphaShift = 0;

         }

         if (m_iDrawable == m_pshow->m_iFadeIn)
         {

            if (!m_bEnable)
            {

               m_pshow->m_iFadeIn++;

            }

         }

         m_opacitySlide = m_opacityStart - bAlphaShift;

         //printf_line(".");

      }

      return ::success;

   }


   bool slide::ended()
   {

      return m_pshow->m_iFadeIn == m_iDrawable
         && m_timeStart.elapsed() > m_pshow->m_timeSlidePeriod;

   }


   void slide::enable(bool bEnable)
   {

      m_bEnable = bEnable;

      if (bEnable)
      {

         if (m_pshow)
         {

            m_pshow->m_iFadeIn = m_iDrawable;

            auto a = m_pshow->get_drawable();

            if (a.is_empty())
            {

               m_iZOrder = 0;

            }
            else
            {

               m_iZOrder = a[0]->m_iZOrder + 1;

            }

         }

      }

   }


   slide_show::slide_show(slide_drawer * pdrawer) :
      m_pdrawer(pdrawer)
   {

      initialize(pdrawer);

      defer_create_synchronization();

      m_iFadeIn = 0;

      m_timeFadePeriod = 300_ms;

      m_timeSlidePeriod = 5000_ms;

      //m_timeSlidePeriod = 1_min;

   }


   slide_show::~slide_show()
   {

   }


   void slide_show::draw(::draw2d::graphics_pointer & pgraphics)
   {

      synchronous_lock synchronouslock(this->synchronization(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);

      gcom_slide_show_step();

      auto slidea = get_drawable();

      auto pdrawer = m_pdrawer;

      for (::collection::index i = 0; i < slidea.get_count(); i++)
      {

         auto pslide = slidea[i];

         pdrawer->slide_draw(pgraphics, pslide);

      }

   }


   pointer_array < slide > slide_show::get_drawable()
   {

      synchronous_lock synchronouslock(this->synchronization(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);

      pointer_array < slide > slidea;

      for (::collection::index iDrawable = 0; iDrawable < m_slidea.get_count(); iDrawable++)
      {

         m_slidea.element_at(iDrawable)->m_iDrawable = iDrawable;

         if (m_slidea.element_at(iDrawable)->m_opacitySlide.non_transparent())
         {

            slidea.add(m_slidea.element_at(iDrawable));

         }

      }

      slidea.predicate_sort([&](auto & pslide1, auto & pslide2)
      {

         return pslide1->m_iZOrder > pslide2->m_iZOrder;

      });

      return slidea;

   }


   bool slide_show::gcom_slide_show_step()
   {

      synchronous_lock synchronouslock(this->synchronization(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);

      if (m_iFadeIn < 0)
      {

         m_iFadeIn = 0;

      }

      if (m_iFadeIn >= m_slidea.get_count())
      {

         m_iFadeIn = 0;

      }

      bool bEnded = false;

      for (::collection::index iDrawable = 0; iDrawable < m_slidea.get_count(); iDrawable++)
      {

         m_slidea.element_at(iDrawable)->m_pshow = this;
         
         m_slidea.element_at(iDrawable)->m_iDrawable = iDrawable;

         m_slidea.element_at(iDrawable)->gcom_slide_step();

         if (m_slidea.element_at(iDrawable)->ended())
         {

            bEnded = true;

         }

      }

      if (bEnded)
      {

         m_iFadeIn++;

      }

      return ::success;

   }

   void slide_show::add_slide(const ::scoped_string & scopedstr)
   {

      m_slidea.add(øallocate slide(scopedstr));

   }


} // namespace gcom



