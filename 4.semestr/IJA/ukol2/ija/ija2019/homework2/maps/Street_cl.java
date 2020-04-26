package ija.ija2019.homework2.maps;

import java.util.ArrayList;
import java.util.List;


public class Street_cl implements Street {

	String street_id;
	List<Coordinate> street_coordinate = new ArrayList<Coordinate>();
	List<Stop> street_stop = new ArrayList<Stop>();
	
    public String getId() 
    {
        return this.street_id;
    }

    public List<Coordinate> getCoordinates() 
    {
        return street_coordinate;
    }

    public List<Stop> getStops() 
    {
        return street_stop;
    }
    
    @Override
    public boolean addStop(Stop stop) 
    {

        for (int i = 0; i < street_coordinate.size(); i++) 
        {
            Coordinate first_coor =  street_coordinate.get(i - 1);
            Coordinate second_coor =  street_coordinate.get(i);
            Coordinate stop_coor = stop.getCoordinate();

            double all = Math.sqrt(Math.pow(first_coor.diffX(second_coor), 2) + Math.pow(first_coor.diffX(second_coor), 2));
            double fir = Math.sqrt(Math.pow(first_coor.diffX(stop_coor), 2) + Math.pow(first_coor.diffX(stop_coor), 2));
            double sec = Math.sqrt(Math.pow(second_coor.diffX(stop_coor), 2) + Math.pow(second_coor.diffX(stop_coor), 2));

            if (fir + sec == all) 
            {
                stop.setStreet​(this);
                street_stop.add(stop);
                return true;
            }
        }

        return false;

    }
    
    @Override
    public boolean equals(Object o)
    {
        if (this == o) return true;
        if (!(o instanceof Street_cl)) return false;

        Street_cl street_cl = (Street_cl) o;

        if (getId() != null ? !getId().equals(street_cl.getId()) : street_cl.getId() != null) return false;
        if (getCoordinates() != null ? !getCoordinates().equals(street_cl.getCoordinates()) : street_cl.getCoordinates() != null)
            return false;
        return getStops() != null ? getStops().equals(street_cl.getStops()) : street_cl.getStops() == null;
    }

	public Street_cl (String street_id, List<Coordinate> street_coordinate)
    {
		this.street_id = street_id;

        for (Coordinate oneCoordinate : street_coordinate)
        {
            this.street_coordinate.add(oneCoordinate);
        }
	}

    public Street_cl(String street_id, Coordinate coor_1, Coordinate coor_2)
    {
        this.street_id = street_id;
        street_coordinate.add(coor_1);
        street_coordinate.add(coor_2);
    }

    public Street_cl(String street_id, Coordinate coor_1, Coordinate coor_2, Coordinate coor_3)
    {
        this.street_id = street_id;
        street_coordinate.add(coor_1);
        street_coordinate.add(coor_2);
        street_coordinate.add(coor_3);
    }
	
	public Coordinate begin()
	{
		return street_coordinate.get(0);
	}
	
	public Coordinate end()
	{
		if (street_coordinate.size() == 2) 
        {
    		return street_coordinate.get(1);
		}
    	else if (street_coordinate.size() == 3)
        {
    		return street_coordinate.get(2);
    	}
    	else return null;
	}
	
	public boolean follows​(Street s)
	{
		int size = street_coordinate.size();
        int streetSize = s.getCoordinates().size();

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < streetSize; j++) {
                if (this.street_coordinate.get(i).equals(s.getCoordinates().get(j))) {
                    return true;
                }
            }

        }
        return false;
	}
	
	
	
}
