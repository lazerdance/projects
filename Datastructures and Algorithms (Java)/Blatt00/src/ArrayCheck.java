import java.util.ArrayList;
import java.util.Collections;

/**
 * This class implements two methods. These check an array on a few
 * characteristics.
 * 
 * @author AlgoDat-Tutoren
 * 
 */
public class ArrayCheck {

	/**
	 * Tests all elements of the given array, if they are divisible by the given
	 * divisor.
	 * 
	 * @param arr
	 *            array to be tested
	 * @param divisor
	 *            number by which all elements of the given array should be
	 *            divisible
	 * @return true if all elements are divisible by divisor
	 */
	public boolean allDivisibleBy(ArrayList<Integer> arr, int divisor) {
		boolean divisible = true;
		for (int i=0; i<arr.size(); i++){
			if (arr.get(i) != 0 ){
				if (divisor == 0) {
				divisible = false;
				}
				else if (arr.get(i) % divisor != 0){
					divisible = false;
				}
			}
		}
		return divisible;
	}
	/**
	 * Tests if the given two arrays are anagrams of each other
	 * 
	 * @param arr1
	 *            first array to be compared
	 * @param arr2
	 *            second array to be compared
	 * @return true if the two arrays are an anagram
	 */
    public boolean isAnagram(ArrayList<Character> arr1, ArrayList<Character> arr2) {
        // TODO
    	
    	if (arr1.size() != arr2.size()) {
    		return false;
    	}
    	Collections.sort(arr1);
    	Collections.sort(arr2);
        for (int i = 0; i < arr1.size();  i++) {
            if (arr1.get(i) != arr2.get(i))
                return false;
    	}
       	return true;
  }	
}