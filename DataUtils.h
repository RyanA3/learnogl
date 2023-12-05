#pragma once
#include <vector>


class BinarySearchable {

//Unique id for identifying this object
public:
	int uid;

};




//Performs a binary search on a list of objects with unique ids (assuming they're in order)
int binSearch(std::vector<BinarySearchable> list, int target);