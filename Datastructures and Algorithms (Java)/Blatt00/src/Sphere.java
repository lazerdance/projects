class Sphere {
	
	Point center;
	double radius;
	
	Sphere(int x, int y, int z, double r){
		// TODO
		center = new Point(x,y,z);
		radius = r;
		
	}
	
	Sphere(Point c, double r){
		// TODO
		center = c;
		radius = r;
	}
	
	int getX(){
		return center.getX(); // TODO
	}

	int getY(){
		return center.getY(); // TODO
	}

	int getZ(){
		return center.getZ(); // TODO
	}


	double getRadius(){
		return radius; // TODO
	}

	double calculateDiameter(){
		return radius*2; // TODO
	}	
	
	double calculateSurfaceArea(){
		return Math.PI*Math.pow(radius, 2); // TODO
	}
	
	double calculateVolume(){
		return (4.0/3.0)*Math.PI*Math.pow(radius, 3); // TODO
	}

}

