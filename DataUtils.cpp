#include "DataUtils.h"


int binSearch(std::vector<BinarySearchable> list, int target) {

	int min = 0;
	int max = list.size() - 1;
	int mid = 0;

	while (min <= max) {

		mid = (min + max) / 2;

		if (list[mid].uid < target)
			min = mid + 1;

		else if (list[mid].uid > target)
			max = mid - 1;

		else
			return mid;

	}

	//If the search was unsucsessful, return where an object with this id *would* go (for later insertion)
	return -mid;

}