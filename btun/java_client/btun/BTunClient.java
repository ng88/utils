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

package btun;

import crypto.MD5;
import java.net.Socket;
import java.io.*;
import java.nio.charset.Charset;

/** A BTun client
 */
public class BTunClient
{

    protected BTunService service;

    protected String login;
    protected String passwd;
    protected String host;
    protected int port;
    protected String chan;

    protected Socket socket;

    public BTunClient(String login, String passwd, String host)
    {
	this(login, passwd, host,  BTunProtocol.DEFAULT_PORT);
    }

    public BTunClient(String login, String passwd, String host, int port)
    {
	if(login.length() > BTunProtocol.USER_MAX_LOGIN_SIZE)
	    this.login = login.substring(0, BTunProtocol.USER_MAX_LOGIN_SIZE - 1);
	else
	    this.login = login;

	this.passwd = MD5.encode(passwd);
	this.port = port;
	this.host = host;
	socket = null;
    }

    public BTunClient(BTunClient o)
    {
	this.login = o.login;
	this.passwd = o.passwd;
	this.port = o.port;
	this.host = o.host;
	socket = null;
	chan = null;
    }


    public void connectToChannel(String channel) throws IOException
    {
	connectToChannel(channel, (byte)0);
    }

    public void connectToChannel(String channel, byte options) throws IOException
    {
	chan = channel;
	if(channel.length() > BTunProtocol.USER_MAX_CHANNEL_SIZE - 1)
	    channel = channel.substring(0, BTunProtocol.USER_MAX_CHANNEL_SIZE - 2);

	connect();

	OutputStream outToServer = socket.getOutputStream();
	InputStream inFromServer = socket.getInputStream();

	byte[] data = channel.getBytes(BTunProtocol.CHARSET);
	byte[] ret = new byte[data.length + 2];

	System.arraycopy(data, 0, ret, 0, data.length);

	ret[data.length] = 0;
	ret[data.length + 1] = options;

	outToServer.write(ret);

	byte agreement = (byte)inFromServer.read();
	if(agreement != BTunProtocol.CA_GRANTED)
	    throw new IOException(BTunProtocol.caErrorToString(agreement));
    }



    protected void connect() throws IOException
    {
	socket = new Socket(getHost(), getPort());

	//BufferedReader inFromUser =
	//  new BufferedReader(new InputStreamReader(System.in));


	//outToServer = new DataOutputStream(socket.getOutputStream());
	//inFromServer = new DataInputStream(socket.getInputStream());

	OutputStream outToServer = socket.getOutputStream();
	InputStream inFromServer = socket.getInputStream();

	outToServer.write(BTunProtocol.convertString(getLogin()));

	byte[] challenge = new byte[BTunProtocol.CHALLENGE_SIZE];
	inFromServer.read(challenge);

	byte[] ans = BTunProtocol.answerChallenge(challenge, getPasswd());
	outToServer.write(ans);

	byte agreement = (byte)inFromServer.read();
	if(agreement != BTunProtocol.CA_GRANTED)
	    throw new IOException(BTunProtocol.caErrorToString(agreement));

    }

    public void disconnect()
    {
	try
	{
	    if(socket != null)
		socket.close();

	    chan = null;
	}
	catch(Exception ex)
	{
	}
    }

    /** Create a new BTunClient from this one
     */
    public BTunClient createNewClient()
    {
	return new BTunClient(this);
    }


    public String getLogin() { return login; }
    protected String getPasswd() { return passwd; }
    public String getHost() { return host; }
    public int getPort() { return port; }
    public BTunService getService() { return service; }
    public String getChannel() { return chan; }

    public OutputStream getChannelOutputStream()
    {
	try
	{
	    return chan == null || socket == null ? null : socket.getOutputStream();
	}
	catch(Exception ex)
	{
	    return null;
	}
    }

    public InputStream getChannelInputStream()
    {
	try
	{
	    return chan == null || socket == null ? null : socket.getInputStream();
	}
	catch(Exception ex)
	{
	    return null;
	}
    }

    public Socket getSocket()
    {
	return socket;
    }

    public String toString()
    {
	return "BTunClient[" + getLogin() + "@" + getHost() + ":" + getPort() + " on " + getChannel()  + ", " + getService() + "]";
    }


}

