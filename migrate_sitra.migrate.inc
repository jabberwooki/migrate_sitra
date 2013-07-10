<?php
function migrate_sitra_migrate_api() {
  $api = array(
    'api' => 2,
    'groups' => array(
      'sitra' => array(
        'title' => t('Sitra migrations'),
        'default_format' => 'filtered_html',
      ),
    ),
  	'migrations' => array(
  		'cam' => array(
  			'class_name' => 'CamMigration',
  			'group_name' => 'sitra',
  		),
      'eve' => array(
        'class_name' => 'EveMigration',
        'group_name' => 'sitra',
      ),
  		'hot' => array(
  			'class_name' => 'HotMigration',
  			'group_name' => 'sitra',
  		),
      'res' => array(
        'class_name' => 'ResMigration',
        'group_name' => 'sitra',
      ),
    ),
  );
  return $api;
}
