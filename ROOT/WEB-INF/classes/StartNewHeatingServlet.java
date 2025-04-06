import java.io.IOException;
import java.sql.SQLException;

import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

public class StartNewHeatingServlet extends HttpServlet {

	private static final long serialVersionUID = 4883792011643515326L;
	
	@Override
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		try {
			long entryID = DatabaseManager.startNewHeatingTimeNow();
			response.getWriter().print(entryID);
			response.setContentType("text/plain");
		} catch (SQLException e) {
			e.printStackTrace();
			response.setStatus(500);
			response.getWriter().print("{\"error\": \"Database Error(2)\"}");
		}
	}

}
