package ija.ija2019.homework2.maps;

import java.util.*;

public interface Line 
{

	static Line defaultLine(String id)
    {
        return new Line_cl(id);
    }
	
	public boolean addStop​(Stop stop);
	
	public boolean addStreet​(Street street);
	
	public List<AbstractMap.SimpleImmutableEntry<Street, Stop>> getRoute();
	
	
	
	
	

	
}
