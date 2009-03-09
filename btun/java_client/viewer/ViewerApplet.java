
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
	    String str;

	    if( (str = getParameter("DisableSM")) != null )
		if(str.equals("1")
		   System.setSecurityManager(null);

	    Viewer p = new Viewer();

	    if( (str = getParameter("BTunServer")) != null )
		p.getConfigViewer().setServer(str);

	    if( (str = getParameter("BTunPort")) != null )
		p.getConfigViewer().setPort(Integer.parseInt(str));

	    if( (str = getParameter("BTunLogin")) != null )
		p.getConfigViewer().setLogin(str);

	    if( (str = getParameter("BTunMachine")) != null )
		p.getConfigViewer().setMachine(str);
	    
	    getContentPane().add(p);
	    setVisible(true);

	}
	catch(Exception ex)
	{
	    ex.printStackTrace();
	}
    }
}
