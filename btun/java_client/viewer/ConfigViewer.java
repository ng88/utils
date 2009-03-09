
package viewer;

import btun.*;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
/*
 * Created by JFormDesigner on Sun Mar 08 18:25:44 CET 2009
 */



/**
 * @author Nicolas GUILLAUME
 */
public class ConfigViewer
    extends JPanel
{
    public ConfigViewer()
    {
	initComponents();
	setPort(BTunProtocol.DEFAULT_PORT);
    }

    /*
    private void btnOKActionPerformed(ActionEvent e)
    {
	// TODO add your code here
    }
    
    private void btnCancelActionPerformed(ActionEvent e)
    {
	// TODO add your code here
    }
    */

    public void addActionListener(ActionListener l)
    {
	btnOK.addActionListener(l);
    }

    public void setLogin(String s) { txtLogin.setText(s); }
    public String getLogin() { return txtLogin.getText(); }

    public void setPasswd(String s) { txtPass.setText(s); }
    public String getPasswd() { return new String(txtPass.getPassword()); }

    public void setServer(String s) { txtServer.setText(s); }
    public String getServer() { return txtServer.getText(); }

    public void setMachine(String s) { txtMachine.setText(s); }
    public String getMachine() { return txtMachine.getText(); }

    public void setPort(int s) { txtPort.setText("" + s); }
    public int getPort()
    {
	try
	{
	    return Integer.parseInt(txtPort.getText());
	}
	catch(NumberFormatException ex)
	{
	    setPort(BTunProtocol.DEFAULT_PORT);
	    return BTunProtocol.DEFAULT_PORT;
	}
    }

    public boolean isShell() { return rdShell.isSelected(); }

	private void initComponents() {
		// JFormDesigner - Component initialization - DO NOT MODIFY  //GEN-BEGIN:initComponents
		// Generated using JFormDesigner Evaluation license - Nicolas GUILLAUME
		label5 = new JLabel();
		vSpacer1 = new JPanel(null);
		label1 = new JLabel();
		txtServer = new JTextField();
		label6 = new JLabel();
		txtPort = new JTextField();
		label2 = new JLabel();
		txtLogin = new JTextField();
		label3 = new JLabel();
		txtPass = new JPasswordField();
		label4 = new JLabel();
		txtMachine = new JTextField();
		rdVNC = new JRadioButton();
		rdShell = new JRadioButton();
		vSpacer2 = new JPanel(null);
		panel1 = new JPanel();
		btnOK = new JButton();
		btnCancel = new JButton();

		//======== this ========
		{


			this.setLayout(new GridBagLayout());
			((GridBagLayout)this.getLayout()).columnWidths = new int[] {0, 0, 0, 0, 0};
			((GridBagLayout)this.getLayout()).rowHeights = new int[] {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			((GridBagLayout)this.getLayout()).columnWeights = new double[] {0.0, 0.0, 0.0, 0.0, 1.0E-4};
			((GridBagLayout)this.getLayout()).rowWeights = new double[] {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0E-4};

			//---- label5 ----
			label5.setText("BTun Viewer");
			label5.setFont(new Font("Dialog", Font.BOLD, 16));
			label5.setHorizontalAlignment(SwingConstants.RIGHT);
			this.add(label5, new GridBagConstraints(1, 0, 1, 1, 0.0, 0.0,
				GridBagConstraints.CENTER, GridBagConstraints.BOTH,
				new Insets(0, 0, 4, 4), 0, 0));
			this.add(vSpacer1, new GridBagConstraints(1, 1, 1, 1, 0.0, -1.0,
				GridBagConstraints.CENTER, GridBagConstraints.BOTH,
				new Insets(0, 0, 4, 4), 0, 0));

			//---- label1 ----
			label1.setText("Server:");
			label1.setLabelFor(txtServer);
			this.add(label1, new GridBagConstraints(0, 2, 1, 1, 0.0, 0.0,
				GridBagConstraints.CENTER, GridBagConstraints.BOTH,
				new Insets(0, 0, 4, 4), 0, 0));

			//---- txtServer ----
			txtServer.setColumns(16);
			this.add(txtServer, new GridBagConstraints(1, 2, 1, 1, 0.0, 0.0,
				GridBagConstraints.CENTER, GridBagConstraints.BOTH,
				new Insets(0, 0, 4, 4), 0, 0));

			//---- label6 ----
			label6.setText("Port:");
			label6.setLabelFor(txtPort);
			this.add(label6, new GridBagConstraints(2, 2, 1, 1, 0.0, 0.0,
				GridBagConstraints.CENTER, GridBagConstraints.BOTH,
				new Insets(0, 0, 4, 4), 0, 0));

			//---- txtPort ----
			txtPort.setColumns(5);
			this.add(txtPort, new GridBagConstraints(3, 2, 1, 1, 0.0, 0.0,
				GridBagConstraints.CENTER, GridBagConstraints.BOTH,
				new Insets(0, 0, 4, 0), 0, 0));

			//---- label2 ----
			label2.setText("Login:");
			label2.setLabelFor(txtLogin);
			this.add(label2, new GridBagConstraints(0, 3, 1, 1, 0.0, 0.0,
				GridBagConstraints.CENTER, GridBagConstraints.BOTH,
				new Insets(0, 0, 4, 4), 0, 0));
			this.add(txtLogin, new GridBagConstraints(1, 3, 1, 1, 0.0, 0.0,
				GridBagConstraints.CENTER, GridBagConstraints.BOTH,
				new Insets(0, 0, 4, 4), 0, 0));

			//---- label3 ----
			label3.setText("Password:");
			label3.setLabelFor(txtPass);
			this.add(label3, new GridBagConstraints(0, 4, 1, 1, 0.0, 0.0,
				GridBagConstraints.CENTER, GridBagConstraints.BOTH,
				new Insets(0, 0, 4, 4), 0, 0));
			this.add(txtPass, new GridBagConstraints(1, 4, 1, 1, 0.0, 0.0,
				GridBagConstraints.CENTER, GridBagConstraints.BOTH,
				new Insets(0, 0, 4, 4), 0, 0));

			//---- label4 ----
			label4.setText("Machine:");
			label4.setLabelFor(txtMachine);
			this.add(label4, new GridBagConstraints(0, 5, 1, 1, 0.0, 0.0,
				GridBagConstraints.CENTER, GridBagConstraints.BOTH,
				new Insets(0, 0, 4, 4), 0, 0));
			this.add(txtMachine, new GridBagConstraints(1, 5, 1, 1, 0.0, 0.0,
				GridBagConstraints.CENTER, GridBagConstraints.BOTH,
				new Insets(0, 0, 4, 4), 0, 0));

			//---- rdVNC ----
			rdVNC.setText("VNC");
			rdVNC.setSelected(true);
			this.add(rdVNC, new GridBagConstraints(1, 6, 1, 1, 0.0, 0.0,
				GridBagConstraints.CENTER, GridBagConstraints.BOTH,
				new Insets(0, 0, 4, 4), 0, 0));

			//---- rdShell ----
			rdShell.setText("Shell");
			this.add(rdShell, new GridBagConstraints(1, 7, 1, 1, 0.0, 0.0,
				GridBagConstraints.CENTER, GridBagConstraints.BOTH,
				new Insets(0, 0, 4, 4), 0, 0));
			this.add(vSpacer2, new GridBagConstraints(1, 8, 1, 1, 0.0, -1.0,
				GridBagConstraints.CENTER, GridBagConstraints.BOTH,
				new Insets(0, 0, 4, 4), 0, 0));

			btnGroup = new ButtonGroup();
			btnGroup.add(rdVNC);
			btnGroup.add(rdShell);

			//======== panel1 ========
			{
				panel1.setLayout(new GridBagLayout());
				((GridBagLayout)panel1.getLayout()).columnWidths = new int[] {0, 0, 0, 0, 0};
				((GridBagLayout)panel1.getLayout()).rowHeights = new int[] {0, 0};
				((GridBagLayout)panel1.getLayout()).columnWeights = new double[] {0.0, 0.0, 0.0, 0.0, 1.0E-4};
				((GridBagLayout)panel1.getLayout()).rowWeights = new double[] {0.0, 1.0E-4};

				//---- btnOK ----
				btnOK.setText("Connect");
				/*	btnOK.addActionListener(new ActionListener() {
					public void actionPerformed(ActionEvent e) {
						btnOKActionPerformed(e);
						}
						});*/
				panel1.add(btnOK, new GridBagConstraints(1, 0, 1, 1, 0.0, 0.0,
					GridBagConstraints.CENTER, GridBagConstraints.BOTH,
					new Insets(0, 0, 0, 2), 0, 0));

				//---- btnCancel ----
				btnCancel.setText("Cancel");
				/*	btnCancel.addActionListener(new ActionListener() {
					public void actionPerformed(ActionEvent e) {
						btnCancelActionPerformed(e);
						}
						});*/
				/*panel1.add(btnCancel, new GridBagConstraints(3, 0, 1, 1, 0.0, 0.0,
					GridBagConstraints.CENTER, GridBagConstraints.BOTH,
					new Insets(0, 0, 0, 0), 0, 0));
				*/
			}
			this.add(panel1, new GridBagConstraints(1, 9, 1, 1, 0.0, 0.0,
				GridBagConstraints.CENTER, GridBagConstraints.VERTICAL,
				new Insets(0, 0, 4, 4), 0, 0));
		}
		// JFormDesigner - End of component initialization  //GEN-END:initComponents
	}

	// JFormDesigner - Variables declaration - DO NOT MODIFY  //GEN-BEGIN:variables
	// Generated using JFormDesigner Evaluation license - Nicolas GUILLAUME

	private JLabel label5;
	private JPanel vSpacer1;
	private JLabel label1;
	private JTextField txtServer;
	private JLabel label6;
	private JTextField txtPort;
	private JLabel label2;
	private JTextField txtLogin;
	private JLabel label3;
	private JPasswordField txtPass;
	private JLabel label4;
	private JTextField txtMachine;
	private JRadioButton rdVNC;
	private JRadioButton rdShell;
	private JPanel vSpacer2;
	private JPanel panel1;
	private JButton btnOK;
	private JButton btnCancel;
        private ButtonGroup btnGroup;
	// JFormDesigner - End of variables declaration  //GEN-END:variables

}
