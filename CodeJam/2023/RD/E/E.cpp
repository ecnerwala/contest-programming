#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int C, L; cin >> C >> L;
		//cerr << "Solving " << C << ' ' << L << '\n';

		assert(L >= C);
		{
			std::vector<std::array<int, 2>> edges; edges.reserve(L);
			for (int i = 0; i < C; i++) {
				edges.push_back({i, i+1 == C ? 0 : i+1});
			}
			for (int i = 1; int(edges.size()) < L && i < C; i++) {
				for (int j = i-2; int(edges.size()) < L && j >= 0; j--) {
					edges.push_back({i, j});
				}
			}

			assert(int(edges.size()) == L);
			for (auto [x, y] : edges) {
				assert(x != y);
				cout << x+1 << ' ' << y+1 << '\n';
			}
			cout << flush;
		}

		{
			std::vector<std::array<int, 2>> edges(L);
			std::vector<std::vector<int>> adj(C);
			for (int i = 0; i < L; i++) {
				int x, y; cin >> x >> y; x--, y--;
				edges[i] = {x, y};
				adj[x].push_back(y);
				adj[y].push_back(x);
			}

			std::vector<int> deg(C);
			for (int i = 0; i < C; i++) deg[i] = int(adj[i].size());

			std::vector<int> cyc; cyc.reserve(C);

			int min_deg = *std::min_element(deg.begin(), deg.end());
			int max_deg = *std::max_element(deg.begin(), deg.end());
			if (min_deg == 2) {
				int cur = -1, prv = -1;
				if (L == C) {
					cur = edges[0][0], prv = edges[0][1];
				} else if (L == C + 1 && C > 4) {
					assert(max_deg == 3);
					for (int i = 0; i < C; i++) {
						if (deg[i] == 2) {
							if (deg[adj[i][0]] == 3 && deg[adj[i][1]] == 3) {
								continue;
							} else if (deg[adj[i][0]] == 3) {
								prv = adj[i][0];
								cur = i;
								break;
							} else if (deg[adj[i][1]] == 3) {
								prv = adj[i][1];
								cur = i;
								break;
							}
						}
					}
				} else {
					for (auto [x, y] : edges) {
						if (deg[x] == 2 && deg[y] != 2) {
							cur = x;
							prv = y;
							break;
						} else if (deg[y] == 2 && deg[x] != 2) {
							cur = y;
							prv = x;
							break;
						}
					}
				}
				assert(cur != -1 && prv != -1);
				cyc.push_back(prv);
				cyc.push_back(cur);
				while (deg[cur] == 2 && int(cyc.size()) < C) {
					assert(adj[cur].size() == 2);
					int nxt = adj[cur][0] ^ adj[cur][1] ^ prv;
					cyc.push_back(nxt);
					prv = cur;
					cur = nxt;
				}
				if (int(cyc.size()) < C) {
					assert(L > C);
					std::vector<bool> vis(C);
					for (int v : cyc) vis[v] = true;
					std::vector<std::pair<int, int>> extras;
					for (int c = 0; c < C; c++) {
						if (!vis[c]) extras.push_back({deg[c], c});
					}
					std::sort(extras.begin(), extras.end());

					if (deg[cyc.front()] > deg[cyc.back()]) {
						std::reverse(cyc.begin(), cyc.end());
					}
					assert(deg[cyc.front()] <= deg[cyc.back()]);
					for (auto [d, v] : extras) cyc.push_back(v);
				}
			} else {
				cyc.resize(C);
				std::iota(cyc.begin(), cyc.end(), 0);
				std::sort(cyc.begin(), cyc.end(), [&](int i, int j) { return deg[i] < deg[j]; });
				std::swap(cyc.front(), cyc.back());
			}

			//cerr << "given edges" << '\n';
			//for (auto [x, y] : edges) { cerr << x << ' ' << y << '\n'; }
			//cerr << "cycle" << '\n';
			//for (auto v : cyc) { cerr << v << ' '; } cerr << '\n';

			for (int i = 0; i < C; i++) {
				int x = cyc[i], y = cyc[(i+1)%C];
				if (x > y) std::swap(x, y);
				assert(std::binary_search(edges.begin(), edges.end(), std::array<int, 2>{x, y}));
			}

			assert(int(cyc.size()) == C);
			for (int i = 0; i < C; i++) {
				cout << cyc[i]+1 << " \n"[i+1==C];
			}
			cout << flush;
		}


	}

	return 0;
}
