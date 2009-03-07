
package btun;

import crypto.MD5;
import java.nio.charset.Charset;
import java.security.*;

public class BTunProtocol
{

    public static final Charset CHARSET = Charset.forName("US-ASCII");

    public static final int DEFAULT_PORT = 12358;

    public static final int USER_MAX_PASS_SIZE = 64;
    public static final int USER_MAX_LOGIN_SIZE = 8;
    public static final int USER_MAX_CHANNEL_SIZE = 16;
    public static final int CHALLENGE_SIZE = 16;

    /** master/slave channel */
    public static final byte OPT_MASTER = 1;
    /** unrestricted channel */
    public static final byte OPT_UNRESTRICTED = 2;
    /** autoclose channel */
    public static final byte OPT_AUTOCLOSE = 4;

    /** user access  */
    public static final int UA_GRANTED = 0;
    public static final int UA_DENIED = 1;

    /** channel access  */
    public static final byte CA_GRANTED = 0;
    public static final byte CA_DENIED = 1;
    public static final byte CA_CANT_BE_MASTER = 2;
    public static final byte CA_TOO_MUCH_CHANNEL = 3;
    public static final byte CA_TOO_MUCH_USER = 4;
    public static final byte CA_CANT_CHPERM = 5;


    public static String caErrorToString(byte err)
    {
        switch(err)
	    {
	    case CA_GRANTED:
		return "";
	    case CA_DENIED:
		return "can't join channel: permission denied";
	    case CA_CANT_BE_MASTER:
		return "channel already exists, can't be the master.";
	    case CA_CANT_CHPERM:
		return "channel already exists, can't change permissions/options.";
	    case CA_TOO_MUCH_CHANNEL:
		return "can't create channel, too much channels!";
	    case CA_TOO_MUCH_USER:
		return "can't join channel, too much users!";
	    default:
		return "can't join channel, error " + err;
	    }
    }


    public static byte[] answerChallenge(byte[] ch, String pass)
    {
	MessageDigest md = MD5.getMessageDigest();
	md.update(ch);
	md.update(pass.getBytes(CHARSET));
	md.update(ch);

	return md.digest();
    }


    public static byte[] convertString(String s)
    {
	byte[] data = s.getBytes(CHARSET);
	byte[] ret = new byte[data.length + 1];

	System.arraycopy(data, 0, ret, 0, data.length);

	ret[data.length] = 0;
	return ret;
    }


    // debug
    public static void printba(byte[] a)
    {
	System.out.print("ta=");
	for(int i = 0; i < a.length; i++)
	    System.out.print(a[i] + " ");
	System.out.println();
    }

}
