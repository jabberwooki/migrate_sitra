<?php
/**
 * @file
 * hot.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function hot_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function hot_node_info() {
  $items = array(
    'sitra_hot' => array(
      'name' => t('Hôtel'),
      'base' => 'node_content',
      'description' => t('Objet touristique Sitra de type HOTELLERIE.'),
      'has_title' => '1',
      'title_label' => t('Nom'),
      'help' => '',
    ),
  );
  return $items;
}
