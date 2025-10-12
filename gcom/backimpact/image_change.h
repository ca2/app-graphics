#pragma once


#include "helper.h"
#include "acme/parallelization/happening.h"


namespace backimpact
{


   class CLASS_DECL_GCOM image_change :
      public helper
   {
   public:


      class ::time                        m_timeBackgroundChangePeriod;
      enum_image_placement                m_eplacement;

      happening                           m_happeningImageChangeFinish;
      unsigned int                        m_dwLoadCounter;
      class ::time                        m_timeBackgroundLastChange;
      class ::time                        m_timeBackgroundPreTransitionStart;
      bool                                m_bLastLoadImageSynch;
      string                              m_strCurrentImagePath;
      class ::time                        m_timeLoadStart;
      bool                                m_bLoadImage;
      bool                                m_bInitialPath;

      ::image::image_pointer              m_pimage;



      image_change();
      virtual ~image_change();


      virtual void initialize_backimpact_helper(main * pmain) override;


      void OnHappeningLoadNow();

      bool start_loading_next_image();
      bool LoadImageSync();
      bool LoadImageSync(const ::scoped_string & scopedstrImagePath);
      bool LoadImageAsync(const ::scoped_string & scopedstrImagePath);
      bool LoadImageAsync();

      class ::time get_background_change_period();
      void set_background_change_period(const class time & time);


   };


} // namespace backimpact





