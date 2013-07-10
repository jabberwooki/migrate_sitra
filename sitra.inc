<?php
/**
 * @file 
 * @author Christophe Espiau
 *
 */

abstract class BasicSitraMigration extends Migration {
	
	// Path to the Sitra export files.
	protected $export_path = '';
	
	// An array of all Sitra elements referenced to by the exported objects. 
	protected $elements_reference = array();
	
	// An array of all communes referenced to by the exported objects.
	protected $communes = array();
	
	protected $modes_paiement = array();
	
	protected $classements = array();
	
	public function __construct() {
		// Always call the parent constructor first for basic setup
		parent::__construct();
		
		// Build path to exported json data.
		$this->export_path = variable_get('file_public_path', conf_path() . '/files') . '/sitra-exports/';
		
		// Reference elements data are cached at $this->elements_reference.
		if (empty($this->elements_reference)) {
			$json = file_get_contents($this->export_path . 'elements_reference.json');
			$this->elements_reference = drupal_json_decode($json);
		}
		
		// Communes data are cached at $this->communes;
		if (empty($this->communes)) {
			$json = file_get_contents($this->export_path . 'communes.json');
			$communes_data = drupal_json_decode($json);
			foreach ($communes_data as $data) {
				$this->communes[$data['id']] = $data['nom'];
			}
		}
	}
	
	public function getElementReferenceLibelle($id, $language='Fr') {
		foreach ($this->elements_reference as $element) {
			if ($element['id'] == $id) {
				return $element['libelle' . $language];
			}
		}
	}
	
	public function getCommune($commune_id) {
		return $this->communes[$commune_id];
	}
	
	public function getModePaiement($mode_id) {
		if (!isset($this->modes_paiement[$mode_id])) {
			$mode = $this->getElementReferenceLibelle($mode_id);
			$this->modes_paiement[$mode_id] = $mode;
			return $mode;
		}
		return $this->modes_paiement[$mode_id];
	}
	
	function prepareRow($row) {
		// nom
		$row->nom_Fr = $row->nom->libelleFr;
		
		// moyens de communication
		$telephones = array();
		$row->fax = '';
		$row->email = '';
		$row->siteweb = '';
		if (isset($row->informations->moyensCommunication)) {
			$moyens_comm = $row->informations->moyensCommunication;
		}
		if (isset($row->informations->moyensCommunicationExternes)) {
			$moyens_comm = $row->informations->moyensCommunicationExternes;
		}
		if (isset($moyens_comm)) {
			foreach ($moyens_comm as $moyen) {
				switch ($moyen->type->id) {
					case "201":
						$telephones[] = $moyen->coordonnee;
						break;
					case "202":
						$row->fax = $moyen->coordonnee;
						break;
					case "204":
						$row->email = $moyen->coordonnee;
						break;
					case "205":
						$row->siteweb = $moyen->coordonnee;
						break;
				}
			}
		}
		$row->telephone = $telephones;
		unset($row->informations);
		
		// description
		$row->descriptifCourt_Fr = $row->presentation->descriptifCourt->libelleFr;
		$row->descriptifDetaille_Fr = $row->presentation->descriptifDetaille->libelleFr;
		unset($row->presentation);
		
		// adresse
		$row->adresse1 = $row->localisation->adresse->adresse1;
		$row->adresse2 = $row->localisation->adresse->adresse2;
		$row->codepostal = $row->localisation->adresse->codePostal;
		$row->commune = $this->getCommune($row->localisation->adresse->commune->id);
		unset($row->localisation->adresse);
		
		// geolocalisation
		$row->geo_type = 'point';
		$coordinates = $row->localisation->geolocalisation->geoJson->coordinates;
		$row->lat = $coordinates[1];
		$row->lon = $coordinates[0];
		unset($row->localisation->geolocalisation);
		
		// tarifs
		if (isset($row->descriptionTarif->tarifsEnClair)) {
			$row->tarifs = $row->descriptionTarif->tarifsEnClair->libelleFr;
		}
		// modes paiement
		if (isset($row->descriptionTarif->modesPaiement)) {
			foreach ($row->descriptionTarif->modesPaiement as $mode) {
				$row->modes_paiement[] = $this->getModePaiement($mode->id);
			}
		}
		unset($row->descriptionTarif);
		
		// image principale
		$row->image_url = '';
		$row->image_legende = '';
		//$row->image_title = '';
		if (isset($row->imagePrincipale)) {
			$row->image_url = $row->imagePrincipale->traductionFichiers[0]->urlDiaporama;
			if (isset($row->imagePrincipale->legende)) {
				$row->image_legende = $row->imagePrincipale->legende->libelleFr;
			}
			unset($row->imagePrincipale);
		}
		
		// images
		if (isset($row->images)) {
			foreach ($row->images as $image) {
				$row->images_url[] = $image->traductionFichiers[0]->urlDiaporama;
				if (isset($image->legende)) {
					$row->images_legende[] = $image->legende->libelleFr;
				}
			}
			unset($row->images);
		}
	}
	
	public function prepare(stdClass $entity, stdClass $row) {
		$entity->language = 'fr';
	}
}
