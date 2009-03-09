
package viewer;

import btun.*;
import vnc.*;
import shell.*;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;



public class Viewer extends JTabbedPane
{

    protected ConfigViewer cfg;

    public ConfigViewer getConfigViewer() { return cfg; }

    public Viewer()
    {
	addTab("Configuration", cfg = new ConfigViewer()); 

	cfg.addActionListener(new ActionListener()
	    {
		public void actionPerformed(ActionEvent e)
		{
		    try
		    {
			cfg.getPort();
			if(cfg.getLogin().equals("")) throw new Exception("Login required");
			if(cfg.getPasswd().equals("")) throw new Exception("Password required");
			if(cfg.getServer().equals("")) throw new Exception("Server required");
			if(cfg.getMachine().equals("")) throw new Exception("Machine required");

			if(cfg.isShell())
			{
			    BTunClient bt = new BTunClient(
							   cfg.getLogin(),
							   cfg.getPasswd(),
							   cfg.getServer(),
							   cfg.getPort()
							   );
			    bt.connectToChannel(cfg.getLogin() + "_" + cfg.getMachine() + "_Shell");
			    ShellPanel shell = new ShellPanel(bt);
			    addTab("Shell on " + cfg.getMachine(), shell);
			}
			else
			{
			    VncViewer v = new VncViewer();
			    v.setParameters(
					    cfg.getLogin(),
					    cfg.getPasswd(),
					    cfg.getServer(),
					    cfg.getPort(),
					    cfg.getMachine()
					    );
			    v.init();
			    addTab("VNC on " + cfg.getMachine(), v);
			    v.start();
			}
			setSelectedIndex(getTabCount() - 1);
		    }
		    catch(Exception ex)
		    {
			ex.printStackTrace();
			JOptionPane.showMessageDialog(
						      null,
						      "Unable to connect to server\n\n" + ex.getMessage(), 
						      "Error",
						      JOptionPane.ERROR_MESSAGE );
		    }
		}
	    });

    }

}

