<?php
/**
 * @file
 * res.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function res_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function res_node_info() {
  $items = array(
    'sitra_res' => array(
      'name' => t('Restaurant'),
      'base' => 'node_content',
      'description' => t('Objet touristique Sitra de type RESTAURATION'),
      'has_title' => '1',
      'title_label' => t('Nom'),
      'help' => '',
    ),
  );
  return $items;
}
