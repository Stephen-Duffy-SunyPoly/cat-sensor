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
		
		Socket soc = new Socket("DOCKER_AI_CONTAINER_NAME",0);
		//send that imcoming data to the AI Server
		request.getInputStream().transferTo(soc.getOutputStream());
		
		byte[] result = soc.getInputStream().readAllBytes();
		
		
		String resp = new String(result);
		
		resp = resp.toLowerCase();
		soc.close();
		response.getWriter().print((resp.equals("true"))?1:0);
	}

}
