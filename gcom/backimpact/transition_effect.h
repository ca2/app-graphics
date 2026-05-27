#pragma once


#include "helper.h"
#include "acme/prototype/geometry2d/_collection.h"


namespace backimpact
{


   class CLASS_DECL_GCOM transition_effect :
      public helper
   {

      friend class user_interaction;
      friend class visual_effect;
   public:

      class tool_001 :
         public helper
      {
      public:
         
         union DATA
         {
            struct SQUARY_PIXELATE
            {
               ::f64 xyz;
            } m_squarypixelate;
            struct HEXAGON_PIXELATE
            {
               ::f64 xyz;
            } m_hexagonpixelate;
            struct SLICE_FRAME
            {
               int m_iImageCount;
               int m_iTileCount;
               int m_iFrameCount;
               int m_iGroupCount;
            } m_sliceframe;
            struct FLY
            {
               int m_iFrameCount;
               ::f64 m_dRate;
            } m_fly;
            struct TILES
            {
               int m_iTilesX;
               int m_iTilesY;
               int m_iTiles;
            } m_tiles;
            struct ALPHA_PIXELATE
            {
               int m_c1;
               int m_c2;
               int m_iSizeIndex;
            } m_alphapixelate;
            struct RADIAL_UNVEIL
            {
               int m_iRadius;
               int m_iRadiusIncrement;
            } m_radialunveil;
         } m_data;
         ::f64      m_dAlpha;

         enum EAlign
         {
            AlignTop,
            AlignBottom,
            AlignLeft,
            AlignRight,
            AlignLeftTop,
            AlignTopRight,
            AlignBottomLeft,
            AlignRightBottom,
            AlignCenter,
            AlignCenterOut,
         };


      protected:
         
         
         ETransitionEffect       m_etransitioneffect;
         i32_size                    m_size;
         int                     m_cx;
         int                     m_cy;
         ::f64                  m_dRate;
         ::f64                  m_dRateEx;
         ::f64                  m_phi;
         int                     m_iRadius;

         
      public:
         
         
         int                              m_iStepRepeatCount;
         ::i32_rectangle                  m_rectangle;
         i32_point                        m_point;
         ::i32_rectangle                  m_rectangleIn;
         ::i32_rectangle                  m_rectangleOut;
         ::i32_rectangle                  m_rectangleA;
         ::i32_rectangle                  m_rectangleB;
         ::i32_rectangle                  m_rectangleC;
         ::i32_rectangle                  m_rectangleD;
         ::i32_point_array                m_pointa;
         ::i32_point_array                m_pointa2;
         ::f64_polygon                    m_polygon;
         ::i32_array_base                        m_ia;
         f64_array                     m_da;
         ::collection::index                            m_iStep; // 0 - Inactive, >= 1 Active
         ::collection::count                          m_iStepCount; // -1 - Infinite, 0 and 1 - invalid, > 1 ::collection::count out
         ::collection::index                            m_iFrame;
         ::f64                           m_dMinimumAlpha;
         
         
      public:


         tool_001();


         virtual void initialize_backimpact_helper(main * pmain);


         void Start(int cx, int cy, ::f64 dRate, ::f64 dRateEx);
//         void Go(ETransitionEffect eType);
         void Go();

         const int & cx() const { return m_size.cx; }
         int & cx() { return m_size.cx; }
         const int & cy() const { return m_size.cy; }
         int & cy() { return m_size.cy; }

         void GetSliceRect(
         ::i32_rectangle *   lprect,
         EAlign   aelign);

         void GetSliceRect(
         ::i32_rectangle *   lprect,
         EAlign   aelign,
         ::f64   dRate,
         ::f64   dRateMinus);

         void GetSliceRect(
         int      cx,
         int      cy,
         ::i32_rectangle *   lprect,
         EAlign   aelign,
         ::f64   dRate,
         ::f64   dRateMinus);


         void GetRectAB(::i32_rectangle * lprectA, ::i32_rectangle * lprectB);
         void GetRectABCD(::i32_rectangle * lprectA, ::i32_rectangle * lprectB, ::i32_rectangle * lprectC, ::i32_rectangle * lprectD);
         void GetHorizontalHexagon(::i32_rectangle * lprect, ::f64_point * lppointa);
         int FindRandomEnglobingEllipse(const ::i32_rectangle * lprect, ::i32_rectangle * lprectCircle, int iMaxRand);
         int FindRandomEnglobingCircle(const ::i32_rectangle * lprect, ::i32_rectangle * lprectCircle, int iMaxRand);
         void GetRect(::i32_point * lppoint, const ::i32_rectangle & rectangle);
         void GetRect(::f64_point * lppoint, const ::i32_rectangle & rectangle);
         void GetRect(
         ::i32_rectangle *   lprect);
         void GetRect(
         ::i32_rectangle * lprect,
         ::f64   dRate,
         ::f64   dRateEx);
         void GetRect(
         ::i32_rectangle *   lprect,
         EAlign   ealign);
         void GetRect(
         ::i32_rectangle *   lprect,
         EAlign   ealign,
         ::f64   dRate);
         void GetRect(
         ::i32_rectangle *   lprect,
         EAlign   ealign,
         int      cx,
         int      cy,
         ::f64   dRate);
         void GetRotateRect(int w, int h, int i, int j, ::f64 phi, ::f64_point * lppoint);
         void GetRotateRect(int i, int j, ::f64_point * lppoint);
         void TranslateRect(::i32_rectangle * lprect, int w, int h, int i, int j);
         void GetRotateHexagon(int iRadius, int i, int j, ::f64 phi, ::f64_point * lppoint);
         void GetRotateHexagon(int i, int j, ::f64_point * lppoint);
         void Initialize(ETransitionEffect eeffect, int cx, int cy, transition_effect & effect);
         void Finalize();

//         static void GetSimplePolyBox(::i32_rectangle * lprect, ::f64_point * lppoint, int iCount);

      };

      friend class tool_001;

      bool                       m_bRun;
      bool                       m_bInitialized;
      bool                       m_bActive;
      class ::time                     m_timeTransitionStepLastRunTime;
      class ::time                     m_timeDelay;
      bool                       m_bTransitionStepRunActive;
      bool                       m_bTransitionStepPostActive;
      class ::time                     m_timeTransitionStepPostActive;

      class ::time                     m_timeLastTransitionTime;

      ::collection::index                      m_iVisual;

      int                        m_iInit;

      ::task_pointer             m_ptaskRunStep;

      ::collection::index                      m_iType;

      comparable_array < ETransitionEffect, ETransitionEffect >
                                 m_etypea;

      ::i32_array_base                  m_iaVisual;
      ::pointer<tool_001>      m_ptool001;
      bool                       m_bDestroy;

   public:


      transition_effect();
      virtual ~transition_effect();


      virtual void initialize_backimpact_helper(main * pmain) override;


      //virtual void finish(::object * pcontextobjectFinish = nullptr) override;
      void destroy() override;


      ETransitionEffect get_type();
      void StepBack();
      void StepNext();
      int CalcRepeatCount();
      ::gcom::e_direction GetDirection(ETransitionEffect eeffect);
      ::e_align GetAlign(ETransitionEffect effect);
      void DisableEffect(ETransitionEffect eeffect);
      void EnableEffect(ETransitionEffect eeffect);

      //void _Init(ETransitionEffect etypeNew);
#ifdef _DEBUG
      virtual ::i64 increment_reference_count() override;
      virtual ::i64 decrement_reference_count() override;
      virtual ::i64 release() override;
#endif

      void OnTimer();
      void Reset();
      bool IsActive();
      bool TestEnd();
      void End();
      void on_step(i32_rectangle_array & rectaUpdate);


      void initialize_transition_effect();
      void _Final();
      void _Init();
      void Restart();

      virtual void one_second_timer();

      virtual void _thread_run_step();

   protected:
      void RunStepProcLevel1(i32_rectangle_array & arra);
      void RunStepProcLevel2();

   };


} // namespace backimpact



