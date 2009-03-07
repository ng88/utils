

import btun.*;


public class Test
{

    public static void main(String[] a)
    {
	try
	{
	   
	    BTunClient b = new BTunClient("test", "test2", "localhost");

	    b.connectToChannel("MaChannel");

	    System.out.println(b);


	    b.disconnect();

	}
	catch(Exception ex)
	{
	    ex.printStackTrace();
	}
    }

}
