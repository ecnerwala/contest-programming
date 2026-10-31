#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class PlugStuffIn {

public:

	vector <int> plug(vector <int> gadgets) {
		int N = int(gadgets.size());
		std::vector<int> res(N, -1);
		std::vector<int> q; q.reserve(N);
		q.push_back(N);

		for (int i = 0; i < N; i++) {
			if (gadgets[i]) {
				assert(!q.empty());
				res[i] = q.back(); q.pop_back();
				for (int j = 0; j < gadgets[i]; j++) q.push_back(i);
			}
		}
		for (int i = 0; i < N; i++) {
			if (!gadgets[i]) {
				if (q.empty()) return {};
				res[i] = q.back(); q.pop_back();
			}
		}
		return res;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::PlugStuffIn;

// vim:ft=cpp
