#pragma once


namespace gcom
{


   class double3d_point :
      public ::sequence_type < double, 3 >
   {
   public:


      //double x;
      //double y;
      //double z;

      const double & x() const { return this->a(); }
      double & x() { return this->a(); }
      const double & y() const { return this->b(); }
      double & y() { return this->b(); }
      const double & z() const { return this->c(); }
      double & z() { return this->c(); }

   };


} // namespace gcom




