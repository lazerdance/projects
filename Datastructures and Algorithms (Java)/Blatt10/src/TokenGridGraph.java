import java.io.IOException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.Arrays;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.PriorityQueue;
import java.util.Queue;
import java.util.Random;

import java.util.LinkedList;
import java.util.List;

/**
 * A data structure to store a graph consisting of a set of unlabeled nodes and
 * a set of directed, weighted edges connecting the nodes.
 * 
 * The interface provides methods to add nodes and edges. Both methods can be
 * called at all time and the data structure should not assume a limitation on
 * the number of nodes or edges.
 * 
 * 
 * Furthermore, there are methods to access a single edge or to check whether
 * two nodes are connected as well as methods to access all nodes connected with
 * a specific node and thus all outgoing edges.
 * 
 * @author Arne Sieverling
 */
public class TokenGridGraph extends GridGraph {

	/**
	 * All tokens (green pixels) the tour must contain
	 */
	public ArrayList<CellNode> tokens;

	RGBColor green = new RGBColor(0, 255, 0);

	public TokenGridGraph() {
		super();
		tokens = new ArrayList<CellNode>();
	}

	/**
	 * Takes a Picture object to construct the graph from, pixel colors have the following meaning:
	 * 
	 * white: pixel is a free space
	 * green: pixel is a token/waypoint
	 * everything else: obstacle  
	 */
	public TokenGridGraph(Picture p) {

		super(p); //construct a graph containing all white pixels
		
		tokens = new ArrayList<CellNode>();

		// Add all green pixels to the graph:
		RGBColor[][] pixels = p.getImageMatrix();
		for (int i = 0; i < columns; i++) {
			for (int j = 0; j < rows; j++) {
				RGBColor pixel = pixels[i][j];
				if (pixel.getGreen() == 255 && pixel.getBlue() == 0 && pixel.getRed() == 0) {
					// Add green node to graph and to tokens
					tokens.add((CellNode) this.addNode(j, i));
				}
			}
		}

		// connect all green nodes (tokens) to their neighbours if they exist
		for (CellNode n : tokens) {
			int i = n.getRow();
			int j = n.getColumn();

			// Connect every node to its 4 direct neighbors:
			CellNode neighbor;
			neighbor = getCellNode(i - 1, j);
			if (neighbor != null) {
				n.addEdge(neighbor, 1);
				neighbor.addEdge(n,1);
			}
			neighbor = getCellNode(i + 1, j);
			if (neighbor != null) {
				n.addEdge(neighbor, 1);
				neighbor.addEdge(n,1);
			}
			neighbor = getCellNode(i, j - 1);
			if (neighbor != null) {
				n.addEdge(neighbor, 1);
				neighbor.addEdge(n,1);
			}
			neighbor = getCellNode(i, j + 1);
			if (neighbor != null) {
				n.addEdge(neighbor, 1);
				neighbor.addEdge(n,1);
			}
		}

	}

	Picture toPicture(List<Node> path) {
		Picture out = super.toPicture(path);
		for (CellNode t : tokens)
			out.setPixel(t.getColumn(), t.getRow(), green);
		return out;
	}

	ArrayList<Node> computeTour(CellNode startNode) {
		
		ArrayList<Node> completePath = new ArrayList<Node>();

		Node tokenFirst = startNode;
		CellNode tokenBefore = startNode;
		Node tokenLast = startNode;
		
		//Iterate over the tokens in given order
		for(CellNode token : tokens)
		{
			if(tokenBefore != null)
			{
				//Find shortest path from this to the next token
				List<Node> pathToNextToken = this.getShortestPathAStar(tokenBefore, token);
				
				//Now append all the nodes to the complete path - skip the First token two avoid adding double nodes
				completePath.addAll(pathToNextToken.subList(1, pathToNextToken.size()));
			} else {
				completePath.add(token);				
				tokenFirst = token;				
			}
			tokenBefore = token;
			tokenLast = token;
		}
		
		//Find shortest path back to the first token
		List<Node> pathBackToFirstToken = this.getShortestPathAStar(tokenLast,tokenFirst);			
		completePath.addAll(pathBackToFirstToken.subList(1, pathBackToFirstToken.size()-1));
		System.out.println(completePath);
		resetState();//clean up 		
		return completePath;
	}

//	@Override
//	public List<Node> getNodes() {
//		return new ArrayList<Node>(nodes.values());
//	}
//	
//	@Override
//	public boolean isConnected(int startNodeID, int endNodeID) {
//		return isConnected(nodes.get(startNodeID), nodes.get(endNodeID));
//	}
//	
//	
//public Graph toMinSpanTree() {
//	// TODO Homework 2.2
//	resetState();
//	
//	HashMap<Integer, Node> mst = new HashMap<Integer, Node>();						// MinSpanbaum erstellen 
//	UnionFindSet<Node> un = new UnionFindSet<Node>();			// Union Set fuer die Knoten erstellen und Speicher alloziieren
//	PriorityQueue<Edge> edgeList = new PriorityQueue<Edge>();	// Queue fuer die Edges erstellen und sortieren nach Gewicht
//	List<Node> nodesList = getNodes();							// alle Knoten zu einer Liste einfuegen
//	
//	for (Node nod : nodesList) {								// ist einer der Knoten leer -> Fehlermeldung
//		if (nod == null) {
//			throw new RuntimeException("Node is empty");
//		}
//		else {
//			un.add(nod);										// ansonsten zu Union Set einfuegen
	//			mst.put(nodes.size(),new Node(nodes.size()));										// und zu unserem Baum einfuegen
//		}
//	}
//		
//	for (Node n : nodesList) {									// fuegt alle Edges der Knoten zu Edges List
//		for(Edge ed : n.getIncidentEdges()) {
//				edgeList.add(ed);
//		}
//	}
//	// eine Schleife die checkt jedes Edge ob es connected und fuegt es mit dem Gewicht zu mst ein
//	while(!edgeList.isEmpty()) { 
//		Edge e = edgeList.poll();
//		if (un.getRepresentative(e.startnode) != un.getRepresentative(e.endnode)) {
//				un.union(e.endnode,e.endnode);
//				if (!mst.isConnected(e.endnode.id, e.endnode.id)){
////					throw new RuntimeException("error cohesion");
//					return null;
//				}
//				int start_id = e.startnode.id;
//				int end_id = e.startnode.id;
//				e.startnode.status = Node.GRAY;
//				e.endnode.status = Node.GRAY;
//				mst.addEdge(start_id,end_id,e.weight);				// fuegt die Edges zu ihrem passenden Nodes ein
//			}
//		}

