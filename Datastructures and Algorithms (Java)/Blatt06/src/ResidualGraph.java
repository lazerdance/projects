import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Stack;

/**
 * @author Amro Hendawi
 * 
 */
public class ResidualGraph extends DiGraph {


	// -- constructor --
	public ResidualGraph() {
	}
	
	/**
	 * Finds an augmenting path from start to end in the graph A path is
	 * augmenting if all it's edges have residual capacity > 0 (You can choose
	 * from several algorithms to find a path)
	 * 
	 * @param startNodeId
	 *            the id of the start node from where we should start the search
	 * @param endNodeId
	 *            the id of the end node which we want to reach
	 * 
	 * @return the path from start to end or an empty list if there is none
	 */
	public LinkedList<Node> findAugmentingPath(int startNodeId, int endNodeId) {
		// TODO: Your implementation here
		LinkedList<Node> q = new LinkedList<Node>();				// queue Liste
		LinkedList<Node> visitedList = new LinkedList<Node>();			// bfs Resultat
		ArrayList<Node> nachbarn = new ArrayList<Node>();				// nachbarknoten zwischenspeicher
		LinkedList<Node> leer = new LinkedList<Node>();					//einfach leere Liste
		
		LinkedList<Node> vor = new LinkedList<Node>();					// path vorwaerts
		LinkedList<Node> ruck = new LinkedList<Node>();					// path ruckwaerts
		
		Node startNode = nodes.get(startNodeId);						// startNode als Knoten	
		Node endNodee = nodes.get(endNodeId);							// endNode als Knoten

		/*
		 * checkt ob der eingegebene Knoten nicht gultig ist
		 */
		if (!getNodes().contains(startNode) || startNode == null || !getNodes().contains(endNodee)) {
			throw new RuntimeException("Error");
		}
		
		if (endNodee.equals(startNode))				//gibt es in dem Graph nur einen Knoten dann gibt nur diesen Knoten zurueck
			return visitedList;
		for (Node g : getNodes()) {					// predecessor initialisieren
			g.predecessor = null;
		}
		
		startNode.status = Node.GRAY;					// gray == besucht
		q.addLast(startNode);
		visitedList.add(startNode);
		Node t = null;
		
		
		/*
		 *  entwickelte BFS die nur die Knoten mit echten Edges einfuegt
		 */
		while(!q.isEmpty()) {
			Node n = q.removeFirst();
			n.status=Node.GRAY;
			nachbarn = n.getAdjacentNodes();
			for (Node v : nachbarn) {
				if( n.getEdgeTo(v) != null && v.status == Node.WHITE && n.getEdgeTo(v).weight != 0.0 ) {				// edge.weight == capacity 
					v.status = Node.GRAY;
					v.predecessor = n;
					q.addLast(v);					// addLast == add in diesem Fall
					visitedList.addLast(v);			// in der besuchten Liste einfuegen
				}
			}
		}
		
		/*
		 *  eine Schleife die checkt ob der Graph zusammenhaengend ist und aktualisiert den Path hin und zurueck
		 */
		for (Node nn : visitedList) 
			if (nn.id == endNodeId) {						// kann auch mit equals berechnet werden
				if(nn == null) return leer;
				t = nn;
			}
			while(t != null) {
				vor.add(t);
				t = t.predecessor;
			}
			for(int j=vor.size(); j>0; j--) {
					ruck.add(vor.get(j-1));
			}
			
			/*
			 * einfach alles zuruecksetzen 
			 */
			for(Node w : getNodes()) {
				w.status = Node.WHITE;
				w.predecessor = null;
			}
		return ruck;
	}

	/**
	 * Finds the minimal residual capacity over the given path
	 * 
	 * @return the minimal capacity
	 */
	public double findMinCapacity(LinkedList<Node> path) {
		// TODO: Your implementation here

		if (path.isEmpty()) {						
			return 0.0;
		}
		double minimum = path.get(0).getWeight(path.get(1));			// Anfangsminimum oder man kann auch infinity setzen
		
		double weight = 0.0;											// zwischenspeicher fuer den weight
		for(int y=1; y<path.size()-1; y++) {							// minCapacity finden
			weight = path.get(y).getWeight(path.get(y+1));
			if(weight <minimum) {
				minimum = weight;
			}
		}

		return minimum;
	}

	/**
	 * Update capacity on given path, to be executed on residual graph
	 */
	public void updateResidualCapacity(double minCapacity, LinkedList<Node> path) {
		// TODO: Your implementation here

		double weight1 = 0.0;
		double weight2 = 0.0;
		
		// weight aktualisieren hin und zurueck
		for(int y=0; y<path.size()-1;y++) {
			path.get(y).getEdgeTo(path.get(y+1)).weight = path.get(y).getEdgeTo(path.get(y+1)).weight - minCapacity;
			path.get(y+1).getEdgeTo(path.get(y)).weight = path.get(y+1).getEdgeTo(path.get(y)).weight + minCapacity;
		}
	}

}