import java.io.IOException;
import java.net.Socket;

import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

public class IsCatServlet extends HttpServlet {
	private static final long serialVersionUID = 2392334474444801685L;
	
	//when a post request is received for the path /isCat
	@Override
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		//create the connection to the AI server
		Socket soc = new Socket("ai-server",33333);
		
		//send that incoming data to the AI Server
		request.getInputStream().transferTo(soc.getOutputStream());
		soc.shutdownOutput();//force end of stream so if less data then the buffer amount is sent it will not stay in an infinite loop forever
		
		//get the response from the AI
		byte[] result = soc.getInputStream().readAllBytes();
		
		//convert the result into a string
		String resp = new String(result);
		resp = resp.toLowerCase();
		//close the connection to the AI
		soc.close();
		
		//sent a response to the device as the charter of 1 or 0
		response.getWriter().print((resp.equals("true"))?1:0);
	}

}
