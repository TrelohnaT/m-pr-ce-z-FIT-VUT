package ija.ija2019.homework2.maps;

public class Stop_cl implements Stop{
	
	public String stop_id;
	public Coordinate place;
	public Street stop_street = null;
	
    public String getId() 
    {
        return this.stop_id;
    }
	
    public Coordinate getCoordinate() 
    {
        return this.place;
    }
	
    public void setStreet(Street s) 
    {
    	this.stop_street = s;
    }

    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof Stop)) return false;

        Stop stop = (Stop) o;

        if (getId() != null ? !getId().equals(stop.getId()) : stop.getId() != null) return false;
        if (getCoordinate() != null ? !getCoordinate().equals(stop.getCoordinate()) : stop.getCoordinate() != null)
            return false;
        return getStreet() != null ? getStreet().equals(stop.getStreet()) : stop.getStreet() == null;
    }
	

	public Street getStreet()
    {
        return this.stop_street;
    }
    
    public Stop_cl(String stop_id, Coordinate place)
    {
    	this.stop_id = stop_id;
    	this.place = place;
    }
    
    
    public String toString()
    {
    	return String.format("stop("+stop_id+")");
    }
	
}
