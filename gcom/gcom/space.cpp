#include "framework.h"
#include "space.h"
#include "acme/prototype/geometry2d/rectangle.h"
//#include "acme/prototype/geometry2d/_geometry2d.h"


namespace gcom
{


   space::space()
   {

   }

   space::~space()
   {

   }

   void space::align(::int_rectangle * lprectalign, ::int_rectangle * lprectspace, ::e_align ealign, double dRate)
   {
      int cx = 0;
      int cy = 0;
      switch(ealign)
      {
      case e_align_bottom_left:
         cx = lprectspace->left() - lprectalign->left();
         cy = lprectspace->bottom() - lprectalign->bottom();

         break;
      case e_align_left:
         cx = lprectspace->left() - lprectalign->left();
         break;
      case e_align_top:
         cy = lprectspace->top() - lprectalign->top();
         break;
      case e_align_right:
         cx = lprectspace->right() - lprectalign->right();
         break;
      case e_align_bottom:
         cy = lprectspace->bottom() - lprectalign->bottom();
         break;
      case e_align_top_left:
         cx = lprectspace->left() - lprectalign->left();
         cy = lprectspace->top() - lprectalign->top();
         break;
      case e_align_top_right:
         cx = lprectspace->right() - lprectalign->right();
         cy = lprectspace->top() - lprectalign->top();
         break;
      case e_align_bottom_right:
         cx = lprectspace->right() - lprectalign->right();
         cy = lprectspace->bottom() - lprectalign->bottom();
         break;
         default:
            break;
      }
      cx = (int) (cx * dRate);
      cy = (int) (cy * dRate);
      lprectalign->left()     += cx;
      lprectalign->right()    += cx;
      lprectalign->top()      += cy;
      lprectalign->bottom()   += cy;
   }


   void space::deviate(::int_rectangle * lprectDeviate, ::int_rectangle * lprectspace, e_direction edirection, double dRate)
   {

      int greekdeltax = lprectspace->right() - lprectspace->left();
      int greekdeltay = lprectspace->bottom() - lprectspace->top();
      greekdeltax = (int) (greekdeltax * dRate);
      greekdeltay = (int) (greekdeltay * dRate);

      switch(edirection)
      {
      case direction_left:
         greekdeltax = -greekdeltax;
         greekdeltay = 0;
         break;
      case direction_top:
         greekdeltax = 0;
         greekdeltay = - greekdeltay;
         break;
      case direction_right:
         greekdeltay = 0;
         break;
      case direction_bottom:
         greekdeltax = 0;
         break;
      case direction_top_left:
         greekdeltax = - greekdeltax;
         greekdeltay = - greekdeltay;
         break;
      case direction_top_right:
         greekdeltay = - greekdeltay;
         break;
      case direction_bottom_right:
         break;
      case direction_bottom_left:
         greekdeltax = -greekdeltax;
         break;
         default:
            break;
      }

      lprectDeviate->left()     += greekdeltax;
      lprectDeviate->right()    += greekdeltax;
      lprectDeviate->top()      += greekdeltay;
      lprectDeviate->bottom()   += greekdeltay;

   }

