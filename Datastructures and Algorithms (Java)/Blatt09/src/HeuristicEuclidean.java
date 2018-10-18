import java.util.Comparator;

/**
 * This class implements a heuristic based on the "Euclidean" distance metric (L2 Norm)
 * 
 **/
 public class HeuristicEuclidean implements Comparator<CellNode> {
	
	    /*
	    This is the goal node the heuristic uses to estimate the remaining distance
	    */
		CellNode goal;
		
		public HeuristicEuclidean(CellNode goal) {
			this.goal = goal;
		}
		
		/**
		 * Computes an estimate of the remaining distance from node n to the goal node and
		 * updates the node attribute distanceRemainingEstimate
		 * 
		 *  This class implements the L2 norm (euclidean distance)
		 *  as the permissible heuristic
		 *  
		 * @param n
		 * 		node to estimate the remaining distance from
		 */
		public void estimateDistanceToGoal(CellNode n) {
        	//Your implementation here
			n.distanceToGoalEstimate = 0.0; //Setting this to 0.0 effectively turns A* into Dijkstra
        	//Hint: you only need to compute the estimate once for each node.
			n.distanceToGoalEstimate = Math.sqrt(Math.pow(n.i-goal.i, 2) + Math.pow(n.j-goal.j, 2));
			System.out.println(n.distanceToGoalEstimate);
        	return;
		}
		
		
		
		/*
		 * compares two nodes based on an estimate of the path length to goal
		 * Computes the function cost(n) = d(start, n)+h(n, goal), 
		 * where d(start, n) is the distance from the start to n \
		 * and h(n, goal) is an estimate of the distance from n to the goal based on the Euclidean distance.
		 *  
		 * @see java.util.Comparator#compare(java.lang.Object, java.lang.Object)
		 */		
        public int compare(CellNode n1, CellNode n2) {
        	
        	//Your implementation here
        	n1.distanceToGoalEstimate = Math.abs(n1.i - goal.i) + Math.abs(n1.j - goal.j);
        	n2.distanceToGoalEstimate = Math.abs(n2.i - goal.i) + Math.abs(n2.j - goal.j);
        	double x = n1.distanceToGoalEstimate + n1.distance;
        	double y = n2.distanceToGoalEstimate + n2.distance;
//        	int g = n1.compareTo(n2);
        	if(x<y) return -1;
        	if(x>y) return 1;
        	
        	return 0;
        }	
}

