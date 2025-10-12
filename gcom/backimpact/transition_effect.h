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
               double xyz;
            } m_squarypixelate;
            struct HEXAGON_PIXELATE
            {
               double xyz;
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
               double m_dRate;
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
         double      m_dAlpha;

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
         int_size                    m_size;
         int                     m_cx;
         int                     m_cy;
         double                  m_dRate;
         double                  m_dRateEx;
         double                  m_phi;
         int                     m_iRadius;

         
      public:
         
         
         int                              m_iStepRepeatCount;
         ::int_rectangle                  m_rectangle;
         int_point                        m_point;
         ::int_rectangle                  m_rectangleIn;
         ::int_rectangle                  m_rectangleOut;
         ::int_rectangle                  m_rectangleA;
         ::int_rectangle                  m_rectangleB;
         ::int_rectangle                  m_rectangleC;
         ::int_rectangle                  m_rectangleD;
         ::int_point_array                m_pointa;
         ::int_point_array                m_pointa2;
         ::double_polygon                    m_polygon;
         ::int_array_base                        m_ia;
         double_array                     m_da;
         ::collection::index                            m_iStep; // 0 - Inactive, >= 1 Active
         ::collection::count                          m_iStepCount; // -1 - Infinite, 0 and 1 - invalid, > 1 ::collection::count out
         ::collection::index                            m_iFrame;
         double                           m_dMinimumAlpha;
         
         
      public:


         tool_001();


         virtual void initialize_backimpact_helper(main * pmain);


         void Start(int cx, int cy, double dRate, double dRateEx);
//         void Go(ETransitionEffect eType);
         void Go();

         const int & cx() const { return m_size.cx(); }
         int & cx() { return m_size.cx(); }
         const int & cy() const { return m_size.cy(); }
         int & cy() { return m_size.cy(); }

         void GetSliceRect(
         ::int_rectangle *   lprect,
         EAlign   aelign);

         void GetSliceRect(
         ::int_rectangle *   lprect,
         EAlign   aelign,
         double   dRate,
         double   dRateMinus);

         void GetSliceRect(
         int      cx,
         int      cy,
         ::int_rectangle *   lprect,
         EAlign   aelign,
         double   dRate,
         double   dRateMinus);


         void GetRectAB(::int_rectangle * lprectA, ::int_rectangle * lprectB);
         void GetRectABCD(::int_rectangle * lprectA, ::int_rectangle * lprectB, ::int_rectangle * lprectC, ::int_rectangle * lprectD);
         void GetHorizontalHexagon(::int_rectangle * lprect, ::double_point * lppointa);
         int FindRandomEnglobingEllipse(const ::int_rectangle * lprect, ::int_rectangle * lprectCircle, int iMaxRand);
         int FindRandomEnglobingCircle(const ::int_rectangle * lprect, ::int_rectangle * lprectCircle, int iMaxRand);
         void GetRect(::int_point * lppoint, const ::int_rectangle & rectangle);
         void GetRect(::double_point * lppoint, const ::int_rectangle & rectangle);
         void GetRect(
         ::int_rectangle *   lprect);
         void GetRect(
         ::int_rectangle * lprect,
         double   dRate,
         double   dRateEx);
         void GetRect(
         ::int_rectangle *   lprect,
         EAlign   ealign);
         void GetRect(
         ::int_rectangle *   lprect,
         EAlign   ealign,
         double   dRate);
         void GetRect(
         ::int_rectangle *   lprect,
         EAlign   ealign,
         int      cx,
         int      cy,
         double   dRate);
         void GetRotateRect(int w, int h, int i, int j, double phi, ::double_point * lppoint);
         void GetRotateRect(int i, int j, ::double_point * lppoint);
         void TranslateRect(::int_rectangle * lprect, int w, int h, int i, int j);
         void GetRotateHexagon(int iRadius, int i, int j, double phi, ::double_point * lppoint);
         void GetRotateHexagon(int i, int j, ::double_point * lppoint);
         void Initialize(ETransitionEffect eeffect, int cx, int cy, transition_effect & effect);
         void Finalize();

//         static void GetSimplePolyBox(::int_rectangle * lprect, ::double_point * lppoint, int iCount);

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

      ::int_array_base                  m_iaVisual;
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
      virtual long long increment_reference_count() override;
      virtual long long decrement_reference_count() override;
      virtual long long release() override;
#endif

      void OnTimer();
      void Reset();
      bool IsActive();
      bool TestEnd();
      void End();
      void on_step(int_rectangle_array & rectaUpdate);


      void initialize_transition_effect();
      void _Final();
      void _Init();
      void Restart();

      virtual void one_second_timer();

      virtual void _thread_run_step();

   protected:
      void RunStepProcLevel1(int_rectangle_array & arra);
      void RunStepProcLevel2();

   };


} // namespace backimpact



