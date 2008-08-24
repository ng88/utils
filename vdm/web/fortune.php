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

$vdmserv = new VDMServer();

$fp = fopen('cache-lock', 'w+') or die('Impossible d\'ouvrir cache-lock');
flock($fp, LOCK_EX) or die('Impossible de verrouiller le fichier !');

$ftmp = fopen('cache-last-tmp', 'w') or die('Impossible d\'ouvrir cache-last-tmp');

$last_id = trim(file_get_contents('cache-last-id'));

$sep = "\n%\n";
$page = 0;
$done = false;
do
{
    $list = $list->getLast($page);
    for($i = 0; $i < $list->count(); $i++)
    {
        if($list->getVDMAt($i)->getID() == $last_id)
        {
            $done = true;
            break;
        }
        fwrite($ftmp, $list->getVDMAt($i)->toPlainText(false).$sep);
    }
    $page++;
}
while(!$done && $list->count() > 0)



flock($fp, LOCK_UN);
fclose($fp);


echo file_get_contents('cache-last-txt');


?>
