import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import java.util.Arrays;

import org.junit.Before;
import org.junit.Test;


public class ArrayCheckTest {
	// NOTTODO DIESE ZEILE BITTE NICHT VERAENDERN!!
	// TODO Fuehrt eure Tests auf diesem ArrayCheck-Objekt aus!
	// Ansonsten werden unsere Tests fehlschlagen
	public ArrayCheck arrayCheckTestObject = new ArrayCheck();

	@Test(timeout = 1000)
	public void testAllDivisibleBy() {

		// TODO Schreibt verschiedene Testfaelle, die unterschiedliche Arrays
		// anlegen und an arrayCheck.testAllDivisibleBy uebergeben.
		// Benutzt nur diese Methode.
		// Testet auch randfaelle wie z.B. leere arrays.
		// Bsp: assertTrue("mein test 1", arrayCheckTestObject.allDivisibleBy(testArray, 7));
		ArrayList<Integer> testArray = new ArrayList<Integer>();
		testArray.add(10);
		testArray.add(15);
		assertTrue("mein test 1", arrayCheckTestObject.allDivisibleBy(testArray, 7));
	}

	@Test(timeout = 1000)
	public void testIsAnagram() {
		// TODO Schreibt verschiedene Testfaelle fuer isAnagram in diese Methode
		// Bsp: assertTrue("mein test 2", arrayCheckTestObject.isAnagram(testArray1, testArray2));
		ArrayList<Character> testArray1 = new ArrayList<Character>();
		ArrayList<Character> testArray2 = new ArrayList<Character>();

		testArray1.add('A');
		testArray1.add('C');
		testArray1.add('D');
		
		testArray2.add('D');
		testArray2.add('A');
		testArray2.add('C');
		
		
		assertTrue("mein test 2", arrayCheckTestObject.isAnagram(testArray1, testArray2));
		
		// TODO Schreibt verschiedene Testfaelle fuer isAnagram}
	}
}

