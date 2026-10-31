#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class EllysIncinerator {

public:

	double getMax(vector <double> X, vector <double> Y, vector <double> R) {
		int N = int(X.size());
		auto is_good = [&](double V) -> bool {
			for (int i = 0; i < N; i++) {
				std::vector<std::pair<double, int>> p; p.reserve(2*N+8);
				int cur = 0;
				auto add_range = [&](double c, double d) -> void {
					// c - d to c + d
					if (c-d < -M_PI) {
						cur++;
						p.push_back({c+d, -1});
						p.push_back({c-d + 2 * M_PI, 1});
					} else if (c+d > M_PI) {
						cur++;
						p.push_back({c+d - 2 * M_PI, -1});
						p.push_back({c-d, 1});
					} else {
						p.push_back({c-d, 1});
						p.push_back({c+d, -1});
					}
				};
				double r1 = R[i] + V;
				if (r1 >= 1000 - X[i]) {
					add_range(0, acos((1000 - X[i]) / r1));
				}
				if (r1 >= X[i] - 0) {
					add_range(M_PI, acos((X[i] - 0) / r1));
				}
				if (r1 >= 1000 - Y[i]) {
					add_range(M_PI/2, acos((1000 - Y[i]) / r1));
				}
				if (r1 >= Y[i] - 0) {
					add_range(-M_PI/2, acos((Y[i] - 0) / r1));
				}
				for (int j = 0; j < N; j++) {
					if (i == j) continue;
					double l = hypot(X[i] - X[j], Y[i] - Y[j]);
					double r2 = R[j] + V;
					if (r1 + r2 <= l) continue;
					if (r2 >= r1 + l) {
						goto enclosed;
					}
					if (r1 >= r2 + l) {
						continue;
					}

					double c = atan2(Y[j] - Y[i], X[j] - X[i]);
					double d = acos((r1 * r1 + l * l - r2 * r2) / (2 * r1 * l));
					add_range(c, d);
				}
				sort(p.begin(), p.end());
				for (auto it : p) {
					cur += it.second;
					if (cur == 0) return true;
				}
enclosed: ;
			}
			return false;
		};

		double mi = 0;
		double ma = 10000;
		for (int z = 0 ;z < 60; z++) {
			double md = (mi + ma) / 2;
			if (is_good(md)) mi = md;
			else ma = md;
		}
		return mi;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::EllysIncinerator;

// vim:ft=cpp