   void space::slice(::int_rectangle * lprectSliced1, ::int_rectangle * lprectSlicing, e_direction edirection, double dRate, double dRateEx)
   {
      int cx = lprectSlicing->right() - lprectSlicing->left();
      int cy = lprectSlicing->bottom() - lprectSlicing->top();

      switch(edirection)
      {
      case direction_left:
         lprectSliced1->left()     = lprectSlicing->left() + (int) (cx * (1.0 - dRateEx));
         lprectSliced1->right()    = lprectSlicing->left() + (int) (cx * (1.0 - dRate));
         lprectSliced1->top()      = lprectSlicing->top();
         lprectSliced1->bottom()   = lprectSlicing->bottom();
         break;
      case direction_top:
         lprectSliced1->left()     = lprectSlicing->left();
         lprectSliced1->right()    = lprectSlicing->right();
         lprectSliced1->top()      = lprectSlicing->top() + (int) (cy * (1.0 - dRateEx));
         lprectSliced1->bottom()   = lprectSlicing->top() + (int) (cy * (1.0 - dRate));
         break;
      case direction_right:
         lprectSliced1->left()     = lprectSlicing->left() + (int) (cx * dRate);
         lprectSliced1->right()    = lprectSlicing->left() + (int) (cx * dRateEx);
         lprectSliced1->top()      = lprectSlicing->top();
         lprectSliced1->bottom()   = lprectSlicing->bottom();
         break;
      case direction_bottom:
         lprectSliced1->left()     = lprectSlicing->left();
         lprectSliced1->right()    = lprectSlicing->right();
         lprectSliced1->top()      = lprectSlicing->top() + (int) (cy * dRate);
         lprectSliced1->bottom()   = lprectSlicing->top() + (int) (cy * dRateEx);
         break;
      case direction_top_left:
         lprectSliced1->left()     = lprectSlicing->left() + (int) (cx * (1.0 - dRateEx));
         lprectSliced1->right()    = lprectSlicing->left() + (int) (cx * (1.0 - dRate));
         lprectSliced1->top()      = lprectSlicing->top() + (int) (cy * (1.0 - dRateEx));
         lprectSliced1->bottom()   = lprectSlicing->top() + (int) (cy * (1.0 - dRate));
         break;
      case direction_top_right:
         lprectSliced1->left()     = lprectSlicing->left() + (int) (cx * dRate);
         lprectSliced1->right()    = lprectSlicing->left() + (int) (cx * dRateEx);
         lprectSliced1->top()      = lprectSlicing->top() + (int) (cy * (1.0 - dRateEx));
         lprectSliced1->bottom()   = lprectSlicing->top() + (int) (cy * (1.0 - dRate));
         break;
      case direction_bottom_right:
         lprectSliced1->left()     = lprectSlicing->left() + (int) (cx * dRate);
         lprectSliced1->right()    = lprectSlicing->left() + (int) (cx * dRateEx);
         lprectSliced1->top()      = lprectSlicing->top() + (int) (cy * dRate);
         lprectSliced1->bottom()   = lprectSlicing->top() + (int) (cy * dRateEx);
         break;
      case direction_bottom_left:
         lprectSliced1->left()     = lprectSlicing->left() + (int) (cx * (1.0 - dRateEx));
         lprectSliced1->right()    = lprectSlicing->left() + (int) (cx * (1.0 - dRate));
         lprectSliced1->top()      = lprectSlicing->top() + (int) (cy * dRate);
         lprectSliced1->bottom()   = lprectSlicing->top() + (int) (cy * dRateEx);
         break;
         default:
            break;
      }

   }


