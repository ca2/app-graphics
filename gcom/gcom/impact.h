//
//  gcom_impact.hpp
//  gcom
//
//  Created by Camilo Sasuke Thomas Borregaard Soerensen on 30/10/16.
//  Copyright (c) 2016 Camilo Sasuke Thomas Borregaard Soerensen. All rights reserved.
//

#pragma once


#include "app-core/gcom/backimpact/user_interaction.h"


namespace gcom
{


   class CLASS_DECL_GCOM impact :
      virtual public ::backimpact::user_interaction
   {
   public:


      
      ::image::image_pointer        m_pimageBk;


      impact();
      virtual ~impact();


//      // void assert_ok() const override;
//      // void dump(dump_context & dumpcontext) const override;


      void install_message_routing(::channel * pchannel) override;

      virtual void _001OnDraw(::draw2d::graphics_pointer & pgraphics) override;

      virtual void _006OnDraw(::draw2d::graphics_pointer & pgraphics);

      void handle(::topic * ptopic, ::handler_context * phandlercontext) override;

//      ::user::document * get_document();

      virtual void on_layout(::draw2d::graphics_pointer & pgraphics) override;

      DECLARE_MESSAGE_HANDLER(on_message_create);
      DECLARE_MESSAGE_HANDLER(on_message_destroy);

      //virtual long long increment_reference_count() override
      //{
      //   return ::object::increment_reference_count();
      //}
      //virtual long long decrement_reference_count() override
      //{
      //   return ::object::decrement_reference_count();
      //}

   };


} // namespace asciimedia





















