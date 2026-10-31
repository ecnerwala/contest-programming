#include <bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

void wssert(bool b) {
	if (!b) {
		std::cout << "GARBAGE" << '\n';
		exit(0);
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, K, S; cin >> N >> M >> K >> S;
		vector<vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		vector<int> sz(N);
		vector<int> par(N);
		vector<int> depth(N);
		par[0] = -1;
		std::y_combinator([&](auto self, int cur) -> void {
			sz[cur] = 1;
			for (int nxt : adj[cur]) {
				depth[nxt] = depth[cur]+1;
				par[nxt] = cur;
				adj[nxt].erase(find(adj[nxt].begin(), adj[nxt].end(), cur));
				self(nxt);
				sz[cur] += sz[nxt];
			}
		})(0);

		int cur_idx = 0;
		vector<int> preorder(N);
		vector<int> st(N);
		vector<int> en(N);
		vector<int> heavy_root(N);
		heavy_root[0] = 0;
		std::y_combinator([&](auto self, int cur) -> void {
			st[cur] = cur_idx;
			preorder[cur_idx++] = cur;
			if (!adj[cur].empty()) {
				auto it = max_element(adj[cur].begin(), adj[cur].end(), [&](int a, int b) { return sz[a] < sz[b]; });
				std::swap(*adj[cur].begin(), *it);
			}
			bool is_heavy = true;
			for (int nxt : adj[cur]) {
				heavy_root[nxt] = is_heavy ? heavy_root[cur] : nxt;
				is_heavy = false;
				self(nxt);
			}
			en[cur] = cur_idx;
		})(0);
		assert(cur_idx == N);

		// sort the tokens by top time
		vector<pair<int, int>> tokens(M);
		for (auto& [t, v] : tokens) { cin >> t >> v; v--; v = st[v]; }
		sort(tokens.begin(), tokens.end());

		set<int> all_token_times;

		vector<set<int>> token_times(N);
		for (auto [t, v] : tokens) {
			all_token_times.insert(t);
			token_times[preorder[v]].insert(t);
		}

		vector<int> min_time(N, int(2e9));
		min_pq<pair<int, int>> pq;
		for (int i = 0; i < N; i++) {
			if (adj[i].empty()) {
				int t = S - depth[i];
				// Special case: must be visible when you first place it.
				while (token_times[i].count(t)) t++;
				pq.push({min_time[i] = t, i});
			}
		}

		vector<bool> visited(N, false);
		int cur_time = -int(1e9);
		// We need to store the alive vertices in one set so that we can mark them bad
		set<pair<int, int>> alive_verts; // stored by preorder; also, we never have both a vertex and its ancestor
		// We also want to store the parents in an HLD which we'll skip through
		set<pair<int, int>> alive_pars; // stored by preorder
		while (true) {
			//cerr << "cur_time = " << cur_time << '\n';
			if (alive_verts.empty()) {
				if (pq.empty()) {
					break;
				}
				cur_time = max(cur_time, pq.top().first);
			}
			//cerr << "cur_time = " << cur_time << '\n';
			while (!pq.empty() && pq.top().first <= cur_time) {
				int i = pq.top().second;
				pq.pop();
				if (visited[i]) continue;
				//cerr << "pop " << i << ' ' << min_time[i] << '\n';
				assert(min_time[i] == cur_time);
				if (i == 0) {
					goto answer;
				}
				// let us do the unphased walking
				if (min_time[i] + K+1 < min_time[par[i]]) {
					pq.push({min_time[par[i]] = min_time[i] + K + 1, par[i]});
				}
				std::y_combinator([&](auto self, int cur) -> void {
					alive_verts.erase({st[cur], en[cur]});
					alive_pars.erase({st[par[cur]], st[cur]});
					if (visited[cur]) return;
					visited[cur] = true;
					for (int nxt : adj[cur]) self(nxt);
				})(i);
				alive_verts.insert({st[i], en[i]});
				alive_pars.insert({st[par[i]], st[i]});
			}
			if (alive_verts.empty()) continue;
			// Now we grab our excluded vertices
			set<int> bad_verts;
			{
				auto it = lower_bound(tokens.begin(), tokens.end(), pair<int, int>{cur_time, -1});
				for (; it != tokens.end() && it->first == cur_time; ++it) {
					int loc = it->second;
					//cerr << "bad vertex " << i << " at depth " << depth[i] << '\n';
					auto vit = alive_verts.upper_bound({loc+1, -1});
					if (vit != alive_verts.begin()) {
						--vit;
						assert(vit->first <= loc);
						if (loc < vit->second) {
							// vit is bad
							bad_verts.insert(vit->first);
						}
					}
				}
			}
			map<int, int> bad_intervals;
			{
				auto it = lower_bound(tokens.begin(), tokens.end(), pair<int, int>{cur_time + 2, -1});
				for (; it != tokens.end() && it->first == cur_time+2; ++it) {
					int loc = it->second;
					// this guy blocks all its parents and *their* children
					int i = preorder[loc];
					//cerr << "bad range " << i << " at depth " << depth[i]+1 << '\n';
					while (true) {
						int r = heavy_root[i];
						assert(st[i] - st[r] == depth[i] - depth[r]);
						bad_intervals[st[r]] ++;
						bad_intervals[st[i]+1] --;
						if (r == 0) break;
						i = par[r];
					}
				}
			}

			{
				int pref = 0;
				for (auto& [_, v] : bad_intervals) v = pref = v + pref;
			}
			{
				auto& bads = bad_intervals;
				auto& cnds = alive_pars;
				auto bit = bads.begin();
				auto cit = cnds.begin();
				while (cit != cnds.end()) {
					if (bit != bads.end() && bit->first <= cit->first) {
						while (bit->second != 0) {
							++bit;
							assert(bit != bads.end());
						}
						cit = cnds.lower_bound({bit->first, -1});
						++bit;
						continue;
					}

					int loc = cit->second;
					if (bad_verts.count(loc)) {
						++cit;
						continue;
					}
					cit = cnds.erase(cit);
					int i = preorder[loc];
					alive_verts.erase({st[i], en[i]});

					if (cur_time + 2 < min_time[par[i]]) {
						pq.push({min_time[par[i]] = cur_time + 2, par[i]});
					}
				}
			}

			cur_time++;
		}

answer:
		int ans = min_time[0];
		while (all_token_times.count(ans)) ans++;
		cout << ans << '\n';
	}

	return 0;
}

// At a vertex v, time t is bad if there is something with t - depth[v] == t_i
// The parent is bad if t - depth[v] + 2 == t_i
//
// We'll use t - depth, which means that the tokens are stationary
//
// You can move up in 2 time as long as both are open at the start/end
// Otherwise, you can phase+move up in K+1 time
