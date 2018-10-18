import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Vector;


/**
 * @author AlgoDat Team
 * 
 */
public class Network implements INetwork {
	// -- attributes --
	private HashMap<Integer, Node> nodes;

	public INetwork residualGraph;

	// -- constructor --
	public Network() {
		nodes = new HashMap<Integer, Node>();
		residualGraph = null;
	}


	// -- node functions --
	/**
	 * returns the nodes
	 */
	@Override
	public LinkedList<Node> getNodes() {
		return new LinkedList<Node>(nodes.values()); //nodes.values() only returns a Collection, not a List, so we need to create it
	}


	@Override
	public Node addNode() {
		int newId = nodes.size();
		Node newNode = new Node(newId);
		nodes.put(newId, newNode);
		return newNode;
	}

	// -- edge functions --
	public void addEdge(Node startNode, Node endNode, int capacity) {
		if (!(testEdgeNodes(startNode, endNode)))
			inputError();
		startNode.addEdge(startNode, endNode, capacity);
	}

	public void addEdge(int startnode, int endnode, int capacity) {
		addEdge(nodes.get(startnode), nodes.get(endnode), capacity);
	}


	public void updateEdge(int start, int end, int flow, int capacity) {
		Edge tmp = this.getEdge(start,end);
		tmp.currentFlow = flow;
		tmp.capacity = capacity;
	}

	/**
	 * Returns graph edge specified by source and destination indices.
	 * 
	 * @param startNodeInd
	 *            index of start node
	 * @param targetNode
	 *            index of target node
	 */
	public Edge getEdge(int startNodeInd, int targetNodeint) {
		Node n = nodes.get(startNodeInd);
		for (Edge e : n.getIncidentEdges())
			if (e.endNode.id == targetNodeint)
				return e;
		return null;
	}

	public boolean testEdgeNodes(Node startNode, Node endNode) {
		return (startNode != null) && (endNode != null)
				&& nodes.values().contains(startNode)
				&& nodes.values().contains(startNode);
	}

	// -- state reset functions --
	/**
	 * resets the state of all nodes and edges to white
	 */
	public void clearMarksAll() {
		clearMarksNodes();
		for (Node currentNode : nodes.values())
			for (Edge currentEdge : currentNode.getIncidentEdges())
				currentEdge.status = Edge.WHITE;
	}

	/**
	 * help function to reset the state of all nodes to white
	 */
	public void clearMarksNodes() {
		for (Node n : nodes.values())
			n.status = Node.WHITE;
	}

	public boolean isAdjacent(Node startNode, Node endNode) {
		if (!(testEdgeNodes(startNode, endNode)))
			inputError();
		return startNode.hasEdgeTo(endNode);
	}

	/**
	 * Searches for sources in the graph
	 * 
	 * @return All sources found in the graph
	 */
	public Node findSource() {
		LinkedList<Node> sources = new LinkedList<Node>();
		boolean isSource = true;
		// source <-> no incoming edges
		for (Node n : nodes.values()) {
			isSource = true;
			for (Node m : nodes.values()) {
				if (!m.equals(n) && isAdjacent(m, n)) {
					isSource = false;
					break;
				}
			}
			if (isSource)
				sources.add(n);
		}
		// error handling
		testSingle(sources);
		return sources.getFirst();
	}

	/**
	 * Searches the graph for sinks.
	 * 
	 * @return All sinks found in the graph
	 */
	public Node findSink() {
		LinkedList<Node> sinks = new LinkedList<Node>();
		// sink <-> no incoming edges
		for (Node n : nodes.values()) {
			if (n.getIncidentEdges().isEmpty())
				sinks.add(n);
		}
		// error handling
		testSingle(sinks);
		return sinks.getFirst();
	}

