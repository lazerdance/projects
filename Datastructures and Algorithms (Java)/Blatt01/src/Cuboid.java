/**
 * Represents a Cuboid
 * http://en.wikipedia.org/wiki/Cuboid
 * 
 * @author Amro Hendawi
 *
 */
public class Cuboid extends Body{

	double height, length, width;
	
	/*
	 * Constructor for a Cuboid object
	 */
	public Cuboid() {
		//TODO
		this.height = 1.0;
		this.length = 1.0;
		this.width = 1.0;
	}
	/*
	 * Constructor for a Cuboid object
	 * 
	 * @param h height
	 * @param l length
	 * @param w width
	 */
	public Cuboid(double h, double l, double w) {
		this.height = h;
		this.length = l;
		this.width = w;
		
		if (this.height < 0) {
			this.height = 1.0;
		}
		if (this.width < 0) {
			this.width = 1.0;
		}

		if (this.length < 0) {
			this.length = 1.0;
		}

		//TODO
	}

	public double getHeight(){
		return this.height;
	}
	public double getLength(){
		return this.length;
	}
	public double getWidth(){
		return this.width;
	}
	//TODO: ggf. weitere Methoden und member implementieren
	@Override
	public double calculateVolume() {
		return this.height*this.length*this.width;
	}
	
	@Override
	public double calculateSurface() {
		return 2*(this.height*this.length + this.length*this.width + this.width*this.height);
	}


}

