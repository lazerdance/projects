
public class Pair<T> {
	private T first;
	private T second;
	
	Pair(T first, T second) {
		// TODO
		this.first = first;
		this.second = second;
	}

	public T getFirst() {
		// TODO
		return first;
	}

	public T getSecond() {
		// TODO
		return second;
	}

	public void setFirst(T dt) {
		// TODO
		first = dt;
	}

	public void setSecond(T dt) {
		// TODO
		second = dt;
	}

	public void swap() {
		// TODO
		T tmp = first;
		first = second;
		second = tmp;
	}

	@Override
	public String toString() {
		// TODO
		return first + ", " + second;
//		return String.format("<%s, %s>", this.getFirst(),this.getSecond());
	}
}

