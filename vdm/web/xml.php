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


class VDmXml
{
   var $parser;
 
   function VDMXml()
   {
      $this->parser = xml_parser_create();
      xml_set_object($this->parser, $this);
      xml_set_element_handler($this->parser, "tag_open", "tag_close");
      xml_set_character_data_handler($this->parser, "cdata");

   }
 
   function parse($data)
   {
	xml_parse($this->parser, $data);
   }

  function parseFromURL($url)
  {
      $this->parse(file_get_contents($url));
  }
 
   function tag_open($parser, $tag, $attributes) {
		echo "[TAG_OPEN] ". $tag;
		if (!empty($attributes)) {
			echo " => ";
			foreach ($attributes as $key => $val) {
				echo "$key=$val, "; 
			}
		}
		echo "<br />";
   }
 
   function cdata($parser, $cdata) {
		echo "[CDATA] $cdata<br />";
   }
 
   function tag_close($parser, $tag) {
		echo "[TAG_CLOSE] $tag<br />";
   }
 
} 
 

?>
