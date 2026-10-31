#define NDEBUG
#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;

		std::vector<std::array<int, 2>> ch(N, std::array<int, 2>{-1, -1});
		std::vector<int> outDeg(N);

		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			int z = 0;
			while (ch[v][z] != -1) {
				z++;
				assert(z < 2);
			}
			ch[v][z] = u;
			outDeg[u]++;
		}

		int root = 0;
		while (outDeg[root]) root++;

		for (int i = 0; i < N; i++) {
			if (ch[i][0] == -1) continue;
			if (ch[i][1] == -1) ch[i][1] = ch[i][0];
			if (ch[i][1] < ch[i][0]) std::swap(ch[i][0], ch[i][1]);
		}

		std::vector<bool> order_fixed(N);
		std::vector<int> depth(N, -1);
		// just sort them either way, doesn't matter
		std::vector<std::vector<int>> verts(N+1);
		depth[root] = 0;
		order_fixed[root] = true;
		verts[0].push_back(root);
		int L = 0;
		for (int d = 0; ch[verts[d][0]][0] != -1; d++) {
			L = d+1;
			verts[d+1].reserve(verts[d].size()+1);
			for (int z = 0; z < 2; z++) {
				for (int m : verts[d]) {
					assert(order_fixed[m]);
					assert(depth[m] == d);
					auto [u, v] = ch[m];
					if (depth[u] == -1) {
						depth[u] = d+1;
						verts[d+1].push_back(u);
					}
					if (depth[v] == -1) {
						depth[v] = d+1;
						verts[d+1].push_back(v);
					}
					if (order_fixed[u] && order_fixed[v]) {
						continue;
					} else if (order_fixed[u]) {
						if (ch[v][0] != ch[u][1]) std::swap(ch[v][0], ch[v][1]);
						assert(ch[v][0] == ch[u][1]);
						order_fixed[v] = true;
					} else if (order_fixed[v]) {
						if (ch[u][1] != ch[v][0]) std::swap(ch[u][0], ch[u][1]);
						assert(ch[u][1] == ch[v][0]);
						order_fixed[u] = true;
					} else {
						assert(ch[u][0] <= ch[u][1]);
						assert(ch[v][0] <= ch[v][1]);
						if (ch[u] == ch[v]) {
							// can't easily disambiguate
							if (z) {
								// this means that u has no other neighbor and v has no other neighbor
								if (u == v) {
									assert(verts[d+1].size() == 1 || ch[u][0] == -1);
									assert(ch[u][0] == ch[u][1]);
									order_fixed[u] = true;
								} else {
									assert(verts[d+1].size() == 2);
									std::swap(ch[v][0], ch[v][1]);
									assert(ch[u][1] == ch[v][0]);
									order_fixed[u] = true;
									order_fixed[v] = true;
								}
							}
						} else {
							// u and v should share exactly one distinct child
							int a = (ch[u][0] == ch[v][0] || ch[u][0] == ch[v][1]) ? ch[u][0] : ch[u][1];
							assert(a == ch[u][0] || a == ch[u][1]);
							assert(a == ch[v][0] || a == ch[v][1]);
							if (a != ch[u][1]) std::swap(ch[u][0], ch[u][1]);
							if (a != ch[v][0]) std::swap(ch[v][0], ch[v][1]);
							assert(ch[u][1] == ch[v][0]);
							order_fixed[u] = order_fixed[v] = true;
						}
					}
				}
			}
		}

		verts.resize(L+1);

		std::vector<std::array<int, 2>> bounds(N);
		for (auto v : verts[L]) bounds[v] = {v, v};
		for (int d = L-1; d >= 0; d--) {
			for (auto v : verts[d]) {
				bounds[v][0] = bounds[ch[v][0]][0];
				bounds[v][1] = bounds[ch[v][1]][1];
			}
		}

		std::vector<int> res; res.reserve(L+1);
		int cur = root;
		for (int i = 0; i <= L; i++) {
			res.push_back(bounds[cur][0]);
			cur = ch[cur][1];
		}
		assert(cur == -1);

		std::string r1; r1.reserve(L+1);
		{
			std::vector<char> leaf_to_char(N, '\0');
			char next_char = 'a';
			for (int v : res) {
				if (!leaf_to_char[v]) {
					assert(next_char <= 'z');
					leaf_to_char[v] = next_char;
					next_char++;
				}
				r1 += leaf_to_char[v];
			}
		}
		std::reverse(res.begin(), res.end());
		std::string r2; r2.reserve(L+1);
		{
			std::vector<char> leaf_to_char(N, '\0');
			char next_char = 'a';
			for (int v : res) {
				if (!leaf_to_char[v]) {
					assert(next_char <= 'z');
					leaf_to_char[v] = next_char;
					next_char++;
				}
				r2 += leaf_to_char[v];
			}
		}
		cout << std::min(r1, r2) << '\n';
	}

	return 0;
}
