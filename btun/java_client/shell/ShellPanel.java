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

package shell;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;

import java.io.*;
import javax.swing.text.*;
import java.util.*;

import btun.*;


public class ShellPanel extends JPanel
{

    protected BTunClient btclient;

    protected Console console;

    public ShellPanel(BTunClient bt)
    {

       setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
        
	JScrollPane sp =  new JScrollPane(
		    console = new Console(),
                    JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
                    JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);

        add(sp);


	btclient = bt;

	final DataOutputStream btout = new DataOutputStream(bt.getChannelOutputStream());
	final BufferedReader btin
	    = new BufferedReader(new InputStreamReader(bt.getChannelInputStream()));
	//final DataInputStream btin = new DataInputStream(bt.getChannelInputStream());

	ActionListener al = new ActionListener()
	    {
		public void actionPerformed(ActionEvent e)
		{
		    try
		    {
			/*String str = input.getText() + "\n";
			btout.writeChars(str);
			console.append(str);
			input.setText("");*/
		    }
		    catch(Exception ex)
		    {
			ex.printStackTrace();
		    }
		}
	    };


	Thread inth = new Thread()
	    {
		
		public void run()
		{
		    try
		    {
			while(true)
			{
			    console.append(btin.readLine());
			    //while(btin.ready())
				//console.append(""+(char)btin.read());
			}
		    }
		    catch(Exception ex)
		    {
			ex.printStackTrace();
		    }
		    
		}
	    };
	inth.start();

    }


}


