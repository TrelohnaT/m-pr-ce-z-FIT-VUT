package ija.ija2019.homework2.maps;

public interface Stop {
	
	public String getId();
	
    public Coordinate getCoordinate();
	
    public void setStreet(Street s);
	
	public Street getStreet();
    
    public static Stop defaultStop​(String id, Coordinate c)
    {
    	return new Stop_cl(id, c);
    }
	
}
