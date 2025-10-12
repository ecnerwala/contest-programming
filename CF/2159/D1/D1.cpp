#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int64_t> A(N);
		for (auto& a : A) { cin >> a; }
		struct stk_t {
			int64_t v;
			int idx;
			int dist;
		};
		std::vector<stk_t> stk; stk.reserve(N+1);
		stk.push_back({-1, -1, 0});
		for (int i = 0; i < N; i++) {
			int64_t a = A[i];
			while (stk.size() > 1 && stk.back().v >= a) {
				stk.pop_back();
			}
			int dist = 1 + stk.back().dist;
			for (int l = 2; l <= dist && l <= 3; l++) {
				// we need tgt * l >= a
				// tgt >= a / l
				int64_t tgt = (a + l - 1) / l;
				int mi = 0, ma = int(stk.size());
				while (ma - mi > 1) {
					int md = mi + (ma - mi) / 2;
					if (stk[md].v >= tgt) ma = md;
					else mi = md;
				}
				dist = std::min(dist, stk[mi].dist + l);
			}
			stk.push_back({a, i, dist});
		}
		cout << stk.back().dist << '\n';
	}

	return 0;
}
