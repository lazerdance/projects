import java.util.ArrayList;

/**
 * A steel factory (an implementation of an enterprise)
 * represented by its name and list of employees.
 * @author Amro Hendawi
 *
 */
public class SteelFactory implements Enterprise {
  
    private static final String NULL = null;
	/** Name of the factory */
    private String name;
    /** List of workers */
    private ArrayList<Worker> workers = new ArrayList<Worker>();		// Speicher für ArrayList alloziieren 


    /**
     * Creates a new steel factory with a given name
     * @param name Name of the factory
     */
    public SteelFactory(String name) {
        this.name = name;
        if (this.name == NULL) {			
        throw new NotImplementedException("SteelFactory");//TODO
        }
    }

    @Override
    public void addWorker(String firstName, String lastName) {
    	
        this.workers.add(new Worker(firstName, lastName));
        
        if(this.workers.size() < 0) {
        throw new NotImplementedException("addWorker");//TODO
        }
    }

    @Override
    public String getName() {
      if (this.name == NULL) {
    	throw new NotImplementedException("getName");//TODO
      }
    	return this.name;
    }

    @Override
    public int compareTo(Enterprise o) {
      int result = 0;
      int myWorkers = this.getWorkerCount();
      int otherWorkers = o.getWorkerCount();
      if (otherWorkers > myWorkers) {			// Firma o > Firma m -> result = -1
    	  result = -1;
      }
      if (otherWorkers < myWorkers) {			// Firma o < Firma m -> result = 1
    	  result = 1;
      }
      return result;
      //throw new NotImplementedException("compareTo"); //TODO
      
    }

    @Override
    public int getWorkerCount() {
//    	int count = 0;
//    	try {
    	return this.workers.size();				// gibt die Zahl der Arbeiter zurück
//    	}
////    	catch(Nu-llPointerException e) {
////    		System.out.println("Null pointer exception occurred");
//    	}
//    	if(count<0) {
//    		throw new NotImplementedException("getWorkerCount");//TODO
//    	}

    }
}

