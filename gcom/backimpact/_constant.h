#pragma once


namespace backimpact
{


   enum enum_state
   {

      StateTiming,
      e_state_loading,
      StatePreTransition,
      StatePreTransitionResize,
      StateInTransitionEffect,
      StateFinish,

   };


   enum enum_happening
   {

      e_event_timer,
      e_event_loaded,
      e_event_pre_transition_finished,
      e_event_transition_effect_finished,
      e_event_load_failed,
      e_event_load_now,
      e_event_flag_changed,
      e_event_resize,

   };


   enum enum_image_placement
   {

      ImagePlacementZoomAll,
      ImagePlacementStretch,
      ImagePlacementTile,
      ImagePlacementZoomExtend

   };


   enum enum_data
   {

      e_data_enhanced_metafile,
      e_data_image_placement,
      e_data_next_image_path,
      e_data_current_image_path,
      e_data_client_rect,

   };


   enum enum_image
   {

      e_image_temp1,
      e_image_temp2,
      e_image_temp3,
      e_image_temp4,
      e_image_temp5,
      e_image_temp6,
      e_image_temp7,
      e_image_temp8,
      e_image_temp9,
      e_image_temp10,
      e_image_buffer = 1000,
      e_image_frame1,
      e_image_source,
      e_image_final,
      e_image_back,
      e_image_transfer,
      e_image_radial_center_alpha,
      e_image_radial_border_alpha,

   };



   enum ETransitionEffect
   {

      TransitionEffectInvalid,
      TransitionEffectStart,
      TransitionEffectSingleStep,
      TransitionEffectSquarypixelate_,
      TransitionEffectHexagonalpixelate_,
      TransitionEffectCirclypixelate_,
      TransitionEffectEllipsoidalpixelate_,
      TransitionEffectLinearFadingTopBottom,
      TransitionEffectLinearFadingBottomTop,
      TransitionEffectLinearFadingLeftRight,
      TransitionEffectLinearFadingRightLeft,
      TransitionEffectAccumulLinearFadingTopBottom,
      TransitionEffectAccumulLinearFadingBottomTop,
      TransitionEffectAccumulLinearFadingLeftRight,
      TransitionEffectAccumulLinearFadingRightLeft,
      TransitionEffectFlyInTopBottom,
      TransitionEffectFlyInBottomTop,
      TransitionEffectFlyInLeftRight,
      TransitionEffectFlyInRightLeft,
      TransitionEffectFlyInLeftBottom,
      TransitionEffectFlyInLeftTop,
      TransitionEffectFlyInRightBottom,
      TransitionEffectFlyInRightTop,
      TransitionEffectWipeBottom,
      TransitionEffectWipeTop,
      TransitionEffectWipeRight,
      TransitionEffectWipeLeft,
      TransitionEffectWipeMidHorizontal,
      TransitionEffectWipeTopLeft,
      TransitionEffectWipeTopRight,
      TransitionEffectWipeBottomLeft,
      TransitionEffectWipeBottomRight,
      TransitionEffectWipeMidVertical,
      TransitionEffectWipeCenter,
      TransitionEffectWipeIn,
      TransitionEffectScaleBottom,
      TransitionEffectScaleTop,
      TransitionEffectScaleRight,
      TransitionEffectScaleLeft,
      TransitionEffectScaleTopLeft,
      TransitionEffectScaleTopRight,
      TransitionEffectScaleBottomLeft,
      TransitionEffectScaleBottomRight,
      TransitionEffectScaleCenter,
      TransitionEffectpixelate_TopBottom,
      TransitionEffectpixelate_BottomTop,
      TransitionEffectpixelate_LeftRight,
      TransitionEffectpixelate_RightLeft,
      TransitionEffectVisual,
      TransitionEffectRadialUnveil,
      TransitionEffectEnd,

   };


   enum EVisualEffect
   {

      VisualEffectStart,
      VisualEffectPixelExplosion,
      VisualEffectAlphaPixelExplosion,
      VisualEffectPixelExplosion2,

      VisualEffectRotateEx8,
      VisualEffectRotateEx4,
      VisualEffectRain1,
      VisualEffectRotateEx5,
      VisualEffectRotateEx6,
      VisualEffectRotateEx7,

      VisualEffectExpand8,
      VisualEffectExpand4,
      VisualEffectExpand5,
      VisualEffectExpand6,
      VisualEffectExpand7,

      VisualEffectRotateBlend,
      VisualEffectNoPrecisionRotateBlend,
      VisualEffectNoPrecisionRotateColorBlend,
      VisualEffectNoPrecisionRotateTrackColorBlend,
      VisualEffectRotateEx1,
      VisualEffectRotateEx2,
      VisualEffectRotateEx3,
      VisualEffectEnd,

   };

} // namespace backimpact






