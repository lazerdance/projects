/**
 * Represents a Cube
 * @author AlgoDat
 */
public class Cube extends Cuboid{

	/*
	 * Constructor without parameter
	 */
	public Cube() {
		//TODO
		super();
		this.length = 1.0;
		this.height = 1.0;
		this.width = 1.0;
	}
	/*
	 * Constructor with one parameter
	 * 
	 * @param length the side length of the cube
	 */
	public Cube(double length) {
		// TODO
		this.length = length;
		if(this.length < 0) {
			this.length = 1.0;
		}
		this.height = length;
		if(this.height < 0) {
			this.height = 1.0;
		}
		this.width = length;
		if(this.width < 0) {
			this.width = 1.0;
		}
		
	}

	public double getLength(){
		return this.length;
	}
	//TODO: ggf. weitere Methoden und member implementieren
	@Override
	public double calculateVolume() {
		return Math.pow(this.length, 3);
	}
	
	@Override
	public double calculateSurface() {
		return Math.pow(this.length, 2)*6;
	}
}

