import java.io.IOException;
import java.sql.SQLException;

import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

public class EndHeatingTimeServlet extends HttpServlet {

	private static final long serialVersionUID = 1595705909685112822L;
	
	@Override
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		String idString = request.getParameter("id");
		if(idString == null) {
			response.setStatus(400);
			response.getWriter().print("no id provided");
			return;
		}
		long id = Long.parseLong(idString);
		
		try {
			DatabaseManager.endHeatingTimeNow(id);
		} catch (SQLException e) {
			e.printStackTrace();
			response.setStatus(500);
			response.getWriter().print("{\"error\": \"Database Error(3)\"}");
		}
		
	}
}
