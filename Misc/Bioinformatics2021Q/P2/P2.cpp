#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int NA, NB, NQ; cin >> NA >> NB >> NQ;
		auto read_float = [&]() -> int64_t {
			double a; cin >> a;
			a *= 1e6;
			return int64_t(a + 0.5 - (a < 0));
		};
		vector<int64_t> A(NA); for (auto& a : A) a = read_float();
		vector<int64_t> B(NB); for (auto& b : B) b = read_float();

		vector<pair<int64_t, int>> Q(NQ);
		for (int q = 0; q < NQ; q++) Q[q] = {read_float(), q};

		vector<pair<int, int>> ans(NQ);

		if (NA <= int(1e4) && NB <= int(1e4)) {
			vector<pair<int64_t, pair<int, int>>> vals; vals.reserve(NA * NB);
			for (int i = 0; i < NA; i++) {
				for (int j = 0; j < NB; j++) {
					if (A[i] + B[j] > 0) vals.push_back({A[i] + B[j], {i, j}});
				}
			}
			sort(vals.begin(), vals.end());
			for (int q = 0; q < NQ; q++) {
				auto it = lower_bound(vals.begin(), vals.end(), pair<int64_t, pair<int, int>>{Q[q].first, {-1, -1}});
				if (it == vals.end()) ans[q] = std::prev(it)->second;
				else if (it == vals.begin()) ans[q] = it->second;
				else if (abs(it->first - Q[q].first) <= abs(std::prev(it)->first - Q[q].first)) ans[q] = it->second;
				else ans[q] = std::prev(it)->second;
			}
		} else if (NB <= int(1e3) && NQ <= int(1e3)) {
			vector<pair<int64_t, int>> VA(NA); for (int i = 0; i < NA; i++) VA[i] = {A[i], i};
			sort(VA.begin(), VA.end());
			for (int q = 0; q < NQ; q++) {
				pair<int64_t, pair<int, int>> resp({1e18, {-1, -1}});
				for (int j = 0; j < NB; j++) {
					auto it = lower_bound(VA.begin(), VA.end(), pair<int64_t, int>{Q[q].first - B[j], -1});
					if (it != VA.end()) {
						assert(it->first + B[j] > 0);
						resp = min(resp, {it->first + B[j] - Q[q].first, {it->second, j}});
					}
					if (it != VA.begin()) {
						--it;
						if (it->first + B[j] > 0) {
							resp = min(resp, {Q[q].first - (it->first + B[j]), {it->second, j}});
						}
					}
				}
				ans[q] = resp.second;
			}
		} else assert(false);

		for (auto it : ans) {
			cout << it.first+1 << ' ' << it.second+1 << '\n';
		}
	}

	return 0;
}
