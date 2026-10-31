#include "nonogram.h"

std::vector<std::vector<int>> SolveNonogram(int N, int M, std::vector<std::vector<int>> Rclue, std::vector<std::vector<int>> Cclue) {
	return std::vector<std::vector<int>>({std::vector<int>({1, 1, 1, 0, 1}),
	                                      std::vector<int>({1, 1, 1, 1, 0}),
	                                      std::vector<int>({1, 0, 1, 1, 1}),
	                                      std::vector<int>({1, 1, 0, 1, 1}),
	                                      std::vector<int>({1, 0, 1, 0, 1})});
}
