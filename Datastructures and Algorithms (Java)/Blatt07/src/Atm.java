import java.util.LinkedList;
import java.util.List;
import java.lang.RuntimeException;

/**
 * The class <code>Node</code> implements a node in a network.
 * 
 * @author AlgoDat team
 */
public class Atm {

	public LinkedList<Integer> denominations;

    /**
	 * Initializes the banknote denominations available to the ATM
	 *
	 * @param name
	 *            the drawn value in visualization
	 **/
	public Atm() {
		// initialize list of available denominations
		denominations = new LinkedList<Integer>();
		//Add denominations in a sorted order, highest value first:
		denominations.add(200);
		denominations.add(100);
		denominations.add(50);
		denominations.add(20);
		denominations.add(10);
		denominations.add(5);
	}

	/**
	 * Computes the the number of banknotes for each denomination
	 * 
	 * @param total
	 *            Amount of money requested
	 *            End point of this edge.
	 * @return List<int> 
	 *            Amount of banknotes for each denomination, 
	 *            as a list in the same order as the list denominations
	 *            Example: [0,1,0,0,0,0]: one 100EUR banknote
	 */
	public LinkedList<Integer> getDivision(int total) {
		LinkedList<Integer> division = new LinkedList<Integer>();
		//TODO: Implement this
		int rechnung;
		for (int d : denominations) {
			rechnung = total/d;
			total = total - (rechnung*d);
			division.add(rechnung);
		}
		return division; 
	}
}

