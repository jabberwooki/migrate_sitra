<?php
/**
 * @file
 * cam.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function cam_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sitra_adresse|node|sitra_cam|default';
  $field_group->group_name = 'group_sitra_adresse';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'sitra_cam';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Adresse',
    'weight' => '13',
    'children' => array(
      0 => 'field_sitra_adresse_1',
      1 => 'field_sitra_adresse_2',
      2 => 'field_sitra_code_postal',
      3 => 'field_sitra_commune',
      4 => 'field_sitra_adresse_3',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_sitra_adresse|node|sitra_cam|default'] = $field_group;

  return $export;
}
