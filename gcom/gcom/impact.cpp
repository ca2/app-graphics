//
//  gcom_impact.cpp
//  gcom
//
//  Created by Camilo Sasuke Thomas Borregaard Soerensen on 30/10/16.
//  Copyright (c) 2016 Camilo Sasuke Thomas Borregaard Soerensen. All rights reserved.
//
#include "framework.h"

#include "aura/graphics/image/context.h"
#include "aura/message/user.h"
#include "axis/platform/application.h"
#include "impact.h"
#include "app-core/gcom/backimpact/main.h"
#include "app-core/gcom/backimpact/gcom.h"


namespace gcom
{



   impact::impact()
   {

      m_bEnableShowGcomBackground = true;

   }

   impact::~impact()
   {
   }


   //void impact::assert_ok() const
   //{

   //   ::backimpact::user_interaction::assert_ok();

   //}

   //void impact::dump(dump_context & dumpcontext) const
   //{
   //   backimpact::user_interaction::dump(dumpcontext);
   //}

   void impact::install_message_routing(::channel * pchannel)
   {

      ::backimpact::user_interaction::install_message_routing(pchannel);

      USER_MESSAGE_LINK(::user::e_message_create, pchannel, this, &impact::on_message_create);
      USER_MESSAGE_LINK(::user::e_message_destroy, pchannel, this, &impact::on_message_destroy);

   }


   void impact::on_message_create(::message::message * pmessage)
   {

      auto papp = get_app();

      ::pointer<::message::create>pcreate(pmessage);

      if (papp->payload("hello_bk").as_string().has_character())
      {

         ::property_set payloadFile;

         payloadFile["url"] = papp->payload("hello_bk");
         payloadFile["http_set"]["raw_http"] = true;
         payloadFile["http_set"]["disable_common_name_cert_check"] = true;

         m_pimageBk = image()->get_image(payloadFile);

      }

      m_timeBackgroundUpdate = get_main()->m_pgcom->m_timeGcomBackgroundUpdate;

      pcreate->previous();

      if (pcreate->m_bRet)
         return;

   }


   void impact::on_message_destroy(::message::message * pmessage)
   {


   }


   void impact::handle(::topic * ptopic, ::handler_context * phandlercontext)
   {

      ::backimpact::user_interaction::handle(ptopic, phandlercontext);

   }


   void impact::_001OnDraw(::draw2d::graphics_pointer & pgraphics)
   {

      ::backimpact::user_interaction::_001OnDraw(pgraphics);

      //_006OnDraw(pgraphics);

   }


   void impact::_006OnDraw(::draw2d::graphics_pointer & pgraphics)
   {


      auto rectangleX = this->rectangle();

      //if (m_bBkLoaded && m_pimage->is_set() && m_pimage->area() > 0)
      //{

      //   m_pimagePost->get_graphics()->BitBlt(
      //      0, 0, minimum(rectangleX.width(), m_pimage->width()),
      //      minimum(rectangleX.height(), m_pimage->height()),
      //      m_pimage->get_graphics());
      //   return;

      //}

      if (rectangleX.area() <= 0)
      {

         return;

      }

      //backimpact::user::impact::backimpact_render(pgraphics, rectangleX);

   }


   //::user::document * impact::get_document()
   //{

   //   return  (::user::impact::get_document());

   //}





   void impact::on_layout(::draw2d::graphics_pointer & pgraphics)
   {

      ::backimpact::user_interaction::on_layout(pgraphics);

   }




} // namespace asciimedia

















