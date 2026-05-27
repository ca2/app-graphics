#pragma once


namespace gcom
{


   class space
   {
   public:



      space();
      ~space();


      void align(::i32_rectangle * lprectAlign, ::i32_rectangle * lprectSpace, ::e_align ealign, ::f64 dRate);
      void deviate(::i32_rectangle * lprectDeviate, ::i32_rectangle * lprectSpace, e_direction edirection, ::f64 dRate);
      void slice(::i32_rectangle * lprectSliced, ::i32_rectangle * lprectSlicing, e_direction edirection, ::f64 dRate, ::f64 dRateEx);
      void slice(::i32_rectangle * lprectSliced1, ::i32_rectangle * lprectSliced2, ::i32_rectangle * lprectSliced3, ::i32_rectangle * lprectSlicing, e_direction edirection, ::f64 dRate, ::f64 dRateEx);


      void scale(::i32_rectangle * lprect, ::f64 dRate);
      void wipe_in(::i32_rectangle * lprect1, ::i32_rectangle * lprect2, ::i32_rectangle * lprect3, ::i32_rectangle * lprect4, ::i32_rectangle * lprect, ::f64 dRate, ::f64 dRateEx);


      void get_center(::i32_point * lppoint, ::i32_rectangle * lprect);


   };


} // namespace gcom




