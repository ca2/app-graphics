#pragma once


#include "double3d_point.h"


namespace gcom
{


   class pixel_explosion
   {
   public:

      
      int                       m_iScan;
      int                       m_iWidth;
      int                       m_iHeight;
      bool                          m_bDrawWithLight;
      int                                 m_iLightModifier;
      int                                 m_iHpage; // The current heightfield
      double                              m_density; // The water density - can change the density...
      double                              m_minradius;
      double                              m_z;
      raw_array_base < double3d_point >   m_pointaM;
      raw_array_base  < double3d_point >  m_pointa;
      color_indexes                       m_colorindexes;


      pixel_explosion();
      virtual ~pixel_explosion();

      void create(int iWidth,int iHeight, int iScan, color_indexes indexes);
      void to(image32_t * pSrcImage, image32_t * pTargetImage);

      void CalcWater(int npage, int density);
      void Calc2(int npage, int density);
      void SmoothWater(int npage);
      void FlattenWater();

      void SineBlob(int x, int y, int radius, int height, int page);
      void WarpBlob(int x, int y, int radius, int height, int page);
      void HeightBox (int x, int y, int radius, int height, int page);
      void Reset();
      void CalcWaterBigFilter(int npage, int density);

      void DrawWaterNoLight(int page, image32_t * pSrcImage, image32_t * pTargetImage);
      //void DrawWaterWithLight(int page, int LightModifier,unsigned int* pSrcImage,unsigned int* pTargetImage);
      image32_t GetShiftedColor(image32_t color,int shift);

   };


} // namespace gcom




