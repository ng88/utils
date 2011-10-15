/***************************************************************************
 * Tar Stream Format                                                       *
 * Tar-like archiving that can be easily streamed over HTTP                *
 ***************************************************************************/

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

import java.io.DataInputStream;
import java.io.File;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.zip.Adler32;
import java.util.zip.DataFormatException;
import java.util.zip.Inflater;

public class TSFStreamReader
{
	public final static int TSF_VERSION = 1;
	public final static int TSF_MAGIC =  0x54534621;

	public final static int TSF_FILE_HEADER_SIZE = 4+2+2+4+4+4;
	public final static int TSF_ENTRY_HEADER_SIZE = 4+4+4+4+4+2;

	public final static int TSF_EF_RAW = 0;
	public final static int TSF_EF_XOR = 1;
	public final static int TSF_EF_GZ = 2;

	private int entryCount;
	private int extractSize; // space needed to extract all files in bytes
	private int archiveSize; // size of this archive file in bytes (including headers)

	private Adler32 sum = null;
	private boolean checkIntegrity; // check data integrity
	
	private byte[] buffer;

	private Inflater inflater = null;
	private byte[] bufferInflate = null;

	private volatile boolean cancel;

	private DataInputStream in;

	private ProgressListener progressListener;

	public static interface ProgressListener
	{
		// return false to cancel
		
		public boolean onExtractionBegin(TSFStreamReader a);
		public boolean onEntryBegin(TSFStreamReader a, String name, int requiredSpace);
		public void onEntryEnd(TSFStreamReader a, String name);
		public void onExtractionEnd(TSFStreamReader a);
	}

	public TSFStreamReader(DataInputStream in)
	{
		this(in, 8192);
	}

	public TSFStreamReader(DataInputStream in, int bufferSize)
	{
		this(in, bufferSize, false);
	}

	public TSFStreamReader(DataInputStream in, int bufferSize,  boolean checkIntegrity)
	{
		this.checkIntegrity = checkIntegrity;
		buffer = new byte[bufferSize];
		this.in = in;
	}

	public void extractTo(String dirName) throws IOException
	{
		cancel = false;
		
		readFileHeader();
		
		if(progressListener != null)
			if(!progressListener.onExtractionBegin(this))
				return;
	
		for(int i = 0; i < entryCount; i++)
			if(!extractEntry(dirName))
				return;

		if(progressListener != null)
			progressListener.onExtractionEnd(this);
	}
	
	/**
	 * Cancel streaming
	 */
	public void cancel()
	{
		cancel = true;
	}

	private void readFileHeader() throws IOException
	{
		int v, maxV;

		// magic field
		v = in.readInt();
		if(v != TSF_MAGIC)
			throw new TSFException("Invalid file format!");

		// file version
		v = in.readShort();
		maxV = in.readShort();
		if(maxV > TSF_VERSION)
			throw new TSFException("This file require a more recent version (" + maxV +").");

		checkInt(entryCount = in.readInt());
		checkInt(extractSize = in.readInt());
		checkInt(archiveSize = in.readInt());
	}

	private boolean extractEntry(String baseFolder) throws IOException
	{
		int entrySize, entryExtractSize, n;
		int checkSum, format, arg;
		String name;

		checkInt(entrySize = in.readInt());
		checkInt(entryExtractSize = in.readInt());

		// format
		format = in.readInt();
		// arg
		arg = in.readInt(); 
		// sum
		checkSum = in.readInt();
		// name
		n = in.readUnsignedShort();

		if(n > buffer.length)
			buffer = new byte[n];

		in.readFully(buffer, 0, n);

		name = new String(buffer, 0, n, "ISO-8859-1");

		if(progressListener != null)
			if(!progressListener.onEntryBegin(this, name, entryExtractSize))
				return false;
		
		File dest = new File(baseFolder, name);
		 
		switch(format)
		{
		case TSF_EF_RAW:
			if(arg == 1)
				dest.mkdirs();
			else
				createRawFile(dest, entrySize, checkSum);
			break;
		case TSF_EF_GZ:
			createGZFile(dest, entrySize, entryExtractSize, checkSum);
			break;
		default:
			throw new TSFException("Unsupported entry format!");
		}

		if(progressListener != null)
			 progressListener.onEntryEnd(this, name);
		return true;
	}

	private void createGZFile(File dest, int inSize, int outSize, int expectedSum) throws IOException
	{
		if(inflater == null)
		{
			inflater = new Inflater();
			bufferInflate = new byte[buffer.length];
		}
		else
			inflater.reset();
		
		final RandomAccessFile dst = new RandomAccessFile(dest, "rw");
		int n;
		int remaining = inSize;
		try
		{
			do
			{
				// read
				final int toRead = Math.min(remaining, buffer.length);	
				n = in.read(buffer, 0, toRead);
				if(n > 0)
				{
					remaining -= n;
					inflater.setInput(buffer, 0, n);
					do
					{
						// uncompress
						n = inflater.inflate(bufferInflate);
						// write
						dst.write(bufferInflate, 0, n);
					}
					while(!inflater.needsInput());
				}
			}
			while(n != -1 && remaining > 0 && !cancel);
		}
		catch(DataFormatException ex)
		{
			throw new TSFException("GZ format error " + ex.getMessage());
		}
		if(remaining > 0)
			throw new TSFException("Missing data for " + dest.getAbsolutePath());
		
		dst.close();
	}

	
	private void createRawFile(File dest, int size, int expectedSum) throws IOException
	{
		if(checkIntegrity)
		{
			if(sum == null)
				sum = new Adler32();
			else
				sum.reset();
		}
		final RandomAccessFile dst = new RandomAccessFile(dest, "rw");
		int n;
		int remaining = size;
		do
		{
			final int toRead = Math.min(remaining, buffer.length);	
			n = in.read(buffer, 0, toRead);
			if(n > 0)
			{
				remaining -= n;
				dst.write(buffer, 0, n);
			}
		}
		while(n != -1 && remaining > 0 && !cancel);
		
		if(remaining > 0)
			throw new TSFException("Missing data for " + dest.getAbsolutePath());
		
		dst.close();
	}

	// no unsigned int, so avoid this case (this is enough for we want to do)
	private static void checkInt(int v) throws IOException
	{
		if(v < 0)
			throw new TSFException("Incompatible file, this reader can only read file with field < 2 GiB");
	}

	public final ProgressListener getProgressListener() { return progressListener; }
	public final void setProgressListener(ProgressListener progressListener) { this.progressListener = progressListener; }

	public final int getEntryCount() { return entryCount; }
	public final int getExtractSize() { return extractSize; }
	public final int getArchiveSize() {	return archiveSize;	}
}
