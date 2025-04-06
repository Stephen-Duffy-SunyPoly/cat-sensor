import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;

public class DatabaseManager {
	
	private static Connection dataBaseConnection_DO_NOT_USE;
	
	static private synchronized void setupConncetion() throws SQLException {
		String connectionProtocal = "jdbc:postgresql://";
	    String databaseAddress = "127.0.0.1:5432";
	    String databaseName = "warm-cat";
	    String databaseUsername = System.getenv("DATABASE_USER");
	    String databasePassword = System.getenv("DATABASE_PASSWORD");
		
		if(dataBaseConnection_DO_NOT_USE == null || dataBaseConnection_DO_NOT_USE.isClosed()) {
			
			dataBaseConnection_DO_NOT_USE = DriverManager.getConnection(connectionProtocal+databaseAddress+"/"+databaseName, databaseUsername, databasePassword);
		}
	}
	
	private static Connection getConnection() throws SQLException {
		if(dataBaseConnection_DO_NOT_USE == null || dataBaseConnection_DO_NOT_USE.isClosed()) {
			setupConncetion();
		}
		return dataBaseConnection_DO_NOT_USE;
	}
	
	
	public static ArrayList<TimeData> getListData() throws SQLException{
		Connection con = getConnection();
		ArrayList<TimeData> data = new ArrayList<>();
		PreparedStatement querey = con.prepareStatement("SELECT heatingStartTime, heatingEndTime, totalHeatTime FROM heatPlateData");
		querey.execute();
		ResultSet rs = querey.getResultSet();
		
		while(rs.next()) {
			//TODO pare the date info returned
			data.add(new TimeData(rs.getString(1), rs.getString(2), rs.getString(3)));
		}
		
		return data;
	}
}
