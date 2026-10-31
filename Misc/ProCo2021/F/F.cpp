#include <bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(nullptr);
	int N; cin >> N;
	vector<int> V(N); for (auto& v : V) cin >> v;
	vector<vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	vector<pair<int, int>> vals(N);
	for (int i = 0; i < N; i++) vals[i] = {V[i], i};
	sort(vals.begin(), vals.end());
	int64_t ans = 0;
	int x = 0, y = 0;

	auto check_ans = [&](int64_t a, int vx, int vy) {
		if (vx > vy) swap(vx, vy);
		if (make_tuple(a, -vx, -vy) > make_tuple(ans, -x, -y)) {
			ans = a;
			x = vx;
			y = vy;
		}
	};

	int root = 0;
	while (root < N && adj[root].size() <= 2) root++;
	if (root == N) root = 0;
	const int C = int(adj[root].size());

	vector<tuple<int, int, int>> idx(N, {-1, -1, -1});

	auto check_path = [&](const vector<pair<int, int>>& path) {
		vector<pair<pair<int, int>, int>> stk; stk.reserve(path.size()+1);
		stk.push_back({{-2, -2}, -1});
		for (int i = 0; i <= int(path.size()); i++) {
			pair<int, int> v = i == int(path.size()) ? pair<int, int>{-1, -1} : path[i];
			while (stk.back().first >= v) {
				pair<int, int> cur_v = stk.back().first;
				stk.pop_back();
				if (i - 1 - stk.back().second > 0) {
					check_ans(int64_t(cur_v.first) * int64_t(i - 1 - stk.back().second), path[stk.back().second+1].second, path[i-1].second);
				}
			}
			stk.push_back({v, i});
		}
	};

	vector<pair<int, int>> dist(C);

	vector<pair<int, int>> cur_path; cur_path.reserve(N);
	set<pair<int, int>> paths;
	for (int z = 0; z < C; z++) {
		int prv = root;
		int cur = adj[root][z];
		cur_path.clear();
		for (int d = 0; true; d++) {
			cur_path.push_back({V[cur], cur});
			idx[cur] = {z, d, prv};
			if (adj[cur].size() == 1) break;
			else if (adj[cur].size() == 2) {
				assert(adj[cur][0] == prv || adj[cur][1] == prv);
				int nxt = adj[cur][0] ^ adj[cur][1] ^ prv;
				prv = cur, cur = nxt;
			} else assert(false);
		}
		dist[z] = {int(cur_path.size()), -cur_path.back().second};
		check_path(cur_path);
		paths.insert(dist[z]);
	}

	for (auto it : vals) {
		int v0 = root, v1 = root;
		int max_cnt = 1;
		auto p = paths.rbegin();
		if (p != paths.rend()) {
			max_cnt += p->first;
			v0 = -p->second;
			++p;
			if (p != paths.rend()) {
				max_cnt += p->first;
				v1 = -p->second;
			}
		}
		check_ans(int64_t(max_cnt) * int64_t(it.first), v0, v1);
		int cur = it.second;
		if (cur == root) break;
		auto [z, d, prv] = idx[cur];
		if (d < dist[z].first) {
			paths.erase(dist[z]);
			dist[z] = {d, -prv};
			paths.insert(dist[z]);
		}
	}

	cout << ans << '\n' << x+1 << ' ' << y+1 << '\n';
}
