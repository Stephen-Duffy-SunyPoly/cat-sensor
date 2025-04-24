import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.time.Instant;
import java.util.ArrayList;

/**manage the connection and handle data transfer between the server and the database 
 */
public class DatabaseManager {
	
	private static Connection dataBaseConnection_DO_NOT_USE;
	
	static private synchronized void setupConncetion() throws SQLException {
		//define the connection parameters
		String connectionProtocal = "jdbc:postgresql://";
	    String databaseAddress = "postgres-db:5432";
	    String databaseName = "warm-cat";
	    //These are set in the DOCKERFILE
	    String databaseUsername = System.getenv("DATABASE_USER");
	    String databasePassword = System.getenv("DATABASE_PASSWORD");
		
	    //if the connection is not open or has not been created
		if(dataBaseConnection_DO_NOT_USE == null || dataBaseConnection_DO_NOT_USE.isClosed()) {
			//create the connection
			dataBaseConnection_DO_NOT_USE = DriverManager.getConnection(connectionProtocal+databaseAddress+"/"+databaseName, databaseUsername, databasePassword);
		}
	}
	
	
	private static Connection getConnection() throws SQLException {
		//if the connection needs to be reconnected
		if(dataBaseConnection_DO_NOT_USE == null || dataBaseConnection_DO_NOT_USE.isClosed()) {
			//try to create the connection
			setupConncetion();
		}
		return dataBaseConnection_DO_NOT_USE;
	}
	
	
	public static ArrayList<TimeData> getListData() throws SQLException{
		Connection con = getConnection();
		ArrayList<TimeData> data = new ArrayList<>();
		//prepare the database query
		PreparedStatement querey = con.prepareStatement("SELECT heatingStartTime, heatingEndTime, heatingEndTime-heatingStartTime FROM heatPlateData ORDER BY heatingStartTime DESC;");
		//run the query
		querey.execute();
		ResultSet rs = querey.getResultSet();
		//read/parse the results
		while(rs.next()) {
			data.add(new TimeData(rs.getString(1), rs.getString(2), rs.getString(3)));
		}
		//close the query
		querey.close();
		return data;
	}
	
	//the rest of these are very similar but with minor changes 
	
	public static long startNewHeatingTimeNow() throws SQLException {
		//INSERT INTO heatplatedata(heatingStartTime, heatingEndTime) VALUES (to_timestamp('1743917720'),to_timestamp('1743917820'))
		Connection con = getConnection();
		PreparedStatement querey = con.prepareStatement("INSERT INTO heatplatedata(heatingStartTime) VALUES (to_timestamp(?)) RETURNING id;");
		querey.setLong(1, Instant.now().getEpochSecond());
		querey.execute();
		
		ResultSet rs = querey.getResultSet();
		
		rs.next();
		long timeID = rs.getLong(1);
		querey.close();
		return timeID;
	}
	
	public static void endHeatingTimeNow(long timeId) throws SQLException {
		Connection con = getConnection();
		PreparedStatement querey = con.prepareStatement("UPDATE heatplatedata SET heatingEndTime = to_timestamp(?) WHERE id = ?;");
		querey.setLong(1, Instant.now().getEpochSecond());
		querey.setLong(2, timeId);
		querey.execute();
	}
}
