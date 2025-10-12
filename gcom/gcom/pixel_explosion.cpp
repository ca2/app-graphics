#include "framework.h"
#include "pixel_explosion.h"
#include <math.h>
#include "acme/graphics/image/image32.h"


namespace gcom
{


   pixel_explosion::pixel_explosion()
   {

      m_iScan = 0;
      m_iWidth = 0;
      m_iHeight = 0;

      m_bDrawWithLight = true;
      m_iLightModifier = 1;
      m_iHpage = 0;
      m_density = 5;
   }

   pixel_explosion::~pixel_explosion()
   {
      // Cleanup
   }
   void pixel_explosion::create(int iWidth,int iHeight, int iScan, color_indexes indexes)
   {
      m_pointa.set_size(iWidth * iHeight);
      m_pointaM.set_size(iWidth * iHeight);
      m_colorindexes = indexes;


      m_iWidth = iWidth;
      m_iHeight = iHeight;
      m_iScan = iScan;

      // set our page to 0
      m_iHpage = 0;

   }
   
   
   void pixel_explosion::FlattenWater()
   {
   
      // clear our height fields
   }


   void pixel_explosion::to(image32_t * pSrcImage, image32_t * pTargetImage)
   {
      int i = 0;

      // Yes they have to be the same size...(for now)
      DrawWaterNoLight(m_iHpage,pSrcImage,pTargetImage);
      if(i == 2)
      {
         Calc2(m_iHpage, (int)m_density);
      }
      else
      {
         CalcWater(m_iHpage, (int) m_density);
      }


      m_iHpage ^= 1;

   }

   void pixel_explosion::CalcWater(int npage, int density)
   {

      if (m_pointa.is_empty())
      {

         return;

      }

      __UNREFERENCED_PARAMETER(npage);
      __UNREFERENCED_PARAMETER(density);
      int imin = - m_iWidth / 2;
      int imax = m_iWidth / 2;
      int jmin = - m_iHeight / 2;
      int jmax = m_iHeight / 2;
      int i, j;
      int x, y;

      double dr;
      double dcos;
      double dsin;



      double dmax = minimum(m_iWidth, m_iHeight);

      double dm = dmax / 3;

      if(m_minradius > 0.0)
      {
         dm = m_minradius;
      }


      double m, n;


      int k = 0;
      for(y = 0, j = jmin; j < jmax; j++, y++)
      {
         for(x = 0, i = imin; i < imax; i++, x++)
         {
            m = m_pointa[k].x();
            n = m_pointa[k].y();


            dr = sqrt(m * m + n * n);
            double d;
            if(fabs(dr) < dm)
            {
               double da = rand() * 3.1415 * 2 / RAND_MAX;
               dcos = cos(da);
               dsin = sin(da);
               dr = rand() * dm * 1.2 / RAND_MAX;;
               d = 1.0;
            }
            else
            {

               dcos = m / dr;
               dsin = n / dr;
            }

            d = dr / dmax;


            //d = sqrt(d);

            // i = x + y * width
            m_pointa[k].x() = dr * dcos * (rand() * density * d / RAND_MAX + 1.0);
            m_pointa[k].y() = dr * dsin * (rand() * density * d / RAND_MAX + 1.0);
            //m_pointa[k].x() = dr * dcos * (1000 * 0.2 * d / RAND_MAX + 1.0);
            //m_pointa[k].y() = dr * dsin * (1000 * 0.2 * d / RAND_MAX + 1.0);
            //m_pointa[k].x() = dr * dcos * 1.2 * d;
            //m_pointa[k].y() = dr * dsin * 1.2 * d;
            k++;
         }
      }


   }

   void pixel_explosion::Calc2(int npage, int density)
   {
      __UNREFERENCED_PARAMETER(npage);
      __UNREFERENCED_PARAMETER(density);
      m_z *= 1.1;

      int imin = - m_iWidth / 2;
      int imax = m_iWidth / 2;
      int jmin = - m_iHeight / 2;
      int jmax = m_iHeight / 2;
      int i, j;
      int x, y;

      double dr;
      double dcos;
      double dsin;

      double dmax = sqrt((double) (imax * imax + jmax * jmax));

      if(m_z > dmax) m_z = dmax;
      //double dmax = minimum(m_iWidth, m_iHeight);
      //double dmax = maximum(m_iWidth, m_iHeight) * 2;

      //double m, n, o;

      double m, n;


      int k = 0;
      for(y = 0, j = jmin; j < jmax; j++, y++)
      {
         for(x = 0, i = imin; i < imax; i++, x++)
         {
            m = m_pointa[k].x();
            n = m_pointa[k].y();
            //o = m_pointa[k].z();


            dr = sqrt(m * m + n * n);
            double d = m_z;
            if(dr < 10.0)
            {
               double da = rand() * 3.1415 * 2 / RAND_MAX;
               dcos = cos(da);
               dsin = sin(da);
               //dr = 1.0;
               dr = maximum(2.0, d);
               //o = 0;
            }
            else
            {

               dcos = m / dr;
               dsin = n / dr;
            }



            //d = sqrt(d);

            //int rnd =
            // i = x + y * width
            m_pointa[k].x() = dr * dcos * (rand() * 0.5 * d / RAND_MAX + 1.0);
            m_pointa[k].y() = dr * dsin * (rand() * 0.5 * d / RAND_MAX + 1.0);
//               m_pointa[k].x() = i * d;
//             m_pointa[k].y() = j * d;
            m_pointa[k].x() = dr * dcos * 1.2 * d;
            m_pointa[k].y() = dr * dsin * 1.2 * d;
            k++;
         }
      }


   }
   void pixel_explosion::SmoothWater(int npage)
   {
      __UNREFERENCED_PARAMETER(npage);
   }

