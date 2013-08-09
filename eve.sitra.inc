<?php
/**
 * @file 
 * @author Christophe Espiau
 *
 */

class EveListJSON extends MigrateListJSON {
	/**
	 * The default implementation assumes the IDs are top-level array elements,
	 * but the array elements are the export items - we need to look inside them
	 * for the hotel objects IDs.
	 */
	protected function getIDsFromJSON(array $data) {
		$ids = array();
		foreach ($data as $item) {
			if ($item['objetsTouristiques'][0]['type'] == 'FETE_ET_MANIFESTATION') {
				foreach ($item['objetsTouristiques'] as $objet) {
					$ids[] = $objet['id'];
				}
			}
		}
		return $ids;
	}
	
	/**
	 * Return a count of all available IDs from the source listing. The default
	 * implementation assumes the count of top-level elements reflects the number
	 * of IDs available - In Sitra case, we need to reach the second level.
	 */
	public function computeCount() {
		$count = 0;
		if (empty($this->httpOptions)) {
			$json = file_get_contents($this->listUrl);
		}
		else {
			$response = drupal_http_request($this->listUrl, $this->httpOptions);
			$json = $response->data;
		}
		if ($json) {
			$data = drupal_json_decode($json);
			if ($data) {
				foreach ($data as $item) {
					if ($item['objetsTouristiques'][0]['type'] == 'FETE_ET_MANIFESTATION') {
						$count = count($item['objetsTouristiques']);
					}
				}				
			}
		}
		return $count;
	}
}

class EveMigration extends BasicSitraMigration {
	
	protected $types_manifestation = array();
	
	protected $categories = array();
	
	protected $themes = array();
	
	public function __construct() {
		parent::__construct();

		$this->description = t('Sitra evenement objects (EVE)');

		// Source.
		$list = variable_get('file_public_path', conf_path() . '/files') . '/sitra-imports/selections.json';
		$item = variable_get('file_public_path', conf_path() . '/files') . '/sitra-imports/objets_modifies/objets_modifies-:id.json';
		$fields = array(
			'id' => 'Sitra2 object id',
			'identifierSitra1' => 'Sitra1 object id',
			'nom_Fr' => 'Name',
			'descriptifCourt_Fr' => 'Short description',
			'descriptifDetaille_Fr' => 'Detailled description',
			'geo_type' => 'Geometry type of the geofield',
			'adresse1' => 'Postal address - first field',
			'adresse2' => 'Postal address - Second field',
			'adresse2' => 'Postal address - Third field',
			'codepostal' => 'Postal address - Postal code field',
			'commune' => 'Postal address - City',
			'telephone' => 'Phone number(s)',
			'fax' => 'Fax number',
			'email' => 'Contact email',
			'siteweb' => 'Web site url',
			'ouverture' => 'Opening period',
 			'tarifs' => 'Prices',
// 				'modes_paiement' => 'Array of Payment means',
			'image_url' => 'External url of the main picture',
			'images_url' => 'External urls of additional pictures',
			'type_Fr' => 'Event type',
			'categories_Fr' => 'Event categories',
			'themes_Fr' => 'Event themes',
			'portee_Fr' => 'Event coverage',		
		);
		$this->source = new MigrateSourceList(new EveListJSON($list), new MigrateItemJSON($item, array()), $fields);

		// Destination.
		$this->destination = new MigrateDestinationNode('sitra_eve');

		// Map.
		$this->map = new MigrateSQLMap(
				$this->machineName,
				array(
						'sid' => array(
								'type' => 'int',
								'unsigned' => TRUE,
								'not null' => TRUE,
								'description' => 'Sitra object id.',
						)
				),
				MigrateDestinationNode::getKeySchema()
		);

		// Fields mapping.
		$this->addFieldMapping('title', 'nom_Fr');
		$this->addFieldMapping('field_sitra2_id', 'id');
		$this->addFieldMapping('field_sitra1_id', 'identifierSitra1');
		$this->addFieldMapping('field_sitra_descriptif_court', 'descriptifCourt_Fr');
		$this->addFieldMapping('field_sitra_descriptif_detaille', 'descriptifDetaille_Fr');
		$geo_arguments = array(
				'lat' => array('source_field' => 'lat'),
				'lon' => array('source_field' => 'lon'),
		);
		$this->addFieldMapping('field_sitra_geolocalisation', 'geo_type')->arguments($geo_arguments);
		$this->addFieldMapping('field_sitra_adresse_1', 'adresse1');
		$this->addFieldMapping('field_sitra_adresse_2', 'adresse2');
		$this->addFieldMapping('field_sitra_adresse_3', 'adresse3');
		$this->addFieldMapping('field_sitra_code_postal', 'codepostal');
		$this->addFieldMapping('field_sitra_commune', 'commune');

		$this->addFieldMapping('field_sitra_telephone', 'telephone');
		$this->addFieldMapping('field_sitra_fax', 'fax');
		$this->addFieldMapping('field_sitra_email', 'email');
		$this->addFieldMapping('field_sitra_site_web', 'siteweb');
		
		$this->addFieldMapping('field_sitra_ouverture', 'ouverture');

		$this->addFieldMapping('field_sitra_tarifs', 'tarifs');
// 		$this->addFieldMapping('field_sitra_modes_paiement', 'modes_paiement');

		$this->addFieldMapping('field_sitra_image_principale', 'image_url');
		$this->addFieldMapping('field_sitra_image_principale:file_replace')->defaultValue(FILE_EXISTS_REPLACE);
		$this->addFieldMapping('field_sitra_image_principale:alt', 'image_legende');
		$this->addFieldMapping('field_sitra_image_principale:title', 'image_legende');

		$this->addFieldMapping('field_sitra_images', 'images_url');
		$this->addFieldMapping('field_sitra_images:file_replace')->defaultValue(FILE_EXISTS_REPLACE);
		$this->addFieldMapping('field_sitra_images:alt', 'images_legende');
		$this->addFieldMapping('field_sitra_images:title', 'images_legende');
		
		$this->addFieldMapping('field_sitra_type_manifestation', 'types_Fr');
		$this->addFieldMapping('field_sitra_eve_categories', 'categories_Fr');
		$this->addFieldMapping('field_sitra_themes', 'themes_Fr');
		$this->addFieldMapping('field_sitra_portee', 'portee_Fr');
	}

