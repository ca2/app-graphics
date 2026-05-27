#pragma once


#include "axis/graphics/render/water_routine.h"
#include "app-graphics/gcom/gcom/pixel_explosion.h"
#include "helper.h"


namespace backimpact
{


   class resize;


   class visual_effect :
      public helper
   {
   public:


      struct tool_001
      {
         ::i32_size                    m_size;
         ::i32_rectangle               m_rectangleX;
         int                           m_iPage;
         int                           m_iFrame;
         ::f64                        m_dMod;
         ::f64                        m_dMinSize;
         ::f64                        m_dAlpha;

         // const int & cx() const { return m_size.cx; }
         // int & cx() { return m_size.cx; }
         // const int & cy() const { return m_size.cy; }
         // int & cy() { return m_size.cy; }

      };

      struct random_grow
      {
         ::f64                        i;
         ::f64                        imax;
         ::f64                        lbound;
         ::f64                        maxlbound;
         ::f64                        maxubound;
      public:
         bool Step();
      };


      struct color_track1
      {
         random_grow                   m_rndgrowDirectionLatency;
         ::f64                        m_dDirection;
         ::f64                        m_dDirectionAddUp;
         i32_point                     m_pointColorTrack;
         random_grow                   m_rndgrowVelocity;
         ::f64                        m_dCos;
         ::f64                        m_dSin;
         ::f64                        m_dSize;
      };


      int                              m_iVisual;
      bool                             m_bGrowColor0;
      ::color::color                   m_color;
      int                              m_iGrowColor;
      int                              m_iGrowMax;
      ::f64                           m_dAngle;

      ::color::color                   m_color2;
      int                              m_iGrowColor2;
      int                              m_iGrowMax2;

      ::color::color                   m_color3;
      int                              m_iGrowColor3;
      int                              m_iGrowMax3;

      ::color::color                   m_color4;
      int                              m_iGrowColor4;
      int                              m_iGrowMax4;

      i32_point                        m_pointColorTrack;
      random_grow                      m_rndgrowVelocity;
      ::f64                           m_dDirection;
      ::f64                           m_dDirectionAddUp;
      random_grow                      m_rndgrowDirectionLatency;

      random_grow                      m_rndgrowAngleOffsetLatency;
      ::f64                           m_dAngleOffset;
      ::f64                           m_dAngleOffsetAddUp;

      array < color_track1 >           m_colortrack1a;

      tool_001                         m_tool1;

      f64_point_array               m_pointa;
      f64_point_array               m_pointa1;
      f64_point_array               m_pointa2;

      ::gcom::pixel_explosion          m_explosion;


      draw2d::water_routine            m_water;
      ::pointer<class resizer>       m_presizer;


      visual_effect();
      virtual ~visual_effect();


      virtual void initialize_backimpact_helper(main * pmain) override;


      int get_effect_count();
      void InitialRender();
      void on_render_buffer(i32_rectangle_array & recta);

      void RenderRotateBlend(i32_rectangle_array & recta);
      void RenderNPRotateBlend(i32_rectangle_array & recta);
      void RenderNPRotateColorBlend(i32_rectangle_array & recta);
      void RenderNPRotateTrackColorBlend(i32_rectangle_array & recta);
      void RenderRotateEx1(i32_rectangle_array & recta);
      void RenderRotateEx2(i32_rectangle_array & recta);
      void RenderRotateEx3(i32_rectangle_array & recta);
      void RenderRotateEx4(i32_rectangle_array & recta);
      void RenderRain1(i32_rectangle_array & recta);
      void RenderRotateEx5(i32_rectangle_array & recta);
      void RenderRotateEx6(i32_rectangle_array & recta);
      void RenderRotateEx7(i32_rectangle_array & recta);
      void RenderRotateEx8(i32_rectangle_array & recta);
      void RenderExpand4(i32_rectangle_array & recta);
      void RenderExpand5(i32_rectangle_array & recta);
      void RenderExpand6(i32_rectangle_array & recta);
      void RenderExpand7(i32_rectangle_array & recta);
      void RenderExpand8(i32_rectangle_array & recta);
      void RenderPixelExplosion(i32_rectangle_array & recta);
      void RenderPixelExplosion2(i32_rectangle_array & recta);
      void RenderAlphaPixelExplosion(i32_rectangle_array & recta);





      static void Constraint001(i32_point & point, const ::i32_rectangle & rectangle, ::f64 & dDirection);
      static void Constraint001(i32_point & point, const ::i32_rectangle * lpcrectIn, const ::i32_rectangle * lpcrectOut, ::f64 & dDirection);
      static void TrackDirection001(::f64 &direction, ::f64 & directionAddUp, random_grow & randomgrow);


      static void StepGrow001(::color::color & color, int & iGrowColor, int & iGrowMax);
      static void StepGrow034(::color::color & color, int & iGrowColor, int & iGrowMax);
      static void StepGrow001(::color::color & color, int & iGrowColor, int & iGrowMax, bool & bGrowColor);



   };


} // namespace backbiew



