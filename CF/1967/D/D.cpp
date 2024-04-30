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

namespace ecnerwala {

using std::swap;

struct level_ancestor {
	int N;
	std::vector<int> preorder;
	std::vector<int> idx;
	std::vector<std::pair<int, int>> heavyPar; // heavy parent, distance
	level_ancestor() : N(0) {}

	level_ancestor(const std::vector<int>& par) : N(int(par.size())), preorder(N), idx(N), heavyPar(N) {
		std::vector<std::vector<int>> ch(N);
		for (int i = 0; i < N; i++) {
			if (par[i] != -1) ch[par[i]].push_back(i);
		}
		std::vector<int> sz(N);
		int nxt_idx = 0;
		for (int i = 0; i < N; i++) {
			if (par[i] == -1) {
				std::y_combinator([&](auto self, int cur) -> void {
					sz[cur] = 1;
					for (int nxt : ch[cur]) {
						self(nxt);
						sz[cur] += sz[nxt];
					}
					if (!ch[cur].empty()) {
						auto mit = max_element(ch[cur].begin(), ch[cur].end(), [&](int a, int b) { return sz[a] < sz[b]; });
						swap(*ch[cur].begin(), *mit);
					}
				})(i);
				std::y_combinator([&](auto self, int cur, int isRoot = true) -> void {
					preorder[idx[cur] = nxt_idx++] = cur;
					if (isRoot) {
						heavyPar[idx[cur]] = {par[cur] == -1 ? -1 : idx[par[cur]], 1};
					} else {
						assert(idx[par[cur]] == idx[cur]-1);
						heavyPar[idx[cur]] = heavyPar[idx[cur]-1];
						heavyPar[idx[cur]].second++;
					}
					bool chRoot = false;
					for (int nxt : ch[cur]) {
						self(nxt, chRoot);
						chRoot = true;
					}
				})(i);
			}
		}
	}

	int get_ancestor(int a, int k) const {
		assert(k >= 0);
		a = idx[a];
		while (a != -1 && k) {
			if (k >= heavyPar[a].second) {
				k -= heavyPar[a].second;
				assert(heavyPar[a].first <= a - heavyPar[a].second);
				a = heavyPar[a].first;
			} else {
				a -= k;
				k = 0;
			}
		}
		if (a == -1) return -1;
		else return preorder[a];
	}

	int lca(int a, int b) const {
		a = idx[a], b = idx[b];
		while (true) {
			if (a > b) swap(a, b);
			assert(a <= b);
			if (a > b - heavyPar[b].second) {
				return preorder[a];
			}
			b = heavyPar[b].first;
			if (b == -1) return -1;
		}
	}

