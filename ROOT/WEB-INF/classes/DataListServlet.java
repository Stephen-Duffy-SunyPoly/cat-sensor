import java.io.IOException;
import java.sql.SQLException;
import java.util.ArrayList;

import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

public class DataListServlet extends HttpServlet {


	private static final long serialVersionUID = 1L;
	
	//tmp
	TimeData td1 = new TimeData("10:00","11:00","1:00"),td2 = new TimeData("10:00","11:00","1:00"),td3 = new TimeData("10:00","11:00","1:00");
	
	//wen a get request reaches this end point
	@Override
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		//set the response content type
		response.setContentType("application/json");
		//get the data and format it into a json array
		ArrayList<TimeData> timeData;
		try {
			timeData = getTimeData();
		} catch (SQLException e) {
			response.setStatus(500);
			response.getWriter().print("{\"error\": \"Database Error(1)\"}");
			e.printStackTrace();
			return;
		}
		String[] timeDataJsons = timeData.stream().map(data -> data.toJson()).toArray(String[]::new);
		String stringafitedJSON = String.join(",",timeDataJsons);
		//send the data
		response.getWriter().print("{\"data\": ["+stringafitedJSON+"]}");
	}
	
	public ArrayList<TimeData> getTimeData() throws SQLException{
		//get data from the database
		ArrayList<TimeData> data = DatabaseManager.getListData();

		return data;
	}

}