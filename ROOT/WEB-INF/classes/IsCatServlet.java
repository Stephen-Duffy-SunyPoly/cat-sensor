import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.URL;

import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

public class IsCatServlet extends HttpServlet {
	private static final long serialVersionUID = 2392334474444801685L;
	
	@Override
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		
		URL aiServerURL = new URL("http://AI_DOCER_CONTAINER_NAME:port");
		HttpURLConnection con = (HttpURLConnection) aiServerURL.openConnection();
		con.setRequestMethod("POST");
		con.setDoOutput(true);
		request.getInputStream().transferTo(con.getOutputStream());
		
		con.connect();
		response.setStatus(con.getResponseCode());
		//get the response
		String resp = new String(con.getInputStream().readAllBytes());
		
		resp = resp.toLowerCase();
		boolean result = false;
		if(resp.equals("true")) {
			result = true;
		}
		
		if(result) {
			response.getWriter().print(1);
		}else {
			response.getWriter().print(0);
		}
	}

}
