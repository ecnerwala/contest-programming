#include <bits/stdc++.h>
using namespace std;

class ArithmeticSequenceDiv1 {

public:

	int findMinCost(vector <int> x) {
		int best = int(1e9);
		for (int z = -1000; z <= 1000; z++) {
			for (int d = -100; d <= 100; d++) {
				int val = 0;
				for (int i = 0; i < int(x.size()); i++) {
					val += abs(z + i * d - x[i]);
				}
				best = min(best, val);
			}
		}
		return best;
	}
};

// vim:ft=cpp
