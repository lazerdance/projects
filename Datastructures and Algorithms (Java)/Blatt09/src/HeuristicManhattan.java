import java.util.Comparator;
import java.lang.Math;

/**
 * This class implements a heuristic based on the "Manhatten" distance metric (L1 Norm)
 * 
 **/
public class HeuristicManhattan implements Comparator<CellNode> {
	
		/**
		 * The goal node which the heuristic operates on:
		 */
		CellNode goal;
		
		/**
		 * 
		 * @param goal
		 * 		the target/goal node the heuristic should be computed with
		 */
		public HeuristicManhattan(CellNode goal) {
			this.goal = goal;
		}
		
		/**
		 * Computes an estimate of the remaining distance from node n to the goal node and
		 * updates the node attribute distanceRemainingEstimate
		 * 
		 *  This class implements the L1 norm ("Manhattan" distance)
		 *  as the permissible heuristic
		 *  
		 * @param n
		 * 		node to estimate the remaining distance from
		 */
		public void estimateDistanceToGoal(CellNode n) {
        	//Your implementation here

        	//Hint: you only need to estimate once for each node.
        	 n.distanceToGoalEstimate = Math.abs(n.i - goal.i) + Math.abs(n.j - goal.j);
// 			 System.out.println(n.distanceToGoalEstimate);
        	}

    		/*
    		 * compares two nodes based on the distance heuristic
    		 * Computes the function cost(n) = d(start, n)+h(n, goal), 
    		 * where d(start, n) is the distance from the start to n \
    		 * and h(n, goal) is an estimate of the distance from n to the goal based on the Manhattan distance
    		 *  
    		 *  
    		 * @see java.util.Comparator#compare(java.lang.Object, java.lang.Object)
    		 */		
            public int compare(CellNode n1, CellNode n2) {
            	//Your implementation here
            	n1.distanceToGoalEstimate = Math.abs(n1.i - goal.i) + Math.abs(n1.j - goal.j);
            	n2.distanceToGoalEstimate = Math.abs(n2.i - goal.i) + Math.abs(n2.j - goal.j);
            	double x =  n1.distanceToGoalEstimate + n1.distance;
            	double y =  n2.distanceToGoalEstimate + n2.distance;
//            	int g = n1.cmpareTo(n2);
//            	System.out.println(x + "\t" + y);
            	if (x<y) return -1;
            	
            	if(x==y) return 0;
            	
            	return 1;
            }		
}