   void space::slice(::int_rectangle * lprectSliced1, ::int_rectangle * lprectSliced2, ::int_rectangle * lprectSliced3, ::int_rectangle * lprectSlicing, e_direction edirection, double dRate, double dRateEx)
   {

      slice(lprectSliced1, lprectSlicing, edirection, dRate, dRateEx);

      switch(edirection)
      {
      case direction_left:
         ASSERT(false);
         break;
      case direction_top:
         ASSERT(false);
         break;
      case direction_right:
         ASSERT(false);
         break;
      case direction_bottom:
         ASSERT(false);
         break;
      case direction_top_left:
         lprectSliced2->left()     = lprectSliced1->right();
         lprectSliced2->right()    = lprectSlicing->right();
         lprectSliced2->top()      = lprectSliced1->top();
         lprectSliced2->bottom()   = lprectSliced1->bottom();

         lprectSliced3->left()     = lprectSliced1->left();
         lprectSliced3->right()    = lprectSliced1->right();
         lprectSliced3->top()      = lprectSliced1->bottom();
         lprectSliced3->bottom()   = lprectSlicing->bottom();
         break;
      case direction_top_right:
         lprectSliced2->left()     = lprectSlicing->left();
         lprectSliced2->right()    = lprectSliced1->left();
         lprectSliced2->top()      = lprectSliced1->top();
         lprectSliced2->bottom()   = lprectSliced1->bottom();

         lprectSliced3->left()     = lprectSliced1->left();
         lprectSliced3->right()    = lprectSliced1->right();
         lprectSliced3->top()      = lprectSliced1->bottom();
         lprectSliced3->bottom()   = lprectSlicing->bottom();
         break;
      case direction_bottom_right:
         lprectSliced2->left()     = lprectSlicing->left();
         lprectSliced2->right()    = lprectSliced1->left();
         lprectSliced2->top()      = lprectSliced1->top();
         lprectSliced2->bottom()   = lprectSliced1->bottom();

         lprectSliced3->left()     = lprectSliced1->left();
         lprectSliced3->right()    = lprectSliced1->right();
         lprectSliced3->top()      = lprectSlicing->top();
         lprectSliced3->bottom()   = lprectSliced1->top();
         break;
      case direction_bottom_left:
         lprectSliced2->left()     = lprectSliced1->right();
         lprectSliced2->right()    = lprectSlicing->right();
         lprectSliced2->top()      = lprectSliced1->top();
         lprectSliced2->bottom()   = lprectSliced1->bottom();

         lprectSliced3->left()     = lprectSliced1->left();
         lprectSliced3->right()    = lprectSliced1->right();
         lprectSliced3->top()      = lprectSlicing->top();
         lprectSliced3->bottom()   = lprectSliced1->top();
         break;
         default:
            break;
      }

   }


   void space::get_center(::int_point * lppoint, ::int_rectangle * lprect)
   {

      lppoint->x() = (lprect->left() + lprect->right()) / 2;
      lppoint->y() = (lprect->top() + lprect->bottom()) / 2;

   }


   void space::scale(::int_rectangle * lprectScaling, double dRate)
   {

      ::int_point point;
      get_center(&point, lprectScaling);
      int cx = lprectScaling->right() - lprectScaling->left();
      int cy = lprectScaling->bottom() - lprectScaling->top();

      cx = (int) (cx * dRate);
      cy = (int) (cy * dRate);

      lprectScaling->left()     = point.x() - cx / 2;
      lprectScaling->right()    = lprectScaling->left() + cx;
      lprectScaling->top()      = point.y() - cy / 2;
      lprectScaling->bottom()   = lprectScaling->top() + cy;

   }


   void space::wipe_in(::int_rectangle * lprect1, ::int_rectangle * lprect2, ::int_rectangle * lprect3, ::int_rectangle * lprect4, ::int_rectangle * lprect, double dRate, double dRateEx)
   {

      ::int_rectangle rectangleIn(*lprect);
      scale(&rectangleIn, dRate);

      ::int_rectangle rectangleOut(*lprect);
      scale(&rectangleOut, dRateEx);

      lprect1->left()     = rectangleIn.left();
      lprect1->right()    = rectangleOut.right();
      lprect1->top()      = rectangleOut.top();
      lprect1->bottom()   = rectangleIn.top();

      lprect2->left()     = rectangleIn.right();
      lprect2->right()    = rectangleOut.right();
      lprect2->top()      = rectangleIn.top();
      lprect2->bottom()   = rectangleOut.bottom();

      lprect3->left()     = rectangleOut.left();
      lprect3->right()    = rectangleIn.right();
      lprect3->top()      = rectangleIn.bottom();
      lprect3->bottom()   = rectangleOut.bottom();

      lprect4->left()     = rectangleOut.left();
      lprect4->right()    = rectangleIn.left();
      lprect4->top()      = rectangleOut.top();
      lprect4->bottom()   = rectangleIn.bottom();

   }


} // namespace gcom








