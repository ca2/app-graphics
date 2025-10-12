#pragma once


namespace gcom
{

   class slide_show;

   class slide_drawer;

   class CLASS_DECL_GCOM slide :virtual public ::object
   {
   public:

      slide_show *   m_pshow;
      string         m_strPath;
      bool           m_bFadeIn;
      bool           m_bFadeOut;
      bool           m_bActive;
      bool           m_bEnable;
      class ::opacity            m_opacitySlide;
      class ::opacity            m_opacityStart;
      class ::time           m_timeStart;
      ::collection::index          m_iDrawable;
      ::collection::index          m_iZOrder;

      slide();
      slide(const ::scoped_string & scopedstrPath);
      ~slide();


      bool ended();
      virtual bool gcom_slide_step();

      void enable(bool bEnable = true);

   };

   class CLASS_DECL_GCOM slide_show :
      virtual public ::object
   {
   public:


      pointer_array < slide >     m_slidea;
      ::collection::index                      m_iFadeIn;
      slide_drawer *             m_pdrawer;
      class ::time                     m_timeFadePeriod;
      class ::time                     m_timeSlidePeriod;

      slide_show(slide_drawer * pdrawer);
      virtual ~slide_show();


      void add_slide(const ::scoped_string & scopedstr);

      virtual bool gcom_slide_show_step();

      void draw(::draw2d::graphics_pointer & pgraphics);

      pointer_array < slide > get_drawable();

   };


} // namespace gcom

