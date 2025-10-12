#pragma once


namespace gcom
{


   class space
   {
   public:



      space();
      ~space();


      void align(::int_rectangle * lprectAlign, ::int_rectangle * lprectSpace, ::e_align ealign, double dRate);
      void deviate(::int_rectangle * lprectDeviate, ::int_rectangle * lprectSpace, e_direction edirection, double dRate);
      void slice(::int_rectangle * lprectSliced, ::int_rectangle * lprectSlicing, e_direction edirection, double dRate, double dRateEx);
      void slice(::int_rectangle * lprectSliced1, ::int_rectangle * lprectSliced2, ::int_rectangle * lprectSliced3, ::int_rectangle * lprectSlicing, e_direction edirection, double dRate, double dRateEx);


      void scale(::int_rectangle * lprect, double dRate);
      void wipe_in(::int_rectangle * lprect1, ::int_rectangle * lprect2, ::int_rectangle * lprect3, ::int_rectangle * lprect4, ::int_rectangle * lprect, double dRate, double dRateEx);


      void get_center(::int_point * lppoint, ::int_rectangle * lprect);


   };


} // namespace gcom




