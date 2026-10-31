#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class MedianFaking {

public:

	vector <int> minimize(int F, int M, vector <int> data, int goal) {
		map<pair<int, int>, pair<int, int>> dp;
		dp[pair<int, int>(0,0)] = pair<int, int>(0,0);
		for (int i = 0; i < F; i++) {
			auto dp0 = dp;
			auto dp1 = dp;
			for (auto& it : dp1) {
				it.second.first ++;
			}
			for (int j = 0; j < M; j++) {
				int v = data[i * M + j];
				map<pair<int, int>, pair<int, int>> ndp;
				for (auto it : dp0) {
					if (v >= goal) {
						auto cnt = it.first;
						cnt.second++;
						auto val = it.second;
						if (!ndp.count(cnt)) ndp[cnt] = val;
						else ndp[cnt] = min(ndp[cnt], val);
					}
					if (v <= goal) {
						auto cnt = it.first;
						cnt.first++;
						auto val = it.second;
						if (!ndp.count(cnt)) ndp[cnt] = val;
						else ndp[cnt] = min(ndp[cnt], val);
					}
					if (v == goal && it.first.first + it.first.second == i * M + j) {
						auto cnt = it.first;
						auto val = it.second;
						if (!ndp.count(cnt)) ndp[cnt] = val;
						else ndp[cnt] = min(ndp[cnt], val);
					}
				}
				dp0 = ndp;
				ndp = {};
				for (auto it : dp1) {
					{
						auto cnt = it.first;
						cnt.second++;
						auto val = it.second;
						if (!(v >= goal)) val.second++;
						if (!ndp.count(cnt)) ndp[cnt] = val;
						else ndp[cnt] = min(ndp[cnt], val);
					}
					{
						auto cnt = it.first;
						cnt.first++;
						auto val = it.second;
						if (!(v <= goal)) val.second++;
						if (!ndp.count(cnt)) ndp[cnt] = val;
						else ndp[cnt] = min(ndp[cnt], val);
					}
					if (it.first.first + it.first.second == i * M + j) {
						auto cnt = it.first;
						auto val = it.second;
						if (!(v == goal)) val.second++;
						if (!ndp.count(cnt)) ndp[cnt] = val;
						else ndp[cnt] = min(ndp[cnt], val);
					}
				}
				dp1 = ndp;
			}

			for (auto it : dp0) {
				if (!dp1.count(it.first)) dp1[it.first] = it.second;
				else dp1[it.first] = min(dp1[it.first], it.second);
			}
			dp = dp1;
		}

		auto res = dp[pair<int, int>((F*M-1)/2, (F*M)/2)];
		return vector<int>({res.first, res.second});
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::MedianFaking;

// vim:ft=cpp
