/*A* Pathfinding node.
 *h value contains the best estimate of
 *current distance to the goal,
 *
 *g value contains the lowest cost
 *to reach this node.
 *
 *f value contains the sum of h and g,
 *which is makes up the best estimated shortest path.*/

typedef struct PathNode {
	int n_position;
	int h_heuristic;
	int g_movecost;
	int f_totalcost;
	struct PathNode* parent;

	bool operator==(const PathNode& pn) const
	{
		return n_position == pn.n_position;
	}

};