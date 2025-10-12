// Created by Camilo!! <3tbs!! 2020-12-10 22:23 BRT CWB
// From hello_multiverse/hello_animation to gcom by camilo on 2022-11-14
//    13:34 <3ThomasBorregaardSorensen!!
#pragma once


namespace gcom
{

   class CLASS_DECL_GCOM image_show :
      virtual public matter
   { 
   public:


      bool                             m_bPendingImageChange;
      string                           m_strImage;
      ::image::image_pointer           m_pimage;
      ::image::image_pointer           m_pimageStretched;
      ::int_size                       m_sizeStretching;


      image_show();
      image_show(const ::image::image_pointer& pimage, const ::scoped_string & scopedstrImage);
      ~image_show() override;


      void on_initialize_particle() override;


      void step1();


   };


} // namespace gcom




