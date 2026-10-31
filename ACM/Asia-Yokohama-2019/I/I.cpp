#include<bits/stdc++.h>
using namespace std;

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

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	int N, M; cin >> N >> M;
	vector<vector<pair<int, int>>> adj(N+1);
	for (int e = 0; e < M; e++) {
		int x, y; cin >> x >> y;
		adj[x].emplace_back(e, y);
		adj[y].emplace_back(e, x);
	}

	vector<int> depth(N+1, -1);
	vector<int> bridgeParEdge(N+1, -1);
	vector<vector<int>> par(N+1, vector<int>(20, 0));

	vector<pair<int, int>> dir(M); // down by default

	depth[0] = 0;
	y_combinator([&](auto self, int cur = 1, int prv = 0) -> int {
		depth[cur] = depth[prv] + 1;
		par[cur][0] = prv;
		for (int i = 0; par[cur][i]; i++) {
			par[cur][i+1] = par[par[cur][i]][i];
		}
		int lowval = depth[cur];
		for (auto it : adj[cur]) {
			int e = it.first;
			int nxt = it.second;
			if (nxt == prv) continue;

			if (depth[nxt] == -1) {
				dir[e] = {cur, nxt};
				int nlowval = self(nxt, cur);
				if (nlowval == depth[nxt]) {
					bridgeParEdge[nxt] = e;
				}
				lowval = min(lowval, nlowval);
			} else if (depth[nxt] < depth[cur]) {
				dir[e] = {cur, nxt};
				lowval = min(lowval, depth[nxt]);
			}
		}
		return lowval;
	})();

	vector<vector<bool>> mustUp(N+1, vector<bool>(20,0));
	vector<vector<bool>> mustDown(N+1, vector<bool>(20,0));

	int K; cin >> K;
	for (int q = 0; q < K; q++) {
		int a, b; cin >> a >> b;
		for (int i = 0; (1<<i) <= abs(depth[a] - depth[b]); i++) {
			if ((1 << i) & abs(depth[a] - depth[b])) {
				if (depth[a] > depth[b]) {
					mustUp[a][i] = true;
					a = par[a][i];
				} else {
					mustDown[b][i] = true;
					b = par[b][i];
				}
			}
		}
		if (a != b) {
			int i = 0;
			while (par[a][i] != par[b][i]) i++;
			while (i) {
				--i;
				if (par[a][i] != par[b][i]) {
					mustUp[a][i] = true;
					a = par[a][i];
					mustDown[b][i] = true;
					b = par[b][i];
				}
			}
			assert(i == 0);
			mustUp[a][0] = true;
			mustDown[b][0] = true;
		}
	}

	y_combinator([&](auto self, int cur = 1) -> void {
		for (auto it : adj[cur]) {
			int nxt = it.second;
			if (depth[nxt] == depth[cur]+1) {
				self(nxt);
			}
		}
		for (int i = 19; i > 0; i--) {
			if (mustUp[cur][i]) {
				mustUp[cur][i-1] = true;
				mustUp[par[cur][i-1]][i-1] = true;
			}
			if (mustDown[cur][i]) {
				mustDown[cur][i-1] = true;
				mustDown[par[cur][i-1]][i-1] = true;
			}
		}

		if (bridgeParEdge[cur] != -1) {
			int e = bridgeParEdge[cur];
			if (mustUp[cur][0] && mustDown[cur][0]) {
				cout << "No" << '\n';
				exit(0);
			} else if (mustUp[cur][0]) {
				swap(dir[e].first, dir[e].second);
			}
		}
	})();
	cout << "Yes" << '\n';
	for (int e = 0; e < M; e++) {
		cout << dir[e].first << ' ' << dir[e].second << '\n';
	}

	return 0;
}
