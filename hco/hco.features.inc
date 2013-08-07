<?php
/**
 * @file
 * hco.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function hco_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function hco_node_info() {
  $items = array(
    'sitra_hco' => array(
      'name' => t('Hébergement collectif'),
      'base' => 'node_content',
      'description' => t('Objet touristique Sitra de type HEBERGEMENT_COLLECTIF'),
      'has_title' => '1',
      'title_label' => t('Nom'),
      'help' => '',
    ),
  );
  return $items;
}
