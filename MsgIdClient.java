import java.net.*;
import java.io.*;

public class MsgIdClient {

    public static String getMsgId()
    {
	String temp = null;
	Socket socket;
	BufferedReader in = null;
	OutputStreamWriter out = null;

	try {
	    socket = new Socket(InetAddress.getByName("192.168.1.1"), 16890);
	    in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
	    out = new OutputStreamWriter(socket.getOutputStream());
	
	    out.write("getMsgId");
	    out.flush();
	    temp = in.readLine();

	}	    

	catch (Exception e)
	    {
		System.out.println("No MsgId Server available");
	    }
	
	return temp;
    }

    public static void main(String argv[])
    {
	MsgIdClient client = new MsgIdClient();
	int i;

	for (i = 0; i < 10 ; i++)
	    System.out.println("MsgId #" + i + ": " + client.getMsgId());

    }

}
