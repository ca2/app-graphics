#pragma once


// gcom - gcom Commons Component Acronym

namespace gcom
{

   class slide_show;

   class slide;

   class CLASS_DECL_GCOM slide_drawer :
      virtual public ::object
   {
   public:


      virtual void slide_draw(::draw2d::graphics_pointer & pgraphics, slide* pslide) = 0;

   };

} // namespace gcom
