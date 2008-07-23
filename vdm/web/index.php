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

require_once('VDMServer.php');

$action = isset($_GET['act']) ? strtolower($_GET['act']) : 'random';
$page = isset($_GET['pg']) ? (0+$_GET['pg']) : 0;

$vdmserv = new VDMServer();

$list = NULL;

switch($action)
{
  case 'rnd':
  case 'rand':
  case 'random':
      $list = $vdmserv->getRandom();
      break;
  case 'top':
      $list = $vdmserv->getTop($page);
      break;
  case 'flop':
      $list = $vdmserv->getFlop($page);
      break;
  case 'last':
      $list = $vdmserv->getLast($page);
      break;
}

if($list != NULL)
  echo $list->toPlainText();

?>
