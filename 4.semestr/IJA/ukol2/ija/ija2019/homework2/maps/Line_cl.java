package ija.ija2019.homework2.maps;

import java.util.*;

public class Line_cl implements Line {
	
	String line_id;
    List <Stop> rosterStops = new ArrayList<Stop>();
    List <Street> rosterStreets = new ArrayList<Street>();
    List<AbstractMap.SimpleImmutableEntry<Street, Stop>> rosterStreetStops = new ArrayList<>();
    
    	public String getId() 
    	{
    		return line_id;
        }

        public void setId(String newValue) 
        {
            line_id = newValue;
        }

        public List<Stop> getRosterStops()
        {
            return rosterStops;
        }

        public void setRosterStops(List<Stop> newValue) 
        {
        	rosterStops = newValue;
        }

        public List<Street> getRosterstreets()
        {
            return rosterStreets;
        }

        public void setRosterstreets(List<Street> newValue) 
        {
        	rosterStreets = newValue;
        }
        
        public Line_cl(String id) 
        {
            this.line_id = id;
        }

        public Line_cl(List<Street> rosterStreets) 
        {
        this.rosterStreets = rosterStreets;
        }

        public Line_cl(List<Stop> rosterStops, List<Street> rosterStreets) {
        this.rosterStops = rosterStops;
        this.rosterStreets = rosterStreets;
    }
        
        @Override
        public boolean addStop​(Stop stop)
        {
            if (rosterStreetStops.isEmpty())
            {
            	rosterStops.add(stop);
            	rosterStreets.add(stop.getStreet());
                rosterStreetStops.add(new AbstractMap.SimpleImmutableEntry<Street, Stop>(stop.getStreet(), stop));
                return true;
            }

            else if(stop.getStreet().follows​(rosterStreetStops.get(rosterStreetStops.size()-1).getKey()))
            {
            	rosterStops.add(stop);
            	rosterStreets.add(stop.getStreet());
                rosterStreetStops.add(new AbstractMap.SimpleImmutableEntry<Street, Stop>(stop.getStreet(), stop));
                return true;
            }
            else
            {
                return false;
            }
        }

        public boolean addStreet​(Street street)
        {

            if(street.follows​(rosterStreets.get(rosterStreets.size()-1)))
            {
            	rosterStreetStops.add(new AbstractMap.SimpleImmutableEntry<Street, Stop>(street, null));
                return true;
            }
            else
            {
                return false;
            }
        }

        public List<AbstractMap.SimpleImmutableEntry<Street, Stop>> getRoute()
        {

            return Collections.unmodifiableList(this.rosterStreetStops);
        	
        }
    

}
