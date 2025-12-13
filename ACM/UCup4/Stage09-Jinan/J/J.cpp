#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> D(N);
		for (auto& d : D) cin >> d;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
		std::vector<int> depth(N);
		[&](this auto&& self, int cur, int prv) -> void {
			depth[cur] = (prv == -1) ? 0 : depth[prv] + 1;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur);
			}
		}(0, -1);
		std::vector<bool> is_unlocked(N);
		std::vector<int> prv_unlock(N, -1);
		std::vector<int> first_unlock(N, -1);
		{
			std::vector<int> unlock_at(N+1, -1);
			[&](this auto&& self, int cur, int prv) -> void {
				assert(depth[cur] < N);
				first_unlock[cur] = unlock_at[depth[cur]];
				int cover_depth;
				if (D[cur] == -1) {
					is_unlocked[cur] = true;
					cover_depth = N;
				} else {
					is_unlocked[cur] = false;
					cover_depth = std::min(N, depth[cur] + D[cur]);
				}
				prv_unlock[cur] = unlock_at[cover_depth];
				unlock_at[cover_depth] = cur;
				for (int nxt : adj[cur]) {
					if (nxt == prv) continue;
					self(nxt, cur);
				}
				// restore
				unlock_at[cover_depth] = prv_unlock[cur];
			}(0, -1);
		}
		std::vector<bool> is_uncovered(N);
		std::vector<std::vector<int>> to_uncover(N);
		{
			std::vector<std::pair<int, int>> stk(N+1);
			int L = 0;
			stk[L] = {N+1, -1};
			[&](this auto&& self, int cur, int prv) -> void {
				assert(depth[cur] < N);
				int orig_L = L;
				assert(stk[L].first >= depth[cur]-1);
				while (stk[L].first <= depth[cur]) {
					L--;
				}
				assert(stk[L].first > depth[cur]);
				if (stk[L].second == -1) {
					assert(L == 0);
					is_uncovered[cur] = true;
				} else {
					is_uncovered[cur] = false;
					to_uncover[stk[L].second].push_back(cur);
				}
				int cover_depth;
				if (D[cur] == -1) {
					is_unlocked[cur] = true;
					cover_depth = N;
				} else {
					is_unlocked[cur] = false;
					cover_depth = std::min(N, depth[cur] + D[cur]);
				}
				assert(cover_depth >= depth[cur]);
				int mi = 0, ma = L+1;
				while (ma - mi > 1) {
					int md = (mi + ma) / 2;
					if (cover_depth >= stk[md].first) ma = md;
					else mi = md;
				}
				L = ma;
				std::pair<int, int> old_stk = stk[L];
				stk[L] = {cover_depth, cur};

				for (int nxt : adj[cur]) {
					if (nxt == prv) continue;
					self(nxt, cur);
				}

				stk[L] = old_stk;
				L = orig_L;
			}(0, -1);
		}

		std::priority_queue<int, std::vector<int>, std::greater<int>> pq;
		for (int i = 0; i < N; i++) {
			if (is_unlocked[i] && is_uncovered[i]) {
				pq.push(i);
			}
		}
		std::vector<int> res; res.reserve(N);
		while (!pq.empty()) {
			int cur = pq.top(); pq.pop();
			res.push_back(cur);
			for (auto nxt : to_uncover[cur]) {
				assert(!is_uncovered[nxt]);
				is_uncovered[nxt] = true;
				if (is_unlocked[nxt]) pq.push(nxt);
			}
			for (int nxt = first_unlock[cur]; nxt != -1 && !is_unlocked[nxt]; nxt = prv_unlock[nxt]) {
				is_unlocked[nxt] = true;
				if (is_uncovered[nxt]) pq.push(nxt);
			}
		}
		if (int(res.size()) == N) {
			for (int i = 0; i < N; i++) cout << res[i]+1 << " \n"[i+1==N];
		} else {
			cout << -1 << '\n';
		}
	}

	return 0;
}
