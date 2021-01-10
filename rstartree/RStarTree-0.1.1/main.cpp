/*
 *  Copyright (c) 2008 Dustin Spicuzza <dustin@virtualroadside.com>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of version 2 of the GNU General Public License
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


#include <string>
#include <ctime>

#include <stdio.h>
#include "RStarTree.h"

#define RANDOM_DATASET
//#define GUTTMAN_DATASET

#ifdef RANDOM_DATASET
	typedef RStarTree<int, 2, 32, 64> 			RTree;
#else
	typedef RStarTree<std::string, 2, 2, 3> 	RTree;
#endif

typedef RTree::BoundingBox			BoundingBox;


BoundingBox bounds(int x, int y, int w, int h)
{
	BoundingBox bb;
	
	bb.edges[0].first  = x;
	bb.edges[0].second = x + w;
	
	bb.edges[1].first  = y;
	bb.edges[1].second = y + h;
	
	return bb;
}


struct Visitor {
	int count;
	bool ContinueVisiting;
	
	Visitor() : count(0), ContinueVisiting(true) {};
	
	void operator()(const RTree::Leaf * const leaf) 
	{
#if defined( RANDOM_DATASET )
		//std::cout << "Visiting " << count << std::endl;
#elif defined( GUTTMAN_DATASET )
		std::cout << "#" << count << ": visited " << leaf->leaf << " with bound " << leaf->bound.ToString() << std::endl;	
#else
		#error "Undefined dataset"
#endif
		count++;
	}
};



int main(int argc, char ** argv)
{
	RTree tree;
	Visitor x;
	
	// insert a bunch of items into the tree
	// Note: this dataset is the one shown on Guttman's original paper



	srand(time(0));

	#define nodes 9000000
	
	for (int i = 0; i < nodes; i++) {
		tree.Insert(i, bounds(rand() % 1000, rand() % 1000, rand() % 10, rand() % 10));
		if(i>99999&&i%100000==0)
			std::cout << "NO" << i << "\n";
	}
		
	//for (int i = 0; i < nodes/2; i++)
	//	tree.Insert(i, bounds( rand() % 1000, rand() % 1000, rand() % 20, rand() % 20));
		
	BoundingBox bound = bounds( 100,100, 300,400 );
	
	x = tree.Query(RTree::AcceptAny(), Visitor());
	std::cout << "AcceptAny: " << x.count << " nodes visited (" << tree.GetSize() << " nodes in tree)" << std::endl;
	
	
	std::cout << "Searching in " << bound.ToString() << std::endl;
	x = tree.Query(RTree::AcceptEnclosing(bound), Visitor());
	std::cout << "Visited " << x.count << " nodes (" << tree.GetSize() << " nodes in tree)" << std::endl;
	
	std::cout << "Removing enclosed area " << bound.ToString() << std::endl;
	tree.RemoveBoundedArea(bound);
	
	std::cout << "Searching in " << bound.ToString() << std::endl;
	x = tree.Query(RTree::AcceptEnclosing(bound), Visitor());
	std::cout << "Visited " << x.count << " nodes. (" << tree.GetSize() << " nodes in tree)" << std::endl;
	
	//tree.Print();
		


	
	
	return 0;
}


/*

http://donar.umiacs.umd.edu/quadtree/rectangles/cifquad.html

1.0 5.0  3.0 2.0
6.0 1.0  2.0 2.0
6.0 4.0 2.0 2.0
9.0 0.0  2.0 14.0
13.0 1.0  1.0 9.0
12.0 5.0  2.0 2.0
0.0 16.0  2.0 2.0
3.0 11.0  6.0 7.0
14.0 10.0  7.0 4.0
16.0 8.0  2.0 9.0
17.0 12.0  3.0 3.0



Insert-BoundingBox{(1.0,5.0)(4.0,7.0)}
Insert-BoundingBox{(6.0,1.0)(8.0,3.0)}
Insert-BoundingBox{(6.0,4.0)(8.0,6.0)}
Insert-BoundingBox{(9.0,0.0)(11.0,14.0)}
Insert-BoundingBox{(13.0,1.0)(14.0,1.0)}
Insert-BoundingBox{(12.0,5.0)(14.0,7.0)}
Insert-BoundingBox{(0.0,16.0)(2.0,18.0)}
Insert-BoundingBox{(3.0,11.0)(9.0,18.0)}
Insert-BoundingBox{(14.0,10.0)(21.0,14.0)}
Insert-BoundingBox{(16.0,8.0)(18.0,17.0)}
Insert-BoundingBox{(17.0,12.0)(20.0,15.0)}

*/


