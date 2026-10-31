#include <bits/stdc++.h>

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int CASES; cin >> CASES;
	while (CASES--) {
		int N, K; cin >> N >> K;
		std::vector<std::string> S(N);
		for (auto& s : S) cin >> s;
		std::string T; cin >> T;

		const int INF = 1e9;
		std::vector<int> single_cost(K, INF);
		single_cost[0] = 0;
		for (const auto& s : S) {
			setmin(single_cost[int(s.size()) % K], int(s.size()) / K + 1);
		}

		std::vector<int> cost(K, INF);
		min_pq<std::pair<int, int>> pq;
		cost[0] = 0;
		pq.push({0, 0});
		while (!pq.empty()) {
			auto [c, v] = pq.top(); pq.pop();
			assert(c >= cost[v]);
			if (c != cost[v]) continue;

			for (int d = 0; d < K; d++) {
				int nc = single_cost[d];
				if (nc == INF) continue;
				nc += c;
				int nv = v + d;
				if (nv >= K) {
					nv -= K;
					nc++;
				}

				if (nc < cost[nv]) {
					cost[nv] = nc;
					pq.push({nc, nv});
				}
			}
		}

		std::vector<int> del_cost(K);
		del_cost[0] = 0;
		for (int i = 1; i < K; i++) {
			del_cost[i] = cost[K-i] == INF ? INF : cost[K-i] + 1;
		}

		//for (int i = 0; i < K; i++) { cerr << "del_cost " << i << ' ' << del_cost[i] << '\n'; }

		int tot_L = 0;
		for (const auto& s : S) tot_L += int(s.size());

		std::vector<std::array<int, 26>> ch; ch.reserve(tot_L);
		std::vector<int> min_cost; min_cost.reserve(tot_L);
		ch.emplace_back(); ch.back().fill(-1);
		min_cost.push_back(INF);
		for (const auto& s : S) {
			int cur = 0;
			for (int z = 0; z <= int(s.size()); z++) {
				int rem = int(s.size()) - z;
				if (del_cost[rem % K] != INF) {
					setmin(min_cost[cur], 1 + del_cost[rem % K] + rem / K);
				}

				if (z == int(s.size())) break;

				int dir = s[z] - 'a';
				if (ch[cur][dir] == -1) {
					int nxt = int(ch.size());
					ch[cur][dir] = nxt;
					ch.emplace_back(); ch.back().fill(-1);
					min_cost.push_back(INF);
					cur = nxt;
				} else {
					cur = ch[cur][dir];
				}
			}
		}

		int64_t BIG_INF = 1e18;
		std::vector<int64_t> dp(int(T.size())+1, BIG_INF);
		dp[0] = 0;
		for (int i = 0; i < int(T.size()); i++) {
			if (dp[i] == BIG_INF) continue;
			int cur = 0;
			for (int j = i; j < int(T.size()); j++) {
				cur = ch[cur][T[j] - 'a'];
				if (cur == -1) break;
				if (min_cost[cur] == INF) continue;
				setmin(dp[j+1], dp[i] + min_cost[cur]);
			}
		}
		int64_t ans = dp.back();
		if (ans >= BIG_INF) {
			cout << -1 << '\n';
		} else {
			cout << ans << '\n';
		}
	}

	return 0;
}
