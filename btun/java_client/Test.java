

import btun.*;
import shell.*;
import viewer.*;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;


public class Test
{

    public static void main(String[] a)
    {
	try
	{
	    JFrame j = new JFrame("MainWindow");
	    Viewer p = new Viewer();
	    
	    j.getContentPane().add(p);
	    j.pack();
	    j.setVisible(true);

	}
	catch(Exception ex)
	{
	    ex.printStackTrace();
	}
    }

}
