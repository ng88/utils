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

$fp = fopen('cache/cache-lock', 'w+') or die('Impossible d\'ouvrir cache-lock');
flock($fp, LOCK_EX) or die('Impossible de verrouiller le fichier !');

$ftmp = fopen('cache/cache-last-tmp', 'w') or die('Impossible d\'ouvrir cache-last-tmp');

$last_id = trim(file_get_contents('cache/cache-last-id'));

$sep = "\n%\n";
$page = 0;
$done = false;
$nlast = -1;
do
{
    $list = $vdmserv->getLast($page);
    for($i = 0; $i < $list->count(); $i++)
    {
        $vdm = $list->getVDMAt($i);

        if($nlast == -1)
            $nlast = $vdm->getID();

        echo ($vdm->getID() == $last_id).'-'.($vdm->getID()).'-'.($last_id)."\n";
        if($vdm->getID() == $last_id)
        {
            $done = true;
            break;
        }
        //fwrite($ftmp, $vdm->toPlainText(false).$sep);
    }
    $page++;
}
while($page < 5 && !$done && $list->count() > 0);

fwrite($ftmp, file_get_contents('cache/cache-last-txt'));

fclose($ftmp);

rename('cache/cache-last-tmp', 'cache/cache-last-txt');

$fid = fopen('cache/cache-last-id', 'w') or die('Erreur last id');
fwrite($fid, $nlast."\n");
fclose($fid);

flock($fp, LOCK_UN);
fclose($fp);


echo file_get_contents('cache/cache-last-txt');


?>
