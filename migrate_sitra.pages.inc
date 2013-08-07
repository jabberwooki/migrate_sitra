<?php 
/**
 * @file
 * @author Christophe Espiau <christophe.espiau@korora.fr>
 * @license GPL
 * @license http://opensource.org/licenses/gpl-license.php GNU Public License
 *
 */

/**
 * 
 */
function migrate_sitra_notifications() {
	$output = "";
	
	if(isset($_GET['sort']) && isset($_GET['order'])){
		// Sort it Ascending or Descending?
		if($_GET['sort'] == 'asc')
			$sort = 'ASC';
		else
			$sort = 'DESC';
	
		// Which column will be sorted
		switch($_GET['order']){
			case 'Name':
				$order = 'website_id';
				break;
			case 'Type':
				$order = 'status';
				break;
			case 'Filename':
				$order = 'date';
				break;
			case 'Module':
				$order = 'module';
				break;
			default:
				$order = 'date';
		}
	}
	else{
		// Default sort
		$sort = 'DESC';
		$order = ' date';
	}
	
	$query = db_select('migrate_sitra_notifications', 'msn')
		->fields('msn', array('snid', 'website_id', 'status', 'casual', 'reinitiated', 'export_url', 'confirm_url', 'confirmed', 'date'))
		->extend('TableSort')
		->extend('PagerDefault')
		->orderBy($order, $sort)
		->limit(5);
	$result = $query->execute();
	
	$headers = array(
		'ID',
		array('data' => t('Website'), 'field' => 'website_id'),
		array('data' => t('Status'), 'field' => 'status'),
		t('Casual'),
		t('Reinitiated'),
		t('Export file url'),
		t('Confirmation url'),
		t('Confirmed'),
		array('data' => t('Date'), 'field' => 'date'),
	);
	
	$rows = array();
	while($data = $result->fetchObject()){
		$rows[] = array(
				$data->snid,
				$data->website_id,
				$data->status,
				$data->casual,
				$data->reinitiated,
				$data->export_url,
				$data->confirm_url,
				$data->confirmed,
				format_date($data->date,'short'),
		);
	}
	
	$output = theme_table(array(
		'header' => $headers,
		'rows' => $rows,
		'attributes' => array(),
		'sticky' => true,
		'colgroups' => array(),
		'caption' => t('Sitra notifications received'),
		'empty' => t('Notifications not yet received')
		))
		.theme('pager');
	
	return $output;
}

function migrate_sitra_exported_files() {
	$output = '';
	
	if(isset($_GET['sort']) && isset($_GET['order'])){
		// Sort it Ascending or Descending?
		if($_GET['sort'] == 'asc')
			$sort = 'ASC';
		else
			$sort = 'DESC';
	
		// Which column will be sorted
		switch($_GET['order']){
			case 'Download Date':
				$order = 'download_date';
				break;
			case 'File name':
				$order = 'filename';
				break;
			case 'Imported':
				$order = 'imported';
				break;
			default:
				$order = 'date';
		}
	}
	else{
		// Default sort
		$sort = 'DESC';
		$order = ' download_date';
	}
	
	$query = db_select('migrate_sitra_exported', 'mse')
	->fields('mse', array('fid', 'filename', 'path', 'download_date', 'imported', 'import_date'))
	->extend('TableSort')
	->extend('PagerDefault')
	->orderBy($order, $sort)
	->limit(5);
	$result = $query->execute();
	
	$headers = array(
			'ID',
			t('Download Date'),
			t('File name'),
			t('Path'),
			t('Imported'),
			t('Import Date'),
	);
	
	$rows = array();
	while($data = $result->fetchObject()){
		$import_data = ($data->import_date != 0) ? format_date($data->import_date,'short') : '0';
		$rows[] = array(
				$data->fid,
				format_date($data->download_date,'short'),
				$data->filename,
				$data->path,
				$data->imported,
				format_date($data->import_date,'short'),
		);
	}
	
	$output = theme_table(array(
			'header' => $headers,
			'rows' => $rows,
			'attributes' => array(),
			'sticky' => true,
			'colgroups' => array(),
			'caption' => t('Sitra export files locally downloaded'),
			'empty' => t('No file downloaded yet')
	))
	.theme('pager');
	
	return $output;
}