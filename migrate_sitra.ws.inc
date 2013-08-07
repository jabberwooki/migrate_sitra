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
function migrate_sitra_execute_ws_call() {		
	// Handle notification data.
	$website_id = $_POST['siteWebId'];
	$status = (strtoupper($_POST['statut']) == 'SUCCESS') ? '1' : '0';
	$casual = (strtoupper($_POST['ponctuel']) == 'true') ? '1' : '0';
	$reinitiated = (strtoupper($_POST['reinitialisation']) == 'true') ? '1' : '0';

	$fields = array(
		'website_id' => $website_id,
		'status' => $status,
		'casual' => $casual,
		'reinitiated' => $reinitiated,
		'date' => time()
	);
	
	// If export succeeded, handle url data
	if ($status) {
		$export_url = $_POST['urlRecuperation'];
		$confirm_url = $_POST['urlConfirmation'];
		
		$url_fields = array(
			'export_url' => $export_url,
			'confirm_url' => $confirm_url,
		);
		$fields = array_merge($fields, $url_fields);
	}
	
	// Send confirmation of reception.
	$data = array(
			'siteWebId' => $website_id,
			'date' => time(),
			'file' => basename($_POST['urlRecuperation'])
	);
	
	$options = array(
			'method' => 'POST',
			'data' => drupal_http_build_query($data),
			'timeout' => 15,
			'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
	);
	
	$result = drupal_http_request($_POST['urlConfirmation'], $options);
	
	// Store notification data into database.
	$inserted_row_id = db_insert('migrate_sitra_notifications')
		->fields($fields)
		->execute();
	
	$export_status = ($status == 1)? t('export succeeded.') : t('export failed');
	watchdog('sitra notifications', t('Notification %snid recorded for site %value : %export_status', array('%snid' => $inserted_row_id, '%value' => $website_id, '%result' => $export_status)));

	// If confirmation request succeeded, table migrate_sitra_notification is updated.
	if ($result->code == '200') {
		db_update('migrate_sitra_notifications')
			->fields(array('confirmed' => 1))
			->condition('snid', $inserted_row_id)
			->execute();
			
			watchdog('sitra notifications', t('Notification %snid confirmation sent successfully.', array('%snid' => $inserted_row_id)));
	}
	
	// Il export succeeded, download export file locally.
	if ($status) {
		$destination = 'public://sitra-exports';
		if ($location = system_retrieve_file($export_url, $destination)) {
			$filename = basename($_POST['urlRecuperation']);
			$fields = array(
				'filename' => basename($_POST['urlRecuperation']),
				'path' => $location,
				'download_date' => time(),
			);
			
			// Store downloaded file details into database.
			db_insert('migrate_sitra_exported')
			->fields($fields)
			->execute();
			watchdog('sitra notifications', t('Export file %file downloaded at %destination', array('%file' => $filename, '%destination' => $destination)));
		}
	}
}