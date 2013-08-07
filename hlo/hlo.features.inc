<?php
/**
 * @file
 * hlo.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function hlo_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function hlo_node_info() {
  $items = array(
    'sitra_hlo' => array(
      'name' => t('Hébergement locatif'),
      'base' => 'node_content',
      'description' => t('Objet touristique Sitra de type HEBERGEMENT_LOCATIF'),
      'has_title' => '1',
      'title_label' => t('Nom'),
      'help' => '',
    ),
  );
  return $items;
}
