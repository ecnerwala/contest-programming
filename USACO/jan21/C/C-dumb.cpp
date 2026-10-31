#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, M, Q; cin >> N >> M >> Q;
	vector<string> G(N);
	for (auto& r : G) {
		r.reserve(M);
		cin >> r;
		assert(int(r.size()) == M);
	}

	vector<int> cc_id(N*M, -1);
	vector<int> pref((N+1)*(M+1));
	{
		auto get_par = [&](int a) {
			while (cc_id[a] >= 0) {
				if (cc_id[cc_id[a]] >= 0) cc_id[a] = cc_id[cc_id[a]];
				a = cc_id[a];
			}
			return a;
		};
		auto merge = [&](int a, int b) {
			a = get_par(a);
			b = get_par(b);
			if (a == b) return false;
			if (cc_id[b] < cc_id[a]) swap(a, b);
			cc_id[a] += cc_id[b];
			cc_id[b] = a;
			return true;
		};

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				if (i && G[i-1][j] == G[i][j]) merge((i-1)*M+j, i*M+j);
				if (j && G[i][j-1] == G[i][j]) merge(i*M+(j-1), i*M+j);
			}
		}

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				pref[(i+1)*(M+1) + (j+1)]
					= pref[(i+1)*(M+1) + j]
					+ pref[i*(M+1) + (j+1)]
					- pref[i*(M+1) + j];
				if (cc_id[i*M+j] < 0) {
					pref[(i+1)*(M+1) + (j+1)]++;
					cc_id[i*M+j] = i*M+j;
				}
			}
		}
	}

	vector<bool> has_cc(N*M);
	// edges are stored by starting location and first direction
	vector<pair<int, int>> edge_id((N+1)*(M+1)*4, {-1,-1});
	auto idx = [&](array<int, 3> e) {
		return (e[0] * (M+1) + e[1]) << 2 | e[2];
	};

	const int dx[4] = {1, 0, -1, 0};
	const int dy[4] = {0, -1, 0, 1};
	auto is_valid_edge = [&](array<int, 3> e) -> bool {
		int x = e[0], y = e[1], d = e[2];
		if (!(0 <= x + dx[d] && x + dx[d] <= N && 0 <= y + dy[d] && y + dy[d] <= M)) return false;
		// we need the 2 colors to be different
		if (dx[d] == 0) {
			if (x == 0 || x == N) return true;
			y = min(y, y + dy[d]);
			return cc_id[(x-1)*M+y] != cc_id[x*M+y];
		} else if (dy[d] == 0) {
			if (y == 0 || y == M) return true;
			x = min(x, x + dx[d]);
			return cc_id[x*M+(y-1)] != cc_id[x*M+y];
		} else assert(false);
	};

	auto get_next_edge = [&](array<int, 3> e) -> array<int, 3> {
		e[0] += dx[e[2]];
		e[1] += dy[e[2]];
		e[2] --, e[2] &= 3;
		while (!is_valid_edge(e)) e[2]++, e[2] &= 3;
		return e;
	};
	auto reverse_edge = [&](array<int, 3> e) -> array<int, 3> {
		e[0] += dx[e[2]];
		e[1] += dy[e[2]];
		e[2] ^= 2;
		return e;
	};

	for (int i = 0; i <= N; i++) {
		for (int j = 0; j <= M; j++) {
			for (int d = 0; d < 4; d++) {
				array<int, 3> cur{i,j,d};
				if (!is_valid_edge(cur)) continue;
				if (edge_id[idx(cur)].first != -1) continue;
				int cc_num = idx(cur);
				int cc_sz = 0;
				//cerr << "starting cycle" << '\n';
				while (edge_id[idx(cur)].first == -1) {
					//cerr << cur[0] << ' ' << cur[1] << ' ' << cur[2] << '\n';
					edge_id[idx(cur)] = {cc_num, cc_sz++};
					cur = get_next_edge(cur);
				}
			}
		}
	}

	for (int q = 0; q < Q; q++) {
		int xlo, ylo, xhi, yhi; cin >> xlo >> ylo >> xhi >> yhi;
		xlo--, ylo--;
		int cnt_interior = pref[xhi*(M+1) + yhi] - pref[xlo*(M+1) + yhi] - pref[xhi*(M+1) + ylo] + pref[xlo*(M+1) + ylo];
		xhi--, yhi--;

		{ // subtract off cc's not completely contained
			{
				auto check_node = [&](int x, int y) {
					int c = cc_id[x * M + y];
					if (xlo <= c/M && c/M <= xhi && ylo <= c%M && c%M <= yhi) {
						has_cc[c] = true;
					}
				};
				for (int y = ylo; y <= yhi; y++) {
					check_node(xlo, y);
					check_node(xhi, y);
				}
				for (int x = xlo; x <= xhi; x++) {
					check_node(x, ylo);
					check_node(x, yhi);
				}
			}
			{
				auto check_node = [&](int x, int y) {
					int c = cc_id[x * M + y];
					if (xlo <= c/M && c/M <= xhi && ylo <= c%M && c%M <= yhi) {
						cnt_interior -= has_cc[c];
						has_cc[c] = false;
					}
				};
				for (int y = ylo; y <= yhi; y++) {
					check_node(xlo, y);
					check_node(xhi, y);
				}
				for (int x = xlo; x <= xhi; x++) {
					check_node(x, ylo);
					check_node(x, yhi);
				}
			}
		}

		int cnt_edges = 0;
		{ // Now, handle the edges
			// First, let's collect the cycle
			vector<pair<int, int>> outer_edges; outer_edges.reserve(2 * (2 * N + 2 * M));
			for (int x = xlo+1; x <= xhi; x++) {
				array<int, 3> cur{x, ylo, 1};
				if (is_valid_edge(cur)) {
					outer_edges.push_back(edge_id[idx(reverse_edge(cur))]);
					outer_edges.push_back(edge_id[idx(cur)]);
				}
			}

			if (outer_edges.empty()) cnt_edges = 1;
		}

		int ans = cnt_interior + cnt_edges;
		cout << ans << '\n';
	}

	return 0;
}
