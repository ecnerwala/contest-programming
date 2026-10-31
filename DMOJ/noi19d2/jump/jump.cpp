#include<bits/stdc++.h>
#include<bits/extc++.h>

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
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, W, H; cin >> N >> M >> W >> H;
	vector<array<int, 2>> pts(N);
	for (int i = 0; i < N; i++) {
		cin >> pts[i][0] >> pts[i][1];
		pts[i][0]--, pts[i][1]--;
	}

	vector<vector<int>> row_coords(2*W);
	{
		vector<array<int, 2>> y_pts = pts;
		sort(y_pts.begin(), y_pts.end(), [](const auto& a, const auto& b) { return a[1] < b[1]; });
		for (const auto& p : y_pts) {
			for (int a = W+p[0]; a; a /= 2) {
				if (row_coords[a].empty() || row_coords[a].back() != p[1]) {
					row_coords[a].push_back(p[1]);
				}
			}
		}
	}

	int V = N;

	vector<int> seg_offset(2*W);
	for (int a = 1; a < 2*W; a++) {
		seg_offset[a] = V-1;
		V += max(0, int(row_coords[a].size()) * 2 - 1);
	}

	vector<vector<int>> adj(V);

	for (int a = 1; a < 2*W; a++) {
		for (int c = 2; c < 2*int(row_coords[a].size()); c++) {
			adj[seg_offset[a] + c/2].push_back(seg_offset[a] + c);
		}
	}

	for (int i = 0; i < N; i++) {
		for (int a = W+pts[i][0]; a; a /= 2) {
			int y = int(lower_bound(row_coords[a].begin(), row_coords[a].end(), pts[i][1]) - row_coords[a].begin());
			adj[seg_offset[a] + int(row_coords[a].size()) + y].push_back(i);
		}
	}

	struct rect {
		int xlo, xhi, ylo, yhi;
	};
	vector<rect> portals(M);
	vector<vector<pair<int, int>>> outPortals(N);
	for (int e = 0; e < M; e++) {
		int p; cin >> p; p--;
		int t; cin >> t;
		int xlo, xhi, ylo, yhi; cin >> xlo >> xhi >> ylo >> yhi; xlo--, ylo--;
		outPortals[p].emplace_back(e, t);
		portals[e] = {xlo, xhi, ylo, yhi};
	}

	vector<int> dist(N);
	vector<bool> vis(V);

	int cur_dist = 0;

	stack<int> fast_q;
	fast_q.push(0);
	int num_vis = 0;
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> edge_q;

	auto mark_node = std::y_combinator([&](auto self, int cur) {
		if (vis[cur]) return;
		vis[cur] = true;
		if (cur < N) {
			dist[cur] = cur_dist;
			num_vis++;
			if (num_vis == N) {
				for (int i = 1; i < N; i++) {
					cout << dist[i] << '\n';
				}
				exit(0);
			}
			for (auto it : outPortals[cur]) {
				edge_q.emplace(cur_dist + it.second, it.first);
			}
		} else {
			for (int nxt : adj[cur]) self(nxt);
		}
	});

	mark_node(0);

	while (!edge_q.empty()) {
		cur_dist = edge_q.top().first;
		int e = edge_q.top().second;
		edge_q.pop();

		int xlo = portals[e].xlo;
		int xhi = portals[e].xhi;
		int ylo = portals[e].ylo;
		int yhi = portals[e].yhi;

		auto do_inner = [&](int a) {
			int lo = int(lower_bound(row_coords[a].begin(), row_coords[a].end(), ylo) - row_coords[a].begin());
			int hi = int(lower_bound(row_coords[a].begin(), row_coords[a].end(), yhi) - row_coords[a].begin());
			for (int c = int(row_coords[a].size())+lo, d = int(row_coords[a].size())+hi; c < d; c >>= 1, d >>= 1) {
				if (c & 1) mark_node(seg_offset[a] + (c++));
				if (d & 1) mark_node(seg_offset[a] + (--d));
			}
		};
		for (int a = W+xlo, b = W+xhi; a < b; a >>= 1, b >>= 1) {
			if (a & 1) do_inner(a++);
			if (b & 1) do_inner(--b);
		}
	}

	assert(false);

	return 0;
}
