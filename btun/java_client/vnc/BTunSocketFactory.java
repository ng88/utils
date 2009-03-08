/******************************************************************************
 *   btun/btund is a powerful backward tunnel creator                         *
 *                                                                            *
 *   An user guide is available on:                                           *
 *    http://svn.ngsoft-fr.com/trac/utils/wiki/BtunUserGuideEnglish           *
 *                                                                            *
 ******************************************************************************/
 
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


package vnc;
import btun.*;

import java.applet.*;
import java.net.*;
import java.io.*;

/** This implements an alternate way to connect to VNC servers via a BTun tunnel
 */ 
public class BTunSocketFactory implements SocketFactory
{

    public Socket createSocket(String host, int port, Applet applet) throws IOException
    {
	try
	{
	    return createSocket(
				applet.getParameter("BTLOGIN"),
				applet.getParameter("PASSWORD"),
				applet.getParameter("BTHOST"),
				Integer.parseInt(applet.getParameter("BTPORT")),
				applet.getParameter("BTMACHINE")
				);
	}
	catch(Exception ex)
	{
	    throw new IOException(ex);
	}
    }

    public Socket createSocket(String host, int port, String[] args) throws IOException
    {
	try
	{
	    return createSocket(
				readArg(args, "BTLOGIN"),
				readArg(args, "PASSWORD"),
				readArg(args, "BTHOST"),
				Integer.parseInt(readArg(args, "BTPORT")),
				readArg(args, "BTMACHINE")
				);
	}
	catch(Exception ex)
	{
	    throw new IOException(ex);
	}
    }


    public Socket createSocket(String login, String passwd, String host, int port, String machine) throws IOException
    {
	BTunClient c = new BTunClient(login, passwd, host);
	c.connectToChannel(c.getLogin() + "_" + machine + "_VNCCtrlChan");

	DataOutputStream btout = new DataOutputStream(c.getChannelOutputStream());
	BufferedReader btin
	    = new BufferedReader(new InputStreamReader(c.getChannelInputStream()));

	/* request a new channel */
	btout.writeBytes("GetNewChannelName VNC\n");
	String channel = btin.readLine();
	System.out.println("GetNewChannelName => "+channel);

	BTunClient newSock = c.createNewClient();
	newSock.connectToChannel(channel, BTunProtocol.OPT_AUTOCLOSE);

	btout.writeBytes("ConnectToVNC " + channel + "\n");
	if(!btin.readLine().startsWith("Ok"))
	    throw new IOException("Can't connect to remote VNC on channel " + channel + "!");

	c.disconnect();

	return newSock.getSocket();
	
    }


  private String readArg(String[] args, String name) {

    for (int i = 0; i < args.length; i += 2) {
      if (args[i].equalsIgnoreCase(name)) {
	try {
	  return args[i+1];
	} catch (Exception e) {
	  return null;
	}
      }
    }
    return null;
  }

}
