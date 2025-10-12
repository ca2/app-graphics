#pragma once


#include "axis/graphics/render/water_routine.h"
#include "app-core/gcom/gcom/pixel_explosion.h"
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
         ::int_size                    m_size;
         ::int_rectangle               m_rectangleX;
         int                           m_iPage;
         int                           m_iFrame;
         double                        m_dMod;
         double                        m_dMinSize;
         double                        m_dAlpha;

         const int & cx() const { return m_size.cx(); }
         int & cx() { return m_size.cx(); }
         const int & cy() const { return m_size.cy(); }
         int & cy() { return m_size.cy(); }

      };

      struct random_grow
      {
         double                        i;
         double                        imax;
         double                        lbound;
         double                        maxlbound;
         double                        maxubound;
      public:
         bool Step();
      };


      struct color_track1
      {
         random_grow                   m_rndgrowDirectionLatency;
         double                        m_dDirection;
         double                        m_dDirectionAddUp;
         int_point                     m_pointColorTrack;
         random_grow                   m_rndgrowVelocity;
         double                        m_dCos;
         double                        m_dSin;
         double                        m_dSize;
      };


      int                              m_iVisual;
      bool                             m_bGrowColor0;
      ::color::color                   m_color;
      int                              m_iGrowColor;
      int                              m_iGrowMax;
      double                           m_dAngle;

      ::color::color                   m_color2;
      int                              m_iGrowColor2;
      int                              m_iGrowMax2;

      ::color::color                   m_color3;
      int                              m_iGrowColor3;
      int                              m_iGrowMax3;

      ::color::color                   m_color4;
      int                              m_iGrowColor4;
      int                              m_iGrowMax4;

      int_point                        m_pointColorTrack;
      random_grow                      m_rndgrowVelocity;
      double                           m_dDirection;
      double                           m_dDirectionAddUp;
      random_grow                      m_rndgrowDirectionLatency;

      random_grow                      m_rndgrowAngleOffsetLatency;
      double                           m_dAngleOffset;
      double                           m_dAngleOffsetAddUp;

      array < color_track1 >           m_colortrack1a;

      tool_001                         m_tool1;

      double_point_array               m_pointa;
      double_point_array               m_pointa1;
      double_point_array               m_pointa2;

      ::gcom::pixel_explosion          m_explosion;


      draw2d::water_routine            m_water;
      ::pointer<class resizer>       m_presizer;


      visual_effect();
      virtual ~visual_effect();


      virtual void initialize_backimpact_helper(main * pmain) override;


      int get_effect_count();
      void InitialRender();
      void on_render_buffer(int_rectangle_array & recta);

      void RenderRotateBlend(int_rectangle_array & recta);
      void RenderNPRotateBlend(int_rectangle_array & recta);
      void RenderNPRotateColorBlend(int_rectangle_array & recta);
      void RenderNPRotateTrackColorBlend(int_rectangle_array & recta);
      void RenderRotateEx1(int_rectangle_array & recta);
      void RenderRotateEx2(int_rectangle_array & recta);
      void RenderRotateEx3(int_rectangle_array & recta);
      void RenderRotateEx4(int_rectangle_array & recta);
      void RenderRain1(int_rectangle_array & recta);
      void RenderRotateEx5(int_rectangle_array & recta);
      void RenderRotateEx6(int_rectangle_array & recta);
      void RenderRotateEx7(int_rectangle_array & recta);
      void RenderRotateEx8(int_rectangle_array & recta);
      void RenderExpand4(int_rectangle_array & recta);
      void RenderExpand5(int_rectangle_array & recta);
      void RenderExpand6(int_rectangle_array & recta);
      void RenderExpand7(int_rectangle_array & recta);
      void RenderExpand8(int_rectangle_array & recta);
      void RenderPixelExplosion(int_rectangle_array & recta);
      void RenderPixelExplosion2(int_rectangle_array & recta);
      void RenderAlphaPixelExplosion(int_rectangle_array & recta);





      static void Constraint001(int_point & point, const ::int_rectangle & rectangle, double & dDirection);
      static void Constraint001(int_point & point, const ::int_rectangle * lpcrectIn, const ::int_rectangle * lpcrectOut, double & dDirection);
      static void TrackDirection001(double &direction, double & directionAddUp, random_grow & randomgrow);


      static void StepGrow001(::color::color & color, int & iGrowColor, int & iGrowMax);
      static void StepGrow034(::color::color & color, int & iGrowColor, int & iGrowMax);
      static void StepGrow001(::color::color & color, int & iGrowColor, int & iGrowMax, bool & bGrowColor);



   };


} // namespace backbiew



