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

require_once('VDM.php');

class VDmXml
{
   var $parser;
   var $list;
   var $vdm;
   var $champs;
 
   function VDMXml()
   {
      $this->parser = xml_parser_create();
      xml_set_object($this->parser, $this);
      xml_set_element_handler($this->parser, "tag_open", "tag_close");
      xml_set_character_data_handler($this->parser, "cdata");

   }
 
   function parse($data)
   {
        $this->list = new VDMList();
        $this->vdm = NULL;
	xml_parse($this->parser, $data);
   }

  function parseFromURL($url)
  {
      $this->parse(file_get_contents($url));
  }

   function tag_open($parser, $tag, $attributes)
   {
        switch($tag)
        {
          case 'VDM':
            $this->vdm = new VDM();
            $this->vdm->setID($attributes['ID']);
            break;
          case 'AUTEUR':
          case 'CATEGORIE':
          case 'DATE':
          case 'JE_VALIDE':
          case 'BIEN_MERITE':
          case 'COMMENTAIRES':
          case 'TEXTE':
            $this->$champs = strtolower($tag);
            $this->setValue($this->$champs, '');
            break;
        }
   }

   function setValue($c, $v)
   {
      $this->vdm->$c = $v;
   }

   function appendValue($c, $v)
   {
      $this->vdm->$c .= $v;
   }
 
   function cdata($parser, $cdata)
   {
	$this->appendValue($this->$champs, $cdata);
   }
 
   function tag_close($parser, $tag)
   {
        if($tag == 'VDM')
        {
          $this->list->addVDM($this->vdm);
        }
   }

    function &getResult() { return $this->list; }
 
} 
 

?>
