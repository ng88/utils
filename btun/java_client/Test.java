

import btun.*;
import shell.*;

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
	   
	    BTunClient b = new BTunClient("test", "test2", "localhost");

	    b.connectToChannel("MaChannel");

	    JFrame j = new JFrame("Shell");
	    ShellPanel p = new ShellPanel(b);
	    
	    j.getContentPane().add(p);
	    j.pack();
	    j.setVisible(true);

	    System.out.println(b);


	    //b.disconnect();

	}
	catch(Exception ex)
	{
	    ex.printStackTrace();
	}
    }

}
