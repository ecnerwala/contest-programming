#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	constexpr bool TESTING = false;
	std::mt19937 mt(48);
	for (int case_num = 0; case_num < (TESTING ? 10000 : 1); case_num++) {
		//cerr << "Case #" << case_num << '\n';

		int N, Q;
		if (TESTING) {
			N = std::uniform_int_distribution<int>(2, 50)(mt);
			Q = std::uniform_int_distribution<int>(1, 20)(mt);
			cerr << N << ' ' << Q << '\n';
		} else {
			cin >> N >> Q;
		}
		std::vector<int> fa(N+1);
		std::vector<int> fa_slow(N+1);
		for (int i = 2; i <= N; i++) {
			if (TESTING) {
				fa[i] = std::uniform_int_distribution<int>(1, i-1)(mt);
				fa_slow[i] = fa[i];
				cerr << fa[i] << " \n"[i==N];
			} else {
				cin >> fa[i];
			}
		}

		struct lct_node {
			// parent
			int p = 0;
			// children
			std::array<int, 2> c = {0, 0};
			int sz = 0;

			// TODO: more metadata specific to the root
			int r = 0;
			int last_vis = 0;
		};

		std::vector<lct_node> tree(N+1);
		auto d = [&](int i) -> bool {
			return i == tree[tree[i].p].c[1];
		};
		auto update = [&](int i) -> void {
			tree[i].sz = tree[tree[i].c[0]].sz + 1 + tree[tree[i].c[1]].sz;
		};
		// doesn't run any update
		auto rot = [&](int i) -> void {
			int p = tree[i].p;
			assert(p != i);
			assert(p != 0);
			bool x = d(i);
			assert(tree[p].c[x] == i);
			int ch = tree[i].c[!x];

			tree[p].c[x] = ch;
			if (ch) tree[ch].p = p;

			int gp = tree[p].p;
			if (gp) tree[gp].c[d(p)] = i;
			tree[i].p = gp;

			// TODO: swap the metadata like this
			// TODO: is it faster unconditionally?
			if (!gp) {
				std::swap(tree[i].r, tree[p].r);
				std::swap(tree[i].last_vis, tree[p].last_vis);
			}

			tree[i].c[!x] = p;
			tree[p].p = i;

			update(p);
		};
		auto splay = [&](int i) -> void {
			while (tree[i].p) {
				int p = tree[i].p;
				if (tree[p].p) {
					if (d(p) == d(i)) rot(p);
					else rot(i);
				}
				rot(i);
			}
			update(i);
		};

		std::set<int> alive;
		const int FA_BLOCK = 300;
		std::vector<int64_t> fa_block(N / FA_BLOCK + 1);

		tree[1].r = -1;

		for (int q = 1; q <= Q; q++) {
			//cerr << "query " << q << '\n';
			int op;
			if (TESTING) {
				op = std::uniform_int_distribution<int>(1, 2)(mt);
			} else {
				cin >> op;
			}
			if (op == 1) {
				int l, r, delta;
				if (TESTING) {
					l = std::uniform_int_distribution<int>(2, N)(mt);
					r = std::uniform_int_distribution<int>(l, N)(mt);
					delta = std::uniform_int_distribution<int>(1, N)(mt);
					for (int i = l; i <= r; i++) fa_slow[i] = std::max(1, fa_slow[i] - delta);
					cerr << "op 1 " << l << '-' << r << ' ' << '-' << delta << '\n';
				} else {
					cin >> l >> r >> delta;
				}
				for (auto it = alive.lower_bound(l); it != alive.end() && *it <= r; it = alive.erase(it)) {
					int cur = *it;
					//cerr << "delete vert " << cur << '\n';
					splay(cur);

					assert(tree[cur].p == 0);

					if (tree[cur].c[1]) {
						int ch = tree[cur].c[1];
						tree[ch].r = cur;
						tree[ch].p = 0;
						tree[ch].last_vis = 0;

						tree[cur].c[1] = 0;
					}
					if (tree[cur].c[0]) {
						int nxt = tree[cur].c[0];
						tree[nxt].r = tree[cur].r;
						tree[nxt].p = 0;
						tree[nxt].last_vis = 0;

						tree[cur].c[0] = 0;
					}

					tree[cur].r = 0;
					tree[cur].last_vis = 0;

					update(cur);
				}
				// go change all the parents
				for (; r >= l && r % FA_BLOCK != FA_BLOCK - 1; r--) {
					fa[r] = std::max(1, fa[r] - delta);
				}
				for (; l <= r && l % FA_BLOCK; l++) {
					fa[l] = std::max(1, fa[l] - delta);
				}
				if (l <= r) {
					for (int i = l / FA_BLOCK; i <= r / FA_BLOCK; i++) {
						fa_block[i] -= delta;
					}
				}
			} else if (op == 2) {
				int k;
				if (TESTING) {
					k = std::uniform_int_distribution<int>(1, N)(mt);
				} else {
					cin >> k;
				}
				std::vector<int> B(k);
				if (TESTING) {
					for (auto& b : B) { b = std::uniform_int_distribution<int>(1, N)(mt); }
					cerr << "op 2 " << k << '\n';
				} else {
					for (auto& b : B) { cin >> b; }
				}

				int lca_depth = N+2;
				int tot_verts = 0;
				assert(!B.empty());
				for (auto b : B) {
					//cerr << "  vertex " << b << '\n';
					int cur = b;
					int nxt = 0;
					while (true) {
						//cerr << "   cur " << cur << ' ' << "nxt " << nxt << '\n';
						splay(cur);
						if (tree[cur].last_vis == q) {
							// we don't need to do anything, unless we're looking for the lca
							if (nxt) {
								tree[nxt].last_vis = q;
								tot_verts += tree[nxt].sz;
							}
							if (tree[cur].r == -1) {
								lca_depth = std::min(lca_depth, tree[tree[cur].c[0]].sz);
							}
							break;
						} else {
							// splice this guy in
							if (tree[cur].c[1]) {
								int ch = tree[cur].c[1];
								tree[ch].r = cur;
								tree[ch].p = 0;
								tree[ch].last_vis = 0;

								tree[cur].c[1] = 0;
							}

							tree[cur].c[1] = nxt;
							if (nxt) {
								tree[nxt].p = cur;
								tree[nxt].r = 0;
								tree[nxt].last_vis = 0;
							}

							update(cur);

							if (tree[cur].r == 0) {
								// look up the correct parent, and insert it into the tree
								assert(tree[cur].c[0] == 0);
								assert(cur >= 2);
								int pa = int(std::max<int64_t>(1, fa[cur] + fa_block[cur / FA_BLOCK]));
								//cerr << "reinsert edge " << cur << ' ' << pa << '\n';
								if (pa > 1) {
									// TODO: optimize this as well?
									// NOTE: Bug here, was alive.insert(pa)
									alive.insert(cur);
								}
								tree[cur].r = pa;
							}

							if (tree[cur].r == -1) {
								tree[cur].last_vis = q;
								assert(tot_verts == 0);
								// NB: Bug was here, used to be tree[tree[cur].c[0]].sz, definitely not findable lol
								lca_depth = tree[cur].sz - 1;
								tot_verts += tree[cur].sz;
								break;
							}
							nxt = cur;
							cur = tree[cur].r;
						}
					}

					// splay b for safety
					splay(b);
				}

				if (TESTING) {
					std::vector<int> vis_cnt(N+1);
					for (int b : B) {
						for (int cur = b; ; cur = fa_slow[cur]) {
							vis_cnt[cur]++;
							if (cur == 1) break;
						}
					}
					int lca_depth_slow = 0;
					for (int i = 2; i <= N; i++) if (vis_cnt[i] == vis_cnt[1]) lca_depth_slow++;
					int tot_verts_slow = 0;
					for (int i = 1; i <= N; i++) if (vis_cnt[i]) tot_verts_slow++;
					assert(lca_depth == lca_depth_slow);
					assert(tot_verts == tot_verts_slow);
				}
				//cerr << "tot_verts " << tot_verts << '\n';
				//cerr << "lca_depth " << lca_depth << '\n';
				if (TESTING) {
				} else {
					cout << tot_verts - lca_depth << '\n';
				}
			} else assert(false);
		}
	}
	return 0;
}
