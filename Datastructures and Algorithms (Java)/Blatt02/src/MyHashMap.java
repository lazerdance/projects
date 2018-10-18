/**
 * 
 * @author Amro Hendawi
 * @version 1.0  
 * 
 */
import java.lang.RuntimeException;
public class MyHashMap {
	
	/**
	 * Use this array to store the values
	 * DO NOT MODIFY OR REMOVE THIS Attribute. Our tests rely on it.
	 */
	private Student[] array;


	
	/**
	 * Tries inserting a Student into the hash map.
	 * Throws RunTimeException, if the student is already in the table or the table is full.
	 */
	public void add(Student s){
		//TODO: Your Code here.
		int h = hashFunction(s);				// h = Hashwert von Student
		if (contains(s) == true) {				// ist der Student schon angemeldet, gibt Fehlermeldung an.
			throw new RuntimeException("Fehler !!");
		}
		for (int i = 0 ;i < array.length ; i++) {

			// fügt der Student in der richtigen Hashadresse ein, falls er nicht auf der Liste steht			
			if(array[h] == null) {
				array[h] = s;
				return;
			}
			else{						// falls die Hashadresse des Student schon besetzt ist, dann fügt ihn in der nächsten freie Adresse ein
				h =(h+1) % array.length;
			}
		}
			throw new RuntimeException("Memory full !!");			// ist die Liste schon voll, gibt eine Fehlermeldung ab.
	}

	
	/**
	 * Try removing a Student from the hash table.
	 * You should use the same implementation for remove discussed in the tutorial.
	 * You should NOT use the lazy deletion strategy (adding a special flag key indicating a deleted key)
	 * See https://en.wikipedia.org/wiki/Linear_probing#Deletion for more details about the algorithm!
	 * Throw RunTimeException if the hash table contained the Student
	 */
	public void remove(Student s){
		if (!contains(s)) {					// ist der Student nicht auf der Liste steht, gibt eine Fehlermeldung ab
			throw new NullPointerException();
		}
		int hash = hashFunction(s);			// hash = Hashwert vom Student
			for (int j = hash; array[j] != null; j=(j+1)%array.length) {
				if(s.equals(array[j])) {		// Löscht den Student falls er in der richtigen Hashadresse getroffen wird
					array[j] = null;
					break;
				}								// falls nicht, dann guckt in der nächsten Adresse an (hash inkrementieren)
			}
		// rehashing Teil
		int kapa = array.length;				// Array Größe
		MyHashMap table = new MyHashMap(kapa);	// erstellt neue Temporary Hashtable mit der gleichen Größe
//		temp = (temp+1)%array.length;
//		while (array[temp] != null) {			// Studenten ordnen bis zur null Stelle
//		int temp = 0;
//		for(temp=hash; array[temp]!=null;) {
//			table[temp] = array[temp];			// Element zwischenspeichern
			for (Student st : array) {			//genau wie beim Resize werde die Elemente von Anfang an rehashed in einer neuen Array
				if (st != null) {
					table.add(st);				// elemente zur zwischenspeicher Array einfügen
				}
//			array[temp] = null;					// Student Löschen
//			add(table[temp]);					// Student nochmal addieren
//			temp=(temp+1)%array.length;
		}
		this.setArray(table.array);				// alte array = neue zwischenspeicher Array
	}	
	
	
	/**
	 * Returns true, if the hash table contains the given Student
	 */
	public boolean contains(Student s){
		int hash = hashFunction(s);
//		for(int i = hash; i != hash-1; i=(i+1)%array.length) {			// eine counter Schleife von der Hashadresse bis HashAdresse-1 läuft
		for (int i=0; i<array.length;i++) {
			if (s.equals(array[hash])) {									// falls der Student getroffen wird 
				return true;											// gibt True zurück
			}
			else if (s.equals(null)) {
				break;
			}
				hash = (hash+1)%array.length;
		}
		return false;				// gibt False zurück falls der Student auf dem ganzen Array überhaupt nicht gefunden wurde
	}

	/**
	 * @param h Hashvalue to search for
	 * @return Number of Student in the hash table that have the hashvalue h
	 */
	public int getNumberStudentsWithHashvalue(int h){
		int n = 0;											// counter = 0
		int temp = h;										// h in temp zwischenspeichern
		for(; h!=h-1; temp=(temp+1) % array.length) {		// sucht im ganzen Array nach Studenten 
		if (array[temp] == null) {							// falls keine Studenten mit h Hashwert dann gibt 0 zurück und stop
			break;
		}
		if (h == hashFunction(array[temp])) {				// für jeden Student mit dem h Hashwert n++
				n++;
			}
		}
		return n;										// gibt die Zahl der Studenten mit h Hashwert an
	}
	
	/**
	 * Doubles the size of the hash table. Recomputes the position of all elements using the
	 * new function.
	 */
	public void resize(){
		//TODO: Your Code here.
		int capacity = 2*array.length;				// Array Größe verdoppeln
		MyHashMap table = new MyHashMap(capacity);	// erstellt neue Temporary Hashtable mit der neuen Größe
		for (Student st : array) {					// alle Studenten an der neuen Hashtabelle einfügen
			if (st != null){
				table.add(st);
			}
		}
		this.setArray(table.array);				// alte Hashtabelle array = neue Hashtabelle
	}

	/**
	 * This method return the array in which the strings are stored.
	 * DO NOT MODIFY OR REMOVE THIS METHOD. Our tests rely on it.
	 */
	public Student[] getArray(){
		return array;
	}
	
	/**
	 * DO NOT MODIFY OR REMOVE THIS METHOD. Our tests rely on it.
	 */
	public void setArray(Student[] array){
		this.array = array;
	}

	/**
	 * Runs the hash function for Student s (dependent on the size of the hash table)
	 * DO NOT MODIFY OR REMOVE THIS METHOD. Our tests rely on it.
	 * @param s Student
	 * @return the hash value for a student. (The position where it would be saved given no collisions)
	 */
	public int hashFunction(Student s){
		int hashvalue = Math.abs(s.hashCode()) % array.length;
		return hashvalue;
	}
	
	/**
	 * Constructor to initialize the hash with a given capacity
	 * DO NOT MODIFY OR REMOVE THIS METHOD. Our tests rely on it.
	 */
	public MyHashMap(int capacity){
		array = new Student[capacity];
	}

}

