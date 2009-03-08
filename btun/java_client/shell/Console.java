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


public class Cconsole
    extends JTextArea
{
    
    public static final Font FIXED_FONT = new Font("Monospaced", Font.BOLD, 12);
    
    public Console(Console c)
    {
	this();
	setText(c.getText());
        
        PeriphInDocument doc = getPeriphDocument();
        doc.setLockedUntilSelection();
        doc.setPrompt(c.getPeriphDocument().prompt());
    }
    
    public Console()
    {
        setForeground(Color.lightGray);
        setBackground(Color.black);
	setCaretColor(Color.lightGray);
        setFont(FIXED_FONT);
	setLineWrap(true);
        
    }

    public Console(String prompt)
    {
        this();
        getPeriphDocument().setPrompt(prompt);
    }
    
    
    


    public PeriphInDocument getPeriphDocument()
    {
	return (PeriphInDocument)super.getDocument();
    }
    
    protected Document createDefaultModel()
    {
        return new PeriphInDocument();
    }
        

     /** Document pour le PeriphTextArea, gère notamment un prompt et l'entrée */
     public static class PeriphInDocument
         extends PlainDocument
     {
        private String newText;
        private int lastPrompt;
        
        protected String prompt;
        
        /** Systeme simpliste d'"action" listener*/
        private InputListener il; 
    
        public void setLockedUntilSelection()
        {
            newText = "";
            lastPrompt = getLength();
        }
        
        /** Ajoute un InputListener (en fait on ne peut en mettre qu'un) */
        public void addInputListener(InputListener il)
        {
            this.il = il;
        }
        
        public PeriphInDocument()
        {
	    resetInput();
        }

        /** Reset de la valeur entrée */
	public void resetInput()
        {
	    newText = "";
	    lastPrompt = 0;
        }
        
        /** Change le prompt */
        public void setPrompt(String str)
        {
            prompt = str;
        }
        
        /** Renvoie le prompt */
        public String prompt()
        {
            return prompt;
        }
        
        public void insertString(int offs, String str, AttributeSet a) 
                throws BadLocationException
        {
    
                if (str == null || str.length() == 0)
                    return;
                    
                if( !(offs <= getLength() && offs >= lastPrompt)  )
                {
                    //GUIRes.beep();
                    return;
                }
                
                newText += str;
                    
                if(str.charAt(str.length() - 1) == '\n')
                {
                    if(il != null)
                        il.newInput(newText);
                        
                    str += prompt;
                    lastPrompt = offs + str.length();
                    newText = "";
                }

                
                super.insertString(offs, str, a);
        }
        
        public void remove(int offs, int len)
            throws BadLocationException
        {

            if(len == 1 && ( offs <= getLength() && offs >= lastPrompt) )
                super.remove(offs, len);
            /*else
	      GUIRes.beep();*/
        }
    }
}


/** Permet de faire un systeme simpliste d'"action" listener*/
interface InputListener
{
    public void newInput(String str);
}

