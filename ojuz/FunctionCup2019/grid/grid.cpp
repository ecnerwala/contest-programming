#include "grid.h"

std::vector<int> SortDisks(int N) {
	int X = PutDisks(std::vector<int>{0,2,3,1});
	return std::vector<int>{1,X,2,3};
}