   void pixel_explosion::CalcWaterBigFilter(int npage, int density)
   {
      __UNREFERENCED_PARAMETER(npage);
      __UNREFERENCED_PARAMETER(density);
   }

   void pixel_explosion::Reset()
   {
      m_z = 0.1;
      int imin = - m_iWidth / 2;
      int imax = m_iWidth / 2;
      int jmin = - m_iHeight / 2;
      int jmax = m_iHeight / 2;

      double maximum = imax * imax + jmax * jmax;
      //double maximum = maximum(imax, jmax) * 2;
      //   maximum = 2 * maximum;

      int k = 0;
      int i, j;
      for(j = jmin; j < jmax; j++)
      {
         for(i = imin; i < imax; i++)
         {

            m_pointaM[k].x() = i;
            m_pointaM[k].x() = j;
            m_pointaM[k].z() = sqrt(maximum - (i * i + j * j));
            m_pointa[k].x() = i;
            m_pointa[k].y() = j;
            m_pointa[k].z() = m_pointaM[k].z();
            k++;
         }
      }

   }

   void pixel_explosion::HeightBox (int x, int y, int radius, int height, int page)
   {
      __UNREFERENCED_PARAMETER(x);
      __UNREFERENCED_PARAMETER(y);
      __UNREFERENCED_PARAMETER(radius);
      __UNREFERENCED_PARAMETER(height);
      __UNREFERENCED_PARAMETER(page);
   }


   void pixel_explosion::WarpBlob(int x, int y, int radius, int height, int page)
   {
      __UNREFERENCED_PARAMETER(x);
      __UNREFERENCED_PARAMETER(y);
      __UNREFERENCED_PARAMETER(radius);
      __UNREFERENCED_PARAMETER(height);
      __UNREFERENCED_PARAMETER(page);

   }


   void pixel_explosion::SineBlob(int x, int y, int radius, int height, int page)
   {

      __UNREFERENCED_PARAMETER(x);
      __UNREFERENCED_PARAMETER(y);
      __UNREFERENCED_PARAMETER(radius);
      __UNREFERENCED_PARAMETER(height);
      __UNREFERENCED_PARAMETER(page);

   }


   void pixel_explosion::DrawWaterNoLight(int page, image32_t * pSrcImage, image32_t * pTargetImage)
   {
      __UNREFERENCED_PARAMETER(page);
      //  int ox, oy;
      int x, y;

      int mx = m_iWidth / 2;
      int my = m_iHeight / 2;


//        int offset=m_iWidth + 1;

      int iW = m_iScan / sizeof(image32_t);

      for (y = 0; y < m_iHeight; y++)
      {
         for (x = 0; x < m_iWidth; x++)
         {
            int i = x + y * iW;

            if (i >= m_pointa.get_size())
            {

               continue;

            }

            int newx = (int) m_pointa[i].x() + mx;
            if(newx < 0 || newx >= iW)
               continue;
            int newy = (int) m_pointa[i].y() + my;
            if(newy < 0 || newy >= m_iHeight)
               continue;

            pTargetImage[newx + newy * m_iWidth] =
            pSrcImage[x + y * m_iWidth];

         }
      }
   }

   image32_t pixel_explosion::GetShiftedColor(image32_t color,int shift)
   {
      int R;
      int G;
      int B;
      int ir;
      int ig;
      int ib;

      R = color.byte_red(m_colorindexes)-shift;
      G = color.byte_green(m_colorindexes)-shift;
      B = color.byte_blue(m_colorindexes)-shift;

      ir = (R < 0) ? 0 : (R > 255) ? 255 : R;
      ig = (G < 0) ? 0 : (G > 255) ? 255 : G;
      ib = (B < 0) ? 0 : (B > 255) ? 255 : B;

      return { argb(255, ir,ig,ib), m_colorindexes };
   }


} // namespace gcom



