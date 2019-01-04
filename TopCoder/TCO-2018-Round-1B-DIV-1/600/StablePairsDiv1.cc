#include <bits/stdc++.h>
using namespace std;

class StablePairsDiv1 {

public:

	vector<int> findMaxStablePairs(int n, int c, int k) {
		if (2 * k > n) {
			return vector<int>{};
		}
		if (k == 1) {
			return vector<int>{n-1, n};
		}

		if (c < 4) {
			return vector<int>{};
		}

		if ((n + (n-1) - (k-1) * c) < 3) {
			return vector<int>{};
		}

		vector<int> res;
		for (int s = n + (n-1); int(res.size()) < 2 * k; s -= c) {
			res.push_back(s / 2 + 1);
			res.push_back(s - (s / 2 + 1));
		}
		reverse(res.begin(), res.end());
		return res;
	}
};

// vim:ft=cpp
