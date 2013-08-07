<?php
/**
 * @file
 * cam.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function cam_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function cam_node_info() {
  $items = array(
    'sitra_cam' => array(
      'name' => t('Camping'),
      'base' => 'node_content',
      'description' => t('Objet touristique Sitra de type HOTELLERIE_PLEIN_AIR'),
      'has_title' => '1',
      'title_label' => t('Nom'),
      'help' => '',
    ),
  );
  return $items;
}