	/*
	 * testDisconnectedGraph Teil
	 */

//	List<Node> visitedList = new LinkedList<Node>();				// Speicher fuer visitedList alloziieren			
//	for (int i =0; i<getNodes().size(); i++){						// nimmt den erste Knoten, der nicht null ist, und aktiviert bfs mit dem, um allen zusammenhaengenden Knoten als WHITE zu markieren
//		if (mst.getNodes().get(i) != null ) {
//		visitedList = breadthFirstSearch(mst.getNodes().get(i));		// suchen nach alle Wege von einem beliebigen Knoten != null und markiert alle besuchte Knoten als besucht
//		}
//		if (visitedList.size()>0){									// stoppt die Schleife, sobald visitedList aufgeladen worden ist
//			break;
//		}
//	}
//
//	for (Node s : mst.getNodes()) {			// ist einer der Knoten nicht visited -> ist der ganze Graph nicht zusammenhaengend
//		if (s.status == Node.WHITE) {
//		return null;
//		}
//	}
//	return mst;
//}


/*
 * BFS Methode genau implementiert wie im Blatt3 (einfach kopiert)
 */
public List<Node> breadthFirstSearch(Node startNode){
	LinkedList<Node> visitedList = new LinkedList<Node>();
	if (startNode == null) {			// ist die Graph leer, return null
		return null;
	}
	if (nodes.values().contains(startNode) && startNode != null) {
		Queue<Node> queue = new LinkedList<Node>();				// Speicher fuer Queue Liste alloziieren 
		startNode.status = Node.BLACK;							// startNode wird als visited markiert
		queue.add(startNode);									// StartNode erster Element in der Queue
		visitedList.add(startNode);								// StartNode erster Element in der VisitedList
		while(!queue.isEmpty()) {								// entleert die Queue Liste
				Node v = queue.poll();						// aber speichert erst die Knoten in temporary Node
			for(Node n : v.getAdjacentNodes()) {				// suchen nach allen Adjazenten
				if(getColorOfNode(n) == Node.WHITE) {			// die noch nicht besucht wurden
					n.status = Node.GRAY;						// als besucht markieren
					n.predecessor = v;
					queue.add(n);								// in der Queue einfuegen
					visitedList.add(n);							// in der visitedList einfuegen
				}
			}
		}
	}
	resetState();
	// TODO
	return (visitedList);							// return die visited Liste
}

	
	public void addEdge(Node startnode, Node endnode, int weight) {
		if (startnode != null && endnode != null) {

			// only add edges between nodes which are already part of this graph
			if (nodes.values().contains(startnode)
					&& nodes.values().contains(endnode)) {
				startnode.addEdge(endnode, weight);
				endnode.addEdge(startnode, weight);
			}
		}
	}
	
	@Override
	public void addEdge(int startnode, int endnode, int weight) {
		addEdge(nodes.get(startnode), nodes.get(endnode), weight);
	}

	public boolean isConnected(Node startnode, Node endnode) {
		if (startnode != null && endnode != null) {
			if (nodes.values().contains(startnode)
					&& nodes.values().contains(endnode)) {
				return startnode.hasEdgeTo(endnode);
			}
		}
		return false;
	}



	
}
