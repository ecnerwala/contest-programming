#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
#include <bits/extc++.h>
using namespace std;
using namespace __gnu_pbds;

namespace ecnerwala {

using ll = long long;
using pii = pair<int, int>;
using rect = pair<pii, pii>;

template <typename T> using ordered_set = tree<
T,
null_type,
less<T>,
rb_tree_tag,
tree_order_statistics_node_update>;

class KRectangleIntersection {

public:

	long long maxIntersection(vector <int> xl, vector <int> yl, vector <int> xh, vector <int> yh, int K) {
		int N = int(xl.size());
		vector<rect> rects(N);
		for (int i = 0; i < N; i++) {
			rects[i] = {{yl[i], yh[i]}, {xl[i], xh[i]}};
		}
		sort(rects.begin(), rects.end());
		ll ans = 0;
		for (int a : xl) {
			for (int b : xh) {
				if (a < b) {
					ordered_set<pair<int, int>> st;
					for (int i = 0; i < N; i++) {
						if (rects[i].second.first <= a && b <= rects[i].second.second) {
							// include it
							st.insert(pair<int, int>(rects[i].first.second, i));
							if (int(st.size()) >= K) {
								int hi = st.find_by_order(int(st.size()) - K)->first;
								ans = max(ans, ll(max(0, hi - rects[i].first.first)) * ll(b - a));
							}
						}
					}
				}
			}
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
	int N = 200;
	vector<int> xl(N, 0);
	vector<int> xh(N, int(1e9));
	vector<int> yl(N, 0);
	vector<int> yh(N, int(1e9));
	cerr << KRectangleIntersection().maxIntersection(xl, yl, xh, yh, 1) << '\n';
	cerr << KRectangleIntersection().maxIntersection(xl, yl, xh, yh, 2) << '\n';
	cerr << KRectangleIntersection().maxIntersection(xl, yl, xh, yh, N) << '\n';
}

}  // namespace ecnerwala

using ecnerwala::KRectangleIntersection;

// vim:ft=cpp
