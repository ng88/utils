import java.io.DataInputStream;
import java.io.InputStream;
import java.io.RandomAccessFile;
import java.nio.channels.Channels;


public class Test
{
	public static void main(String [] argv)
	{
		try
		{
			RandomAccessFile archive = new RandomAccessFile(argv[0], "r");
		    InputStream is = Channels.newInputStream(archive.getChannel());
		    
			TSFStreamReader ar = new TSFStreamReader(new DataInputStream(is));
		
			ar.setProgressListener(new TSFStreamReader.ProgressListener()
			{

				@Override
				public boolean onEntryBegin(TSFStreamReader a, String name, int requiredSpace)
				{
					System.out.print(" + " + name + " ... ");
					return true;
				}

				@Override
				public void onEntryEnd(TSFStreamReader a, String name)
				{
					System.out.println("OK");
				}

				@Override
				public boolean onExtractionBegin(TSFStreamReader a)
				{
					System.out.println("Begin needSpace=" + a.getExtractSize() + " for " + a.getEntryCount() + " elements");
					return true;
				}

				@Override
				public void onExtractionEnd(TSFStreamReader a)
				{
					System.out.println("End");	
				}
				
			});
			
			ar.extractTo(argv[1]);
			
			is.close();
			archive.close();
		
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		
	}
}
