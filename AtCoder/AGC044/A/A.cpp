#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t N, A, B, C, D; cin >> N >> A >> B >> C >> D;
		auto go = [&]() -> int64_t {
			__int128_t ans = N * __int128_t(D);

			map<int64_t, int64_t> dist;
			priority_queue<pair<int64_t, int64_t>, vector<pair<int64_t, int64_t>>, greater<pair<int64_t, int64_t>>> pq;
			dist.emplace(N, 0);
			pq.emplace(0, N);

			while (!pq.empty()) {
				int64_t d = pq.top().first, cur = pq.top().second; pq.pop();
				if (d != dist[cur]) continue;
				//cerr << "row " << d << ' ' << cur << '\n';
				ans = min(ans, cur * __int128_t(D) + d);
				for (auto [p, pcost] : {pair<int, int64_t>{2, A}, {3, B}, {5, C}}) {
					for (int64_t nxt : {cur / p, (cur + p - 1) / p}) {
						int64_t nd = d + abs(nxt * p - cur) * D + pcost;
						//cerr << "nxt " << nxt << ' ' << nd << '\n';
						auto it = dist.find(nxt);
						if (it == dist.end()) {
							dist.emplace(nxt, nd);
							pq.emplace(nd, nxt);
						} else if (nd < it->second) {
							it->second = nd;
							pq.emplace(nd, nxt);
						}
					}
				}
			}

			return int64_t(ans);
		};

		cout << go() << '\n';
	}

	return 0;
}
