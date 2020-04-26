package ija.ija2019.homework2.maps;

import java.util.*;
import java.util.ArrayList;
import java.util.List;


import java.awt.geom.Point2D;

public interface Street {
	
	public String getId();
	
	public List<Coordinate> getCoordinates();
	
	public List<Stop> getStops();
	
	public boolean addStop(Stop stop);
	
	public Coordinate begin();
	
	public Coordinate end();
	
	public boolean follows​(Street s);
	
	public static Street defaultStreet(String street_id, Coordinate coor_1, Coordinate coor_2, Coordinate coor_3)
	{

		double side_1 = Point2D.distance(coor_1.getX(), coor_1.getY(), coor_2.getX(), coor_2.getY());

		double side_2 = Point2D.distance(coor_2.getX(), coor_2.getY(), coor_3.getX(), coor_3.getY());
		
		double side_3 = Point2D.distance(coor_1.getX(), coor_1.getY(), coor_3.getX(), coor_3.getY());


		double add_sides = Math.pow(side_1, 2) + Math.pow(side_2, 2) - Math.pow(side_3, 2);

		if(add_sides == 0)
		{
			return new Street_cl(street_id, coor_1, coor_2, coor_3);
		}
		else
		{
			return null;
		}

        

    }

	static Street defaultStreet(String street_id, Coordinate coor_1, Coordinate coor_2)
	{
		return new Street_cl(street_id, coor_1, coor_2);
	}
	
}
