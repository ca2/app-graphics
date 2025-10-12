// Created by Camilo!! <3tbs, Mummi and bilbo!! 2020-12-10 22:25 BRT CWB
#include "framework.h"
#include "image_show.h"
#include "acme/platform/system.h"
#include "aura/graphics/image/image.h"


namespace gcom
{

   image_show::image_show()
   {

      m_bPendingImageChange = true;

      defer_create_synchronization();

   }


   image_show::image_show(const ::image::image_pointer& pimage, const ::scoped_string & scopedstrImage) :
      m_pimage(pimage),
      m_strImage(scopedstrImage)
   {

      m_bPendingImageChange = true;

      defer_create_synchronization();

   }



   image_show::~image_show()
   {


   }


   void image_show::on_initialize_particle()
   {

      //auto estatus = 

      ::matter::on_initialize_particle();

      //estatus = 

      system()->øconstruct(m_pimage);

      //estatus = 

      system()->øconstruct(m_pimageStretched);

      //return estatus;

   }


   void image_show::step1()
   {

      //synchronous_lock synchronouslock(this->synchronization(), DEFAULT_SYNCHRONOUS_LOCK_SUFFIX);

      //if (m_bPendingImageChange)
      //{

      //      m_bPendingImageChange = false;

      //      string strFork = m_strImage.c_str();

      //      synchronouslock.unlock();

      //      fork([this, strFork]()
      //         {

      //            get_document()->on_open_document(strFork);

      //         });

      //   }



      //}

   }


} // namespace gcom