	public void testSingle(LinkedList<Node> nodes) {
		if (nodes.size() == 0 || nodes.size() > 1)
			inputError();
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
		LinkedList<Node> q = new LinkedList<Node>();				// queue Liste
		LinkedList<Node> visitedList = new LinkedList<Node>();			// bfs Resultat
		ArrayList<Node> nachbarn = new ArrayList<Node>();				// nachbarknoten zwischenspeicher
		LinkedList<Node> leer = new LinkedList<Node>();					//einfach leere Liste
		
		LinkedList<Node> vor = new LinkedList<Node>();					// path vorwaerts
		LinkedList<Node> ruck = new LinkedList<Node>();					// path ruckwaerts
		
		Node startNode = nodes.get(startNodeId);						// startNode als Knoten	
		Node endNodee = nodes.get(endNodeId);							// endNode als Knoten

		int parent[] = new int[getNodes().size()];
		
		for(int x=0; x<getNodes().size(); x++) {
			parent[x] = -1;
		}
		/*
		 * checkt ob der eingegebene Knoten nicht gultig ist
		 */
		if (!getNodes().contains(startNode) || startNode == null || !getNodes().contains(endNodee)) {
			throw new RuntimeException("Error");
		}
		
		if (endNodee.equals(startNode))				//gibt es in dem Graph nur einen Knoten dann gibt nur diesen Knoten zurueck
			return visitedList;
		
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
			nachbarn = n.getSuccessorNodes();
			for (Node v : nachbarn) {
				if( n.getEdgeTo(v) != null && v.status == Node.WHITE && n.getEdgeTo(v).capacity != 0.0 ) {				// edge.weight == capacity 
					v.status = Node.GRAY;
					parent[v.id] = n.id;
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
				t.id = parent[t.id];
			}
			for(int j=vor.size(); j>0; j--) {
					ruck.add(vor.get(j-1));
			}
			
			/*
			 * einfach alles zuruecksetzen 
			 */
			clearMarksNodes();
			return ruck;
	}

	// -- code to complete --

	/**
	 * Computes the maximum flow over the network with the Ford-Fulkerson
	 * Algorithm
	 * 
	 * @returns Value of maximal flow
	 */
	public int edmondsKarp() {
		int totalFlow = 0;
		// For testing purposes, you could use here your Edmonds-Karp implementation from Blatt 06
		Node source = findSource();
		Node sink = findSink();		
		// You can use this method to create a residual network
		residualGraph = initializeResidualGraph();
		// TODO: Your implementation here
		LinkedList<Node> path = new LinkedList<Node>();
						
		path = residualGraph.findAugmentingPath(source.id, sink.id);
			while(path.size()>0) {
//				path.poll();
				int minCapacity = residualGraph.findMinCapacity(path);
				residualGraph.updateResidualCapacity(minCapacity, path);
				totalFlow += minCapacity;
								
				path = residualGraph.findAugmentingPath(source.id, sink.id);

			}
		
		return totalFlow;
	}


	/**
	 * Finds min cut in this graph
	 * 
	 * 
	 * @return a list of edges which have to be cut for 
	 * severing the flow between source and sink node
	 */
	
	
	public List<Edge> findMinCut() {
		// TODO: Your implementation here
		List<Edge> cutEdges = new LinkedList<Edge>();
		List<Node> visitedList = new LinkedList<Node>();
		LinkedList<Node> queue = new LinkedList<Node>();			// in der Queue fuegen wir die Knoten, von denen wir nach Edges mit 0 Residual capacity suchen

		Node source = findSource();							// wir suchen vom Source Knoten nach dem Min-Cut
		visitedList = findReachableNodes(source);			// fuegt alle erreichbare Knoten vom Source die Edges mit Residual Capacity haben ein
		queue.addLast(source);
		/*
		 * eine Schleife die fuegt alle Edges vom Source mit 0 Residual Capacity zum cutEdges 
		 */
		while (!queue.isEmpty()) {
			Node start = queue.poll();
			for(Node nachbar : start.getSuccessorNodes()) {
				if (start.getEdgeTo(nachbar).capacity == start.getEdgeTo(nachbar).currentFlow) {
					cutEdges.add(start.getEdgeTo(nachbar));
				}
				else {									// ist der Edge Residual capacity != 0 -> suchen von diesem Knoten nach 0 Residual Capacity
					queue.addLast(nachbar);
//				}
//					for (Node nach : nachbar.getSuccessorNodes()) {
//						if (nachbar.getEdgeTo(nach).capacity == nachbar.getEdgeTo(nach).currentFlow) {
//							cutEdges.add(nachbar.getEdgeTo(nach));
//						}						
					}
				}
			}
		return cutEdges;
	}


	public LinkedList<Node> findReachableNodes(int startNode) {
		return findReachableNodes(nodes.get(startNode));
	}

	/** Does a Breadth-First-Search to find the set of nodes which 
	 * are connected to the startNode with nonzero flow capacity
	 * 
	 * @argument startNode: start/source node
	 * 
	 * @return List of nodes that are reachable from source 
	 */	
	public LinkedList<Node> findReachableNodes(Node startNode) {
		LinkedList<Node> nodeList = new LinkedList<Node>();					// all reachable nodes werden in dieser Liste gespeichert
		LinkedList<Node> q = new LinkedList<Node>();						// Queue liste wie immer
		int parent[] = new int[getNodes().size()];							// Alternativ zu Predecessor
		// TODO: Your implementation here
		if (startNode == null || !nodes.values().contains(startNode)) {
		throw new RuntimeException("error");
		}
		
		q.addLast(startNode);
		startNode.status = Node.BLACK;
		parent[startNode.id] = startNode.id;
		/*
		 * eine Schleife die Knoten mit Residual Capacity einfuegt und setzt das Predecessor jeweils
		 */
		while(!q.isEmpty()) {
			Node n = q.removeFirst();
			n.status = Node.GRAY;
			nodeList.addLast(n);
			
			for(Node w : n.getSuccessorNodes()) {
				Edge edge = getEdge(n.id, w.id);
				if (w.status == Node.WHITE && edge.getCapacity()-edge.currentFlow > 0) {
					w.status = Node.BLACK;
					parent[w.id] = n.id;
					q.addLast(w);
				}
			}
		}
		System.out.println(nodeList);
		clearMarksNodes();					// Farben zuruecksetzen
		return nodeList;
	}	
	
	/**
	 * Builds a residual graph from a flow graph
	 * 
	 * @return the residual graph of this flow graph
	 */
	public INetwork initializeResidualGraph() {

		Network residualGraph = new Network();
		Edge reverseEdge;
		// adding nodes
		for (int i = 0; i < nodes.values().size(); i++)
			residualGraph.addNode();
		// adding edges
		for (Node n : nodes.values()) {
			for (Edge e : n.getIncidentEdges()) {
				// Add forward edges with same capacity
				residualGraph.addEdge(n.id, e.endNode.id, e.capacity);
				// Add backwards edges
				reverseEdge = getEdge(e.endNode.id, n.id);
				if (reverseEdge != null)
					residualGraph.addEdge(e.endNode.id, n.id, reverseEdge.capacity);
				else
					residualGraph.addEdge(e.endNode.id, n.id, 0);
			}
		}
		return residualGraph;
	}

	
	
	/**
	 * Finds the minimal residual capacity over the given path
	 * 
	 * @return the minimal capacity
	 */
	public int findMinCapacity(LinkedList<Node> path) {
		int minCapacity = Integer.MAX_VALUE;
 		// For testing purposes, you could use here your Edmonds-Karp implementation from Blatt 06
		if (path.isEmpty()) {						
			return 0;
		}
		
		int weight = 0;											// zwischenspeicher fuer den weight
		for(int y=1; y<path.size()-1; y++) {							// minCapacity finden
			weight = path.get(y).getEdgeTo((path.get(y+1))).capacity;
			if(weight < minCapacity) {
				minCapacity = weight;
			}
		}

		return minCapacity;
	}

	/**
	 * Update capacity on given path, to be executed on residual graph
	 */
	public void updateResidualCapacity(int minCapacity, LinkedList<Node> path) {
		// For testing purposes, you could use here your Edmonds-Karp implementation from Blatt 06
		for(int y=0; y<path.size()-1;y++) {
			path.get(y).getEdgeTo(path.get(y+1)).capacity = path.get(y).getEdgeTo(path.get(y+1)).capacity - minCapacity;
			path.get(y+1).getEdgeTo(path.get(y)).capacity = path.get(y+1).getEdgeTo(path.get(y)).capacity + minCapacity;
		}
	}


	/**
	 * Calculates the current flow in this graph.
	 * 
	 * @param source
	 *            the source of the flow
	 * 
	 * @return the value of the flow
	 */
	public int getFlow(Node source) {
		int flow = 0;
		for (Edge e : source.getIncidentEdges()) {
			if (e.currentFlow > 0)
				flow += e.currentFlow;
		}
		return flow;
	}

	public LinkedList<Node> breadthFirstSearch(int startNode) {
		return breadthFirstSearch(nodes.get(startNode));
	}

	public LinkedList<Node> breadthFirstSearch(Node startNode) {
		LinkedList<Node> nodeList = null;
		clearMarksNodes();

		if (startNode == null || !nodes.values().contains(startNode)) {
			nodeList = new LinkedList<Node>();
		} else {
			nodeList = new LinkedList<Node>();
			LinkedList<Node> queue = new LinkedList<Node>();

			startNode.status = Node.GRAY;
			queue.addLast(startNode);

			while (!queue.isEmpty()) {
				Node current = queue.removeFirst();
				current.status = Node.BLACK;
				nodeList.addLast(current);

				for (Node neighbor : current.getSuccessorNodes()) {

					if (neighbor.status == Node.WHITE) {
						neighbor.status = Node.GRAY;
						queue.addLast(neighbor);
					}
				}
			}
		}
		return nodeList;
	}

	// -- utils --
	public void inputError() {
		System.out.println("Incorrect input.");
		System.exit(1);
	}
}

