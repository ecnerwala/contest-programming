#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<int> P(N);
	for (auto& p : P) { cin >> p; }
	std::vector<int> invP(N);
	for (int i = 0; i < N; i++) invP[P[i]] = i;
	std::vector<std::array<int, 2>> segs(M);
	for (auto& [l, r] : segs) { cin >> l >> r; l--, r--; }

	std::set<std::pair<int, int>> by_l;
	std::set<std::pair<int, int>> by_r;
	for (int i = 0; i < M; i++) {
		by_l.insert({segs[i][0], i});
		by_r.insert({segs[i][1], i});
	}

	int64_t ans = 0;
	for (int w = 0; w <= N; w++) {
		int pos = w == N ? N : invP[w];
		int cur = -1;
		auto do_insert = [&](int i) {
			by_l.erase({segs[i][0], i});
			by_r.erase({segs[i][1], i});
			if (cur == -1) {
				cur = i;
			} else {
				ans += w;
				segs[cur][0] = std::max(segs[cur][0], segs[i][0]);
				segs[cur][1] = std::min(segs[cur][1], segs[i][1]);
			}
		};
		while (!by_l.empty() && by_l.rbegin()->first > pos) {
			do_insert(by_l.rbegin()->second);
		}
		while (!by_r.empty() && by_r.begin()->first < pos) {
			do_insert(by_r.begin()->second);
		}
		if (cur != -1) {
			int i = cur;
			by_l.insert({segs[i][0], i});
			by_r.insert({segs[i][1], i});
		}
	}
	cout << ans << '\n';

	return 0;
}
