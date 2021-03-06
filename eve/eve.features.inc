<?php
/**
 * @file
 * eve.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function eve_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function eve_node_info() {
  $items = array(
    'sitra_eve' => array(
      'name' => t('Evénement'),
      'base' => 'node_content',
      'description' => t('Objet touristique Sitra de type FETE_ET_MANIFESTATION'),
      'has_title' => '1',
      'title_label' => t('Nom'),
      'help' => '',
    ),
  );
  return $items;
}