	/**
	 * Data handling before node saving.
	 * Most data are not at the top-level in the $row object.
	 * We must bring them up at the top to make them accessible to migration.
	 * Original values are removed from $row to avoid data duplication
	 * which would increase unnecessarily the $row object size.
	 */
	function prepareRow($row) {
		parent::prepareRow($row);

		// ouverture
		$row->ouverture = $row->ouverture->periodeEnClair->libelleFr;

		// informations feteetmanifestation - types
		$types = array();
		if (isset($row->informationsFeteEtManifestation->typesManifestation)) {
			foreach ($row->informationsFeteEtManifestation->typesManifestation as $type) {
				$types[] = $this->getTypeManifestation($type->id);
			}
		}
		$row->types_Fr = $types;
		
		// informations feteetmanifestation - Categories
		$categories = array();
		if (isset($row->informationsFeteEtManifestation->categories)) {
			foreach ($row->informationsFeteEtManifestation->categories as $categorie) {
				$categories[] = $this->getCategorie($categorie->id);
			}
		}
		$row->categories_Fr = $categories;
		
		// informations feteetmanifestation - portee
		$portee_id = $row->informationsFeteEtManifestation->portee->id;
		dsm('portee_id = ' . $portee_id);
		$row->portee_Fr = $this->getElementReferenceLibelle($portee_id);
		dsm($row->portee_Fr);
		
		// informations feteetmanifestation - themes
		$themes = array();
		if (isset($row->informationsFeteEtManifestation->themes)) {
			foreach ($row->informationsFeteEtManifestation->themes as $theme) {
				$themes[] = $this->getTheme($theme->id);
			}
		}
		$row->themes_Fr = $themes;
		
		unset($row->informationsFeteEtManifestation);
		
	}
	
	function getTypeManifestation($type_id) {
		if (!isset($this->types_manifestation[$type_id])) {
			$type = $this->getElementReferenceLibelle($type_id);
			$this->types_manifestation[$type_id] = $type;
			return $type;
		}
		return $this->types_manifestation[$type_id];
	}
	
	function getCategorie($categorie_id) {
		if (!isset($this->categories[$categorie_id])) {
			$categorie = $this->getElementReferenceLibelle($categorie_id);
			$this->categories[$categorie_id] = $categorie;
			return $categorie;
		}
		return $this->categories[$categorie_id];
	}
	
	function getTheme($theme_id) {
		if (!isset($this->themes[$theme_id])) {
			$theme = $this->getElementReferenceLibelle($theme_id);
			$this->themes[$theme_id] = $theme;
			return $theme;
		}
		return $this->themes[$theme_id];
	}
}