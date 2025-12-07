#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int L; cin >> L;
	std::vector<std::string> G(4*L+1);
	for (auto& s : G) {
		cin >> s;
		assert(int(s.size()) == 4*L+1);
	}

	int ccs = 0;

	// try to add a new edge, then follow the things in the tree
	// total size of the graph
	const int V = L * L + (L+1) * (L+1);
	const int E = L * L * 4;
	std::vector<std::array<int, 2>> edges(E);
	std::vector<char> legal(L*L*4, 0);
	std::vector<int> cap(L*L);
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			int v = i*L+j;
			edges[4*v+0] = {v, L*L + i * (L+1) + j};
			edges[4*v+1] = {v, L*L + i * (L+1) + (j+1)};
			edges[4*v+2] = {v, L*L + (i+1) * (L+1) + j};
			edges[4*v+3] = {v, L*L + (i+1) * (L+1) + (j+1)};
			char c = G[4*i+2][4*j+2];
			if (c == '.') {
				continue;
			} else if ('1' <= c && c <= '4') {
				ccs++;
				cap[v] = c - '0';
				legal[4*v+0] = G[4*i][4*j] == 'A';
				legal[4*v+1] = G[4*i][4*(j+1)] == 'A';
				legal[4*v+2] = G[4*(i+1)][4*j] == 'A';
				legal[4*v+3] = G[4*(i+1)][4*(j+1)] == 'A';
			} else assert(false);
		}
	}
	for (int i = 0; i <= L; i++) {
		for (int j = 0; j <= L; j++) {
			if (G[4*i][4*j] == 'A') ccs++;
		}
	}

	std::vector<std::vector<std::pair<int, int>>> adj(V);
	for (auto& a : adj) a.reserve(4);
	for (int e = 0; e < E; e++) {
		if (legal[e]) {
			adj[edges[e][0]].push_back({edges[e][1], e});
			adj[edges[e][1]].push_back({edges[e][0], e});
		}
	}

	std::vector<char> alive(E);
	auto fix_G = [&]() -> void {
		for (int i = 0; i < L; i++) {
			for (int j = 0; j < L; j++) {
				int v = i*L+j;
				G[4*i+1][4*j+1] = alive[4*v+0] ? '\\' : '.';
				G[4*i+1][4*j+3] = alive[4*v+1] ? '/' : '.';
				G[4*i+3][4*j+1] = alive[4*v+2] ? '/' : '.';
				G[4*i+3][4*j+3] = alive[4*v+3] ? '\\' : '.';
			}
		}
	};
	auto dump_alive = [&]() -> void {
		fix_G();
		for (const auto& s : G) cerr << s << '\n';
		cerr << '\n';
	};

	// parent vert, parent edge
	std::vector<std::pair<int, int>> tree_par(V);
	std::vector<int> depth(V, 0);
	std::vector<int> cc(V);
	std::iota(cc.begin(), cc.end(), 0);
	std::vector<int> par(V, -1);
	std::vector<int> label(V);
	std::iota(label.begin(), label.end(), 0);
	auto build_subtree_data = [&](int v) -> void {
		depth[v] = 0;
		tree_par[v] = {-1, -1};
		[&](this auto&& self, int cur, int prv) -> void {
			par[cur] = -1;
			label[cur] = cur;
			cc[cur] = v;
			for (auto [nxt, e] : adj[cur]) {
				if (!alive[e]) continue;
				if (nxt == prv) continue;
				//assert(depth[nxt] == -1);
				depth[nxt] = depth[cur] + 1;
				tree_par[nxt] = {cur, e};
				self(nxt, cur);
			}
		}(v, -1);
	};

	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};
	std::vector<int> q; q.reserve(E);
	std::vector<int> prv(E, -1);
	auto saturate = [&](int palace) {
		while (cap[palace] > 0) {
			int cur_cc = cc[palace];
			//cerr << "trying " << palace << '\n';
			// try to insert e
			int found = -1;
			for (auto [_, ne] : adj[palace]) {
				if (!alive[ne]) {
					prv[ne] = -2;
					q.push_back(ne);
					assert(cc[edges[ne][0]] == cur_cc);
					if (cc[edges[ne][1]] != cur_cc) {
						found = ne;
						break;
					}
				}
			}
			for (int z = 0; found == -1 && z < int(q.size()); z++) {
				int e = q[z];
				if (alive[e]) {
					// just substitute it
					int bucket = e / 4;
					assert(cc[bucket] == cur_cc);
					for (int ne = bucket * 4; ne < bucket * 4 + 4; ne++) {
						if (!alive[ne]) {
							if (prv[ne] == -1) {
								prv[ne] = e;
								q.push_back(ne);
								assert(edges[ne][0] == bucket);
								if (cc[edges[ne][1]] != cur_cc) {
									found = ne;
									break;
								}
							}
						}
					}
				} else {
					auto [u, v] = edges[e];
					assert(cc[u] == cur_cc);
					assert(cc[v] == cur_cc);
					// look through the tree between u and v
					int pu = get_par(u);
					u = label[pu];
					int pv = get_par(v);
					v = label[pv];
					while (u != v) {
						//cerr << u << ' ' << v << ' ' << pu << ' ' << pv << '\n';
						if (depth[u] < depth[v]) {
							std::swap(u, v);
							std::swap(pu, pv);
						}
						assert(depth[u] >= depth[v]);
						assert(par[pu] < 0);
						assert(label[pu] == u);
						{
							int ne = tree_par[u].second;
							assert(ne != -1);
							assert(alive[ne]);
							assert(prv[ne] == -1);
							prv[ne] = e;
							q.push_back(ne);
						}
						int npu = get_par(tree_par[u].first);
						int nu = label[npu];
						label[pu] = nu;
						if (par[pu] > par[npu]) std::swap(pu, npu);
						par[pu] += par[npu];
						par[npu] = pu;
						u = nu;
						// if we get to the top, pv could be wrong, but whatever
					}
				}
			}
			if (found == -1) {
				//cerr << "didn't find in " << palace/L << ' ' << palace%L << '\n';
				cap[palace] = 0;
			} else {
				//cerr << "found path" << '\n';
				ccs--;
				cap[palace]--;
				assert(!alive[found]);
				for (int cur = found; cur != -2; cur = prv[cur]) {
					alive[cur] ^= 1;
				}
				//dump_alive();
			}
			build_subtree_data(palace);

			for (auto v : q) prv[v] = -1;
			q.clear();
		}
	};

	[&](this auto&& self, int xlo, int xhi, int ylo, int yhi) -> void {
		if (yhi - ylo == 1) return;
		if (xhi - xlo == 1) return;
		if (yhi - ylo > xhi - xlo) {
			int y = ylo + (yhi - ylo) / 2;
			self(xlo, xhi, ylo, y);
			self(xlo, xhi, y, yhi);
			for (int x = xlo+1; x <= xhi-1; x++) saturate(x * L + y);
		} else {
			int x = xlo + (xhi - xlo) / 2;
			self(xlo, x, ylo, yhi);
			self(x, xhi, ylo, yhi);
			for (int y = ylo+1; y <= yhi-1; y++) saturate(x * L + y);
		}
	}(-1, L, -1, L);

	fix_G();
	cout << ccs << '\n';
	for (const auto& s : G) cout << s << '\n';
	return 0;
}
