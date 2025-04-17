import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.Socket;
import java.net.URL;

import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

public class IsCatServlet extends HttpServlet {
	private static final long serialVersionUID = 2392334474444801685L;
	
	@Override
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		//System.out.println("Starting Socket");
		Socket soc = new Socket("ai-server",33333);
		//send that imcoming data to the AI Server
		//System.out.println("Connected");
		request.getInputStream().transferTo(soc.getOutputStream());
		soc.shutdownOutput();//force end of stream so if less data then the buffer amount is sent it will not stay in an infinite loop forever
		//System.out.println("Input passed in");
		byte[] result = soc.getInputStream().readAllBytes();
		//System.out.println("output recieved");
		
		String resp = new String(result);
		//System.out.println("Rcieved the following resilt: ("+resp+")");
		resp = resp.toLowerCase();
		soc.close();
		response.getWriter().print((resp.equals("true"))?1:0);
	}

}
