
public class TimeData {
	String heatingStartTime;
	String heatingEndTime;
	String totalHeatTime;
	
	public TimeData(String hts, String het, String tht) {
		heatingStartTime = hts;
		heatingEndTime = het;
		totalHeatTime = tht;
	}
	
	public String toJson() {
		return "{\"activeTime\" : \""+heatingStartTime+"\", \"deactivatedTime\": \""+heatingEndTime+"\", \"heatTime\": \""+totalHeatTime+"\"}";
	}
}
