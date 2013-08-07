<?php
/**
 * @file
 * sitra_base.features.inc
 */

/**
 * Implements hook_image_default_styles().
 */
function sitra_base_image_default_styles() {
  $styles = array();

  // Exported image style: sitra-fiche.
  $styles['sitra-fiche'] = array(
    'name' => 'sitra-fiche',
    'effects' => array(
      2 => array(
        'label' => 'Échelle',
        'help' => 'La mise à l\'échelle maintiendra les proportions originales de l\'image. Si une seule dimension est précisée, l\'autre dimension sera calculée automatiquement.',
        'effect callback' => 'image_scale_effect',
        'dimensions callback' => 'image_scale_dimensions',
        'form callback' => 'image_scale_form',
        'summary theme' => 'image_scale_summary',
        'module' => 'image',
        'name' => 'image_scale',
        'data' => array(
          'width' => 360,
          'height' => 240,
          'upscale' => 0,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: sitra-liste.
  $styles['sitra-liste'] = array(
    'name' => 'sitra-liste',
    'effects' => array(
      3 => array(
        'label' => 'Mise à l’échelle et recadrage',
        'help' => 'La mise à l\'échelle et le recadrage maintiendront les proportions originales de l\'image puis recadreront la dimension la plus large. C\'est très utile pour créer des vignettes carrées sans étirer les images.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 180,
          'height' => 120,
        ),
        'weight' => 1,
      ),
    ),
  );

  return $styles;
}
