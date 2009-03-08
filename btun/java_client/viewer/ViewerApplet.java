
package viewer;

import btun.*;
import vnc.*;
import shell.*;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;



public class ViewerApplet extends JApplet
{
    public ViewerApplet()
    {
	try
	{
	    System.setSecurityManager(null);

	    Viewer p = new Viewer();
	    
	    getContentPane().add(p);
	    setVisible(true);

	}
	catch(Exception ex)
	{
	    ex.printStackTrace();
	}
    }
}
