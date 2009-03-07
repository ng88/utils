package crypto;


import java.security.*;


public class MD5
{

    protected static MessageDigest MD5D = null;

    public static final int SIZE = 16;

    public static String encode(String key)
    {
	byte[] hash = encodeBytes(key);

	StringBuffer hashString = new StringBuffer();

	for(int i = 0; i < hash.length; ++i)
	{

	    String hex = Integer.toHexString(hash[i]);

	    if(hex.length() == 1)
	    {
		hashString.append('0');
		hashString.append(hex.charAt(hex.length() - 1));

	    }
	    else
		hashString.append(hex.substring(hex.length() - 2));
	}
	return hashString.toString();
    }


    public static byte[] encodeBytes(String key)
    {
	return getMessageDigest().digest(key.getBytes());
    }

    public static MessageDigest getMessageDigest()
    {

	try
	{
	    if(MD5D == null)
		MD5D = MessageDigest.getInstance("MD5");

	}
	catch (NoSuchAlgorithmException e)
	{
	    throw new Error("no MD5 support in this VM");
	}


	return MD5D;
    }

}

