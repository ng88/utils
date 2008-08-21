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

require_once('xml.php');

// Demande de clef gratuite : http://www.viedemerde.fr/api/developpeurs
define('SERVER_KEY', 'VOTRE_CLEF_DEVELOPPEUR_ICI');
define('SERVER_URL', 'http://api.viedemerde.fr/1.2/');
define('SERVER_ACTION_VIEW', 'view/');

class VDMServer
{

  function VDMServer()
  {
  }


  function &getRandom()
  {
    return $this->getVDM('random');
  }

  function &getTop($page = NULL)
  {
    return $this->getVDM('top', $page);
  }


  function &getFlop($page = NULL)
  {
    return $this->getVDM('flop', $page);
  }

  function &getLast($page = NULL)
  {
    return $this->getVDM('last', $page);
  }


  function &getVDM($type, $page = NULL)
  {
    $parser = new VDmXml();

    $page = (($page == NULL) ? '' : '/' . $page);

    $parser->parseFromURL(SERVER_URL . SERVER_ACTION_VIEW . $type . $page . '?key=' . SERVER_KEY);

    return $parser->getResult();
  }

}

?>