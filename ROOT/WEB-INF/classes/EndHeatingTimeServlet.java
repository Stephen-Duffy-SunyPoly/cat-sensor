import java.io.IOException;
import java.sql.SQLException;

import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

public class EndHeatingTimeServlet extends HttpServlet {

	private static final long serialVersionUID = 1595705909685112822L;
	
	//when a get request is received for the path /heatingStopped
	@Override
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		//get the entry ID from the URL parameter
		String idString = request.getParameter("id");
		if(idString == null) {
			//if the id was not included then return an error
			response.setStatus(400);
			response.getWriter().print("no id provided");
			return;
		}
		//convert the id into a number
		long id = Long.parseLong(idString);
		
		try {
			//record then end time in the database
			DatabaseManager.endHeatingTimeNow(id);
		} catch (SQLException e) {
			e.printStackTrace();
			response.setStatus(500);
			response.getWriter().print("{\"error\": \"Database Error(3)\"}");
		}
		
	}
}
