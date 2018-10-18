import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * A class implementing a Union-Find-data structure with representatives.
 * 
 * @author AlgoDat-Team
 */
public class UnionFindSet<T>{

	//You can use this map to store the representant for each element of the Union find structure
    private HashMap<T,T> element2representative;

	public UnionFindSet() {
		element2representative = new HashMap<>();
	}

	/**
	 * Takes a collection of n elements and adds 
	 * n disjoint partitions each with one element in it.
	 * 
	 * @param set
	 *            The set to be partitioned.
	 */
	public void add(List<T> elements) {
		// TODO Homework 2.1
		for (T elem : elements) {
			add (elem);
		}
//		elements.addAll(element2representative.keySet());
	}

	/**
	 * Creates one disjoint partition with the element in it 
	 * and adds it to the Union-Find data structure
	 * 
	 * @param element
	 *            The element to put in the partition.
	 */
	public void add(T element) {
		// TODO Homework 2.1
//		if (! element2representative.containsKey(element) || element == null ) {
//			throw new RuntimeException("error");
//		}
//		HashMap<T> l = element2representative.get(element, ); 
		if (!(element2representative.containsKey(element)) && element != null) {
//			for (T t : element2representative.keySet()) {	
//			List<String> list = new List<String>()
			element2representative.put(element, element);
//			list = new ArrayList<>();
//			element2representative.put(T, element);
//			}
//			element2representative.put(element, l = new ArrayList<T>());
//			add.insert(element);
//			this.elements.add(element);
			
		}
		
	}

	/**
	 * Retrieves the partition which contains the wanted element.
	 * 
	 * A partition is identified by a single, representative element.
	 * This function returns the representative of the partition
	 * that contains x. 
	 * 
	 * @param x
	 *            The element whose partition we want to know
	 * @return 
	 *            The representative element of the partition
	 */
	public T getRepresentative(T x) {
		// TODO Homework 2.1
		T rep = element2representative.get(x);
		return rep;
	}

	/**
	 * Joins two partitions. First it retrieves the partitions containing the
	 * given elements. Removes one of the joined partitions from
	 * <code>partitions</code>.
	 * 
	 * @param x
	 *            One element whose partition is to be joined.
	 * @param y
	 *            The other element whose partition is to be joined.
	 */
	public void union(T x, T y) {
		// TODO Homework 2.1
		T rep_x, rep_y;
		rep_x = element2representative.get(x);
		rep_y = element2representative.get(y);
		
		if (rep_x == null || rep_y == null || rep_x == rep_y) {
			return;
		}
		for (T e : element2representative.keySet()) {
			if (!element2representative.get(e).equals(rep_y)) {
			continue;	
			}
			element2representative.put(e, rep_x);
		}
	}
}

