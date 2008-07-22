<?php

/***************************************************************************
 *   This file is part of the 'utils' projects                             *
 *                                                                         *
 *   'utils' projects                                                      *
 *                                                                         *
 *   Copyright (C) 2006, 2008 by GUILLAUME Nicolas                         *
 *   ng@ngsoft-fr.com                                                      *
 *                                                                         *
 *   http://svn.ngsoft-fr.com/trac/utils/                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2 of the License only.          *
 *   See the COPYING file.                                                 *
 ***************************************************************************/ 



class VDM
{

  var $texte;
  var $auteur;
  var $date;
  var $je_valide;
  var $bien_merite;
  var $nb_comm;
  var $texte;


  function setAuteur($v) { $this->auteur = $v; }
  function getAuteur() { return $this->auteur; }

  function setTexte($v) { $this->texte = $v; }
  function getTexte() { return $this->texte; }

  function setDate($v) { $this->date = $v; }
  function getDate() { return $this->date; }

  function setJeValide($v) { $this->je_valide = $v; }
  function getJeValide() { return $this->je_valide; }

  function setBienMerite($v) { $this->bien_merite = $v; }
  function getBienMerite() { return $this->bien_merite; }

  function setCommCount($v) { $this->nb_comm = $v; }
  function getCommCount() { return $this->nb_comm; }

  function toPlainText()
  {
      return $this->getTexte() . "\n" .
           '=== Le ' . $this->getDate() . ' par ' . $this->getAuteur() . " ===\n" .
           '=== Je valide ' . $this->getJeValide() . ' / Bien mérité ' . $this->getBienMerite() .
           ' / Commentaires ' . $this->getCommCount() . ' ===';
  }


}


class VDMList
{
  var $data;

  function VDMList()
  {
      $this->data = array();
  }


  function getVDMAt($i)
  {
      return $this->data[$i];
  }

  function count()
  {
    return count($this->data);
  }

  function toPlainText()
  {
    return implode("\n\n", $this->data);
  }

}



?>