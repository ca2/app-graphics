#pragma once


#include "acme/filesystem/filesystem/listing.h"
#include "acme/prototype/geometry2d/_collection.h"
#include "aura/user/user/interaction.h"


namespace backimpact
{

   
   class CLASS_DECL_GCOM user_interaction :
      virtual public ::user::interaction
   {
   public:


      enum EBackImpactWparam
      {
         BackImpactWparamImageChangeEvent,
      };


      ::pointer<thread>       m_pthread;
      task_pointer               m_ptaskTimer2;
      bool                       m_bTransferVoid;
      unsigned int                        m_dwTimerStep;


      //::file::set_pointer        m_spfilesetBackgroundImage;

      ::file::listing_base            m_listing;

      class ::time                 m_timeBackgroundUpdate;
      ::color::color             m_colorBackground;
      bool                       m_bShowGcomBackground;
      bool                       m_bEnableShowGcomBackground;


      user_interaction();
      ~user_interaction() override;


      virtual void initialize(::particle * pparticle) override;


      //void Shutdown();


      void install_message_routing(::channel * pchannel) override;


      ::particle * get_transfer_mutex();
      ::draw2d::graphics_pointer GetTransferDC();

      virtual bool _0723TransferVoid();

      virtual ::color::color get_background_color();
      void backimpact_render(::draw2d::graphics_pointer & pgraphics, int x, int y, int w, int h);
      void backimpact_render(::draw2d::graphics_pointer & pgraphics, const ::int_rectangle & rectangle);
      void enable(bool bEnable = true);
      bool is_enabled();
      void get_current_image_path(string & str);
      void set_current_image_placement(enum_image_placement eplacement);
      enum_image_placement get_current_image_placement();
      void set_background_change_period(const class time & time);
      main * get_main();
      void on_image_loaded(::image::image *pimage);
      void update_drawing_objects();
      void on_destroy() override;


      virtual void initialize_user_interaction();

      //virtual thread * get_backimpact_thread();

      virtual void backimpact_feedback(::image::image *pimage);
      virtual void backimpact_get_client_rect(::int_rectangle & rect);
      virtual ::shift_int backimpact_client_to_screen();
      virtual void backimpact_post_message(::user::enum_message eusermessage, ::wparam wparam, ::lparam lparam);
      virtual void backimpact_post_message(wparam wparam, lparam lparam);
      virtual void backimpact_update_screen();
      virtual void backimpact_get_data(data & data);
      virtual void backimpact_set_data(data & data);
      virtual void backimpact_update_screen(const ::int_rectangle & rectangle, unsigned int uRedraw);
      virtual void backimpact_update_screen(int_rectangle_array & recta, unsigned int uRedraw);

      virtual string backimpact_get_next_background_image_path(const ::scoped_string & scopedstrCurrentImagePath);
      virtual void backimpact_set_current_background_image_path(const ::scoped_string & scopedstr);

      virtual bool backimpact_get_destroy();
      virtual bool backimpact_get_image_placement(const ::scoped_string & scopedstr, enum_image_placement & eplacement);

//#ifdef WINDOWS_DESKTOP
//      virtual HENHMETAFILE backimpact_get_filling_HENHMETAFILE();
//#endif

      virtual void image_change_post_event(enum_happening ehappening);
      virtual bool update_buffer(const ::int_rectangle & rectangle);

      virtual void on_after_image_loaded();

      virtual ::user::interaction * backimpact_get_user_interaction();


      virtual bool on_render_buffer();
      virtual bool start_loading_next_image();
      virtual bool is_full_screen();


      // Windows message handler
      DECLARE_MESSAGE_HANDLER(_001OnBackImpactMessage);
      DECLARE_MESSAGE_HANDLER(_001OnDequeueMessage);
      DECLARE_MESSAGE_HANDLER(on_message_size);
      DECLARE_MESSAGE_HANDLER(on_message_create);
      DECLARE_MESSAGE_HANDLER(on_message_destroy);

      //      virtual void on_timer(::timer * ptimer) override;


      virtual void initialize_timer();


      virtual void _001OnDraw(::draw2d::graphics_pointer & pgraphics) override;

      
      virtual class ::time  backimpact_get_background_update_time();

      virtual bool show_gcom_background(bool bShow);
      virtual bool toggle_show_gcom_background();
      //virtual void install_message_routing(::channel* pchannel) override;
      void handle(::topic * ptopic, ::handler_context * phandlercontext) override;

      virtual void on_layout(::draw2d::graphics_pointer& pgraphics) override;


   };


} // namespace backimpact



