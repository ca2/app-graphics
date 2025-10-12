#pragma once


//#include "acme/prototype/geometry2d/_geometry2d.h"


namespace backimpact
{


   class CLASS_DECL_GCOM main :
      virtual public ::object
   {
   protected:


      ::pointer<user_interaction>         m_puserinteraction;
      ::pointer<thread>                   m_pthread;


   public:


      bool                                m_bFirstLoadImage = true;
      bool                                m_bEnabled;
      enum_state                          m_estate;
      ::pointer<gcom>                     m_pgcom;
      ::pointer<image_change>             m_pimagechange;
      ::pointer<transition_effect>        m_ptransitioneffect;
      ::pointer<visual_effect>            m_pvisualeffect;

      bool                                m_bInitialized;
      ::particle_pointer            m_pparticleStateMachine;

      bool                                m_bPendingLayout;


      main();
      ~main() override;

      
      virtual void initialize_backimpact_main(user_interaction * pbackimpactinterface);

      void set_backimpact_thread(thread* pthread);


      //virtual void shutdown();

      inline ::particle * gcom_pmutex() { return synchronization(); }

      ::pointer< ::particle > get_transfer_mutex();

      ::draw2d::graphics_pointer GetTransferDC();


      bool is_initialized();
      void enable(bool bEnable);
      bool is_enabled();
      void get_current_image_path(string & str);
      void set_current_image_placement(enum_image_placement eplacement);
      enum_image_placement get_current_image_placement();
      bool get_next_image_path(string & str);
      void set_background_change_period(const class time & time);
      void on_image_loaded(::image::image *pimage);
      void update_drawing_objects();
      void on_destroy() override;
      void on_resize();
      void set_state(enum_state estate);


      user_interaction * get_user_interaction();
      image_change * get_image_change();
      transition_effect * get_transition_effect();
      gcom * get_gcom();
      visual_effect * get_visual_effect();
      virtual thread * get_backimpact_thread();

      void _001OnDequeueMessage(wparam wparam, lparam lparam);

      virtual void image_change_post_event(enum_happening ehappening);

      void pulse_happening(enum_happening ehappening);

      void defer_check_layout();

      virtual bool update_buffer(const ::int_rectangle & rectangle);

      virtual void on_after_image_loaded();

      virtual bool on_render_buffer();
      virtual bool start_loading_next_image();
      virtual bool is_full_screen();


      DECLARE_MESSAGE_HANDLER(_001OnImageLoaded);


   };


} // namespace backimpact



