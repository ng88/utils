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


public class Console
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
	this("");
    }

    public Console(String prompt)
    {
        setForeground(Color.lightGray);
        setBackground(Color.black);
	setCaretColor(Color.lightGray);
        setFont(FIXED_FONT);
	setLineWrap(true);

	//((DefaultCaret)getCaret()).setUpdatePolicy(DefaultCaret.ALWAYS_UPDATE);

        getPeriphDocument().setPrompt(prompt);
    }

    /**  Thread safe
     */
    public synchronized void printString(String str, boolean blockInputEvent)
    {
	if(blockInputEvent)
	    getPeriphDocument().blockInputEvent(true);

        append(str);
        getPeriphDocument().setLockedUntilSelection();
	setCaretPosition(getDocument().getLength());

	if(blockInputEvent)
	    getPeriphDocument().blockInputEvent(false);
    }
 
    public void printString(String str)
    {
        printString(str, true);
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

	 protected volatile boolean block;
        
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
	    block = false;
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
                    if(il != null && !block)
                        il.newInput(newText);
                        
                    str += prompt;
                    lastPrompt = offs + str.length();
                    newText = "";
                }

                
                super.insertString(offs, str, a);
        }

	 public synchronized void blockInputEvent(boolean b)
	 {
	     block = b;
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