	int dist(int a, int b) const {
		a = idx[a], b = idx[b];
		int res = 0;
		while (true) {
			if (a > b) swap(a, b);
			assert(a <= b);
			if (a > b - heavyPar[b].second) {
				res += b - a;
				break;
			}
			res += heavyPar[b].second;
			b = heavyPar[b].first;
			if (b == -1) return -1;
		}
		return res;
	}
};

} // namespace ecnerwala

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	//int T; cin >> T;
	int T = 1;
	while (T--) {
		/*
		int L, M; cin >> L >> M;
		std::vector<int> A(L); for (auto& a : A) { cin >> a; a--; }
		std::vector<int> B(M); for (auto& b : B) { cin >> b; b--; }
		*/
		int L = 1e6;
		int M = 1e6;
		std::vector<int> A(L); for (int i = 0; i < L; i++) { A[i] = (i % M); }
		std::vector<int> B(M); for (int i = 0; i < M; i++) { B[i] = (i+1) % M; }
		//cerr << "Case" << '\n';
		//cerr << L << ' ' << M << '\n';
		//for (auto& b : B) { cerr << b << ' '; } cerr << '\n';

		std::vector<int> root_par(M, -1);
		std::vector<std::vector<int>> ch(M);
		std::vector<int> tree_par(M, -1);
		{
			std::vector<int> par(M, -1);
			auto get_par = [&](int a) -> int {
				while (par[a] >= 0) {
					if (par[par[a]] >= 0) par[a] = par[par[a]];
					a = par[a];
				}
				return a;
			};
			auto merge = [&](int a, int b) -> bool {
				a = get_par(a), b = get_par(b);
				if (a == b) return false;
				if (par[a] > par[b]) std::swap(a, b);
				par[a] += par[b];
				par[b] = a;
				return true;
			};
			for (int i = 0; i < M; i++) {
				if (!merge(i, B[i])) {
					root_par[i] = B[i];
				} else {
					tree_par[i] = B[i];
					ch[B[i]].push_back(i);
				}
			}
		}
		//for (auto& b : tree_par) { cerr << b << ' '; } cerr << '\n';
		//for (auto& b : root_par) { cerr << b << ' '; } cerr << '\n';

		//ecnerwala::level_ancestor la(tree_par);

		struct seg_tree_node {
			std::array<int, 2> c{-1, -1};
			int cnt = 0;
		};
		std::vector<seg_tree_node> nodes;
		nodes.reserve(2 * M + 22 * M);
		int empty_tree = std::y_combinator([&](auto self, int lo, int hi) -> int {
			assert(hi > lo);
			int l, r;
			if (hi - lo == 1) {
				l = -1, r = -1;
			} else {
				int md = (lo + hi) / 2;
				l = self(lo, md);
				r = self(md, hi);
			}
			int idx = int(nodes.size());
			nodes.push_back(seg_tree_node{{l, r}, 0});
			return idx;
		})(0, M);

		cerr << nodes.size() << ' ' << nodes.capacity() << '\n';
		cerr << nodes.capacity() * sizeof(seg_tree_node) << '\n';
		std::vector<int> depth(M, -1);
		//std::vector<std::vector<int>> root_path(M);
		std::vector<int> segs(M, -1);
		for (int rt = 0; rt < M; rt++) {
			if (root_par[rt] == -1) continue;
			depth[rt] = 1;
			std::y_combinator([&](auto self, int cur, int cur_tree) -> void {
				cur_tree = segs[cur] = std::y_combinator([&](auto self, int s, int lo, int hi) -> int {
					assert(lo <= cur && cur < hi);
					seg_tree_node res = nodes[s];
					res.cnt++;
					if (hi - lo > 1) {
						int md = (lo + hi) / 2;
						if (cur < md) {
							res.c[0] = self(res.c[0], lo, md);
						} else {
							res.c[1] = self(res.c[1], md, hi);
						}
					}
					int idx = int(nodes.size());
					nodes.push_back(res);
					return idx;
				})(cur_tree, 0, M);
				for (int nxt : ch[cur]) {
					depth[nxt] = depth[cur] + 1;
					self(nxt, cur_tree);
				}
			})(rt, empty_tree);
			/*
			root_path[rt].reserve(depth[root_par[rt]]);
			for (int cur = root_par[rt]; cur != -1; cur = tree_par[cur]) {
				root_path[rt].push_back(cur);
				if (cur == rt) break;
			}
			*/
		}
		cerr << nodes.size() << ' ' << nodes.capacity() << '\n';
		cerr << nodes.size() * sizeof(seg_tree_node) << '\n';
		break;

		/*
		auto query_seg = std::y_combinator([&](auto self, int s_big, int s_small, int v_lo, int lo, int hi) -> int {
			assert(nodes[s_big].cnt >= nodes[s_small].cnt);
			if (nodes[s_big].cnt == nodes[s_small].cnt) {
				return M;
			}
			if (hi <= v_lo) {
				return M;
			}
			if (hi - lo == 1) {
				assert(lo >= v_lo);
				return lo;
			}
			int md = (lo + hi) / 2;
			int r1 = self(nodes[s_big].c[0], nodes[s_small].c[0], v_lo, lo, md);
			if (r1 != M) return r1;
			return self(nodes[s_big].c[1], nodes[s_small].c[1], v_lo, md, hi);
		});

		auto query = [&](int vert, int dist, int v_lo) -> int {
			// # valid verts is dist + 1
			dist++;
			if (dist < depth[vert]) {
				int anc = la.get_ancestor(vert, dist);
				return query_seg(segs[vert], segs[anc], v_lo, 0, M);
			} else {
				int rt = la.get_ancestor(vert, depth[vert] - 1);
				int a1 = query_seg(segs[vert], empty_tree, v_lo, 0, M);
				int num_extra = std::min(dist - depth[vert], depth[root_par[rt]] - 1);
				int a2 = query_seg(segs[root_par[rt]], segs[root_path[rt][num_extra]], v_lo, 0, M);
				return std::min(a1, a2);
			}
		};

		int mi = -1;
		int ma = M;
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			int cur = 0;
			for (auto a : A) {
				cur = query(a, md, cur);
			}
			if (cur == M) {
				mi = md;
			} else {
				ma = md;
			}
		}
		if (ma == M) {
			// M-1 steps lets you get everything, so M is never necessary
			cout << -1 << '\n';
		} else {
			cout << ma << '\n';
		}
		*/
	}

	return 0;
}
