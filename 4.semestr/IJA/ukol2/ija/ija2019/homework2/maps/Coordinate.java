package ija.ija2019.homework2.maps;

public class Coordinate {
	
	
	private int x_coordinate;
	private int y_coordinate;
	
	public Coordinate(int x_coordinate, int y_coordinate)
	{
		this.x_coordinate = x_coordinate;
		this.y_coordinate = y_coordinate;
	}
	
    public int getX()
    {
        return x_coordinate;
    }
	
    public int getY()
    {
        return y_coordinate;
    }

    public void setX(int newValue) {
        x_coordinate = newValue;
    }

    public void setY(int newValue) {
        y_coordinate = newValue;
    }
    
    public static Coordinate create(int x, int y)
    {
    	if(x < 0 || y < 0)
    	{
    		return null;
    	}
    	else
    	{
    		return new Coordinate(x, y);
    	}
	}
	
	@Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof Coordinate)) return false;

        Coordinate that = (Coordinate) o;

        if (getX() != that.getX()) return false;
        return getY() == that.getY();
    }
	
    @Override
    public int hashCode() {
        final int prime = 31;
		int result = 1;
		result = prime * result + x_coordinate;
		result = prime * result + y_coordinate;
		return result;
    }
    
    public int diffX(Coordinate c) {
    	return Math.abs(this.x_coordinate - c.x_coordinate);
    }
    
    public int diffY(Coordinate c) {
    	return Math.abs(this.y_coordinate - c.y_coordinate);
    }
	

}
