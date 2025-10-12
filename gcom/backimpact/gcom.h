#pragma once


#include "helper.h"


namespace backimpact
{


   class CLASS_DECL_GCOM gcom :
      public helper
   {
   public:


      class ::time                             m_timeGcomBackgroundUpdate;
      ::draw2d::graphics_pointer             m_dcScreen;
      ::int_rectangle                        m_rectangleFinalPlacement;


      map_base < enum_image, ::image::image_pointer >      m_mapImage;


      ::pointer < ::mutex >                                m_pmutex1Back;
      ::pointer < ::mutex >                                m_pmutex2Buffer;
      ::pointer < ::mutex >                                m_pmutex3Source;
      ::pointer < ::mutex >                                m_pmutex4Transfer;



      gcom();
      ~gcom() override;


      virtual void initialize_backimpact_helper(main * pmain) override;


      void update_objects();
      void on_destroy() override;
      void on_image_loaded(::image::image *pimage);

//#ifdef WINDOWS_DESKTOP
//      void LayoutBackBitmap(BITMAP * lpbmBack);
//#endif

      enum_image_placement GetDefaultPlacement();

      // buffer Level 2 consists of m_dcBackImpact2,
      // m_bmpBackImpact2 and m_bmpBackImpact2Old
      // It buffers the background as it is usually expected to
      // appear after transition effect has finished.
      // It is a layer between the source bitmap image
      // and the final background buffer.
      bool ApplyImagePlacementSettings();

      ::draw2d::graphics_pointer GetTransferDC(); // m_dcBackImpact2
      ::draw2d::graphics_pointer GetFrame1DC(); // m_dcBackImpact2
      ::draw2d::graphics_pointer GetBufferDC(); // m_dcBackImpact2
      ::draw2d::graphics_pointer GetSourceDC(); // m_dcBackImpactSource
      ::draw2d::graphics_pointer GetScreenDC(); // m_dcBackImpactSource

      ::image::image_pointer get_image(enum_image eimage);

      ::draw2d::bitmap_pointer GetTransferBitmap(); // m_bmpBackImpact2
      ::draw2d::bitmap_pointer GetFrame1Bitmap(); // m_bmpBackImpact2
      ::draw2d::bitmap_pointer GetBufferBitmap(); // m_bmpBackImpact2
      ::draw2d::bitmap_pointer GetSourceBitmap(); // m_bmpBackImpactSource

      virtual void on_create_image(::image::image *pimage,  enum_image eimage);


      ::draw2d::bitmap_pointer GetBackBitmap(); // m_bmpBackImpact
      ::draw2d::graphics_pointer GetBackDC(); // m_dcBackImpact

//         // Back tools keep the current background snapshot
//         ::draw2d::graphics_pointer               m_dcBack;
//         ::draw2d::bitmap            m_bmpBack;
//         ::draw2d::bitmap            m_bmpBackOld;

      // Back tools keep the final ::user::impact background snapshot
//         ::draw2d::graphics_pointer               m_dcBuffer;
//         ::draw2d::bitmap            m_bmpBuffer;
//         ::draw2d::bitmap            m_bmpBufferOld;

      // Used for fast buffer transfer
//         ::draw2d::graphics_pointer               m_dcTransfer;
//         ::draw2d::bitmap            m_bmpTransfer;
//         ::draw2d::bitmap            m_bmpTransferOld;

      // Source tools keep the original bitmap int its original int_size
//         ::draw2d::graphics_pointer               m_dcSource;
//         ::draw2d::bitmap            m_bmpSource;
//         ::draw2d::bitmap            m_bmpSourceOld;

      // Source tools keep the original bitmap int its original int_size
//         ::draw2d::graphics_pointer               m_dcFrame1;
//         ::draw2d::bitmap            m_bmpFrame1;
//       ::draw2d::bitmap            m_bmpFrame1Old;

      void GetFinalPlacement(::int_rectangle * lprect);
      void back_to_transfer();
      void buffer_to_back();


   };


} // namespace backimpact





