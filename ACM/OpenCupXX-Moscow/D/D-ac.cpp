#define NDEBUG
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

struct range_data {
	int max_val;
	int max_val_2;

	range_data() : max_val(0), max_val_2(-1) {}

	void reset() {
		max_val = 0;
		max_val_2 = -1;
		assert(max_val > max_val_2);
	}

	void set_val(int v) {
		assert(v);
		max_val = v;
		max_val_2 = 0;
		assert(max_val > max_val_2);
	}

	void update_ch(const range_data& a) {
		if (a.max_val > max_val) {
			max_val_2 = max(max_val, a.max_val_2);
			max_val = a.max_val;
		} else if (a.max_val == max_val) {
			max_val_2 = max(max_val_2, a.max_val_2);
		} else {
			max_val_2 = max(max_val_2, a.max_val);
		}
		assert(max_val > max_val_2);
		assert(max_val_2 >= a.max_val_2);
	}

	bool is_max_ch(const range_data& a) {
		assert(max_val > max_val_2);
		assert(a.max_val >= max_val || a.max_val <= max_val_2);
		assert(max_val_2 >= a.max_val_2);
		return a.max_val >= max_val;
	}

	bool downdate_ch(range_data& a) {
		if (is_max_ch(a)) {
			assert(a.max_val >= max_val);
			//if (max_val != a.max_val) { cerr << "  " << a.max_val << "->" << max_val << '\n'; }
			a.max_val = max_val;
			return true;
		}
		return false;
	}
};

const int INF = int(1e9) + 200;

struct seg_node {
	range_data v1;
	range_data v2[2];
	range_data v3[2][2];

	void activate_leaf() {
		deactivate_leaf();
		v1.set_val(INF);
		v2[1].set_val(INF);
		v3[1][1].set_val(INF);
	}

	void deactivate_leaf() {
		v1.reset();
		for (int z2 = 0; z2 < 2; z2++) {
			v2[z2].reset();
		}
		for (int z2 = 0; z2 < 2; z2++) {
			for (int z3 = 0; z3 < 2; z3++) {
				v3[z2][z3].reset();
			}
		}
	}

	void downdate_ch(seg_node& a) {
		bool is_max_1 = v1.downdate_ch(a.v1);
		for (int z2 = 0; z2 < 2; z2++) {
			bool is_max_2 = v2[z2 && is_max_1].downdate_ch(a.v2[z2]);
			for (int z3 = 0; z3 < 2; z3++) {
				v3[z2 && is_max_1][z3 && is_max_2].downdate_ch(a.v3[z2][z3]);
			}
		}
	}

	void update_ch(const seg_node& a, const seg_node& b) {
		v1.reset();
		v1.update_ch(a.v1);
		v1.update_ch(b.v1);

		for (int z2 = 0; z2 < 2; z2++) {
			v2[z2].reset();
		}
		for (int z2 = 0; z2 < 2; z2++) {
			v2[z2 && v1.is_max_ch(a.v1)].update_ch(a.v2[z2]);
			v2[z2 && v1.is_max_ch(b.v1)].update_ch(b.v2[z2]);
		}

		for (int z2 = 0; z2 < 2; z2++) {
			for (int z3 = 0; z3 < 2; z3++) {
				v3[z2][z3].reset();
			}
		}

		for (int z2 = 0; z2 < 2; z2++) {
			for (int z3 = 0; z3 < 2; z3++) {
				v3[z2 && v1.is_max_ch(a.v1)][z3 && v2[z2 && v1.is_max_ch(a.v1)].is_max_ch(a.v2[z2])].update_ch(a.v3[z2][z3]);
				v3[z2 && v1.is_max_ch(b.v1)][z3 && v2[z2 && v1.is_max_ch(b.v1)].is_max_ch(b.v2[z2])].update_ch(b.v3[z2][z3]);
			}
		}
	}
};

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int N, Q; cin >> N >> Q;
	vector<vector<pair<int, int>>> Xhist(N);
	for (int i = 0; i < N; i++) {
		int x; cin >> x;
		Xhist[i].emplace_back(0, x);
	}

	vector<vector<int>> queries(N);

	vector<bool> shouldPrint(Q+1);
	vector<int> ans(Q+1, -1);

	for (int q = 1; q <= Q; q++) {
		int op; cin >> op;
		if (op == 1) {
			int i, y; cin >> i >> y; i--;
			Xhist[i].emplace_back(q, y);
		} else if (op == 2) {
			int l; cin >> l; l--;
			queries[l].emplace_back(q);
			shouldPrint[q] = true;
		}
	}

	const int S = 1 << 19;
	vector<seg_node> seg(2*S);

	auto downdate_node = [&](int a) {
		if (a < S) {
			seg[a].downdate_ch(seg[2*a]);
			seg[a].downdate_ch(seg[2*a+1]);
		}
	};
	auto downdate_all = std::y_combinator([&](auto self, int a) -> void {
		if (!a) return;
		self(a/2);
		downdate_node(a);
	});

	auto update_node = [&](int a) {
		if (a < S) {
			seg[a].update_ch(seg[2*a], seg[2*a+1]);
		}
	};
	auto update_all = [&](int a) {
		for (; a; a /= 2) {
			update_node(a);
		}
	};

	for (int i = 0; i < S; i++) seg[S+i].deactivate_leaf();
	for (int i = S-1; i; i--) update_node(i);

	for (int i = 0; i < N; i++) {
		//cerr << "index " << i << '\n';

		for (int t : queries[i]) {
			// activate t
			//cerr << "activate " << t << '\n';
			downdate_all(S+t);
			seg[S+t].activate_leaf();
			update_all(S+t);
		}

		for (int z = 0; z < int(Xhist[i].size()); z++) {
			int t0 = Xhist[i][z].first;
			int t1 = z+1 == int(Xhist[i].size()) ? Q+1 : Xhist[i][z+1].first;
			int v = Xhist[i][z].second;
			// update [t0, t1) with v
			//cerr << "update " << t0 << ' ' << t1 << ' ' << v << '\n';

			assert(t0 < t1);

			std::y_combinator([&](auto self, int a) -> void {
				int alo = (a<<(__builtin_clz(a)-__builtin_clz(S)))-S;
				int ahi = ((a+1)<<(__builtin_clz(a)-__builtin_clz(S)))-S;
				if (t0 >= ahi || t1 <= alo) return;

				assert(a < 2*S);

				auto doPush = [&]() {
					downdate_node(a);
					self(2*a); self(2*a+1);
					update_node(a);
				};
				if (t0 > alo || t1 < ahi) {
					doPush(); return;
				}

				//cerr << "apply " << a << ' ' << alo << '-' << ahi << '\n';

				if (seg[a].v1.max_val == 0) {
					//cerr << "nothing active - exit" << '\n';
					return;
				}
				if (v <= seg[a].v1.max_val_2) {
					doPush(); return;
				}
				if (v <= seg[a].v1.max_val) {
					//cerr << "apply " << a << ' ' << alo << '-' << ahi << ' ' << "v1" << ' ' << seg[a].v1.max_val << "->" << v << '\n';
					seg[a].v1.max_val = v;
				}
				for (int z2 = 0; z2 <= (v > seg[a].v1.max_val); z2++) {
					if (seg[a].v2[z2].max_val == 0) continue;
					if (v <= seg[a].v2[z2].max_val_2) {
						doPush(); return;
					}
					if (v <= seg[a].v2[z2].max_val) {
						//cerr << "apply " << a << ' ' << alo << '-' << ahi << ' ' << "v2[" << z2 << "]" << ' ' << seg[a].v2[z2].max_val << "->" << v << '\n';
						seg[a].v2[z2].max_val = v;
					}
					for (int z3 = 0; z3 <= (v > seg[a].v2[z2].max_val); z3++) {
						if (seg[a].v3[z2][z3].max_val == 0) continue;
						if (v <= seg[a].v3[z2][z3].max_val_2) {
							doPush(); return;
						} else if (seg[a].v3[z2][z3].max_val_2 != 0) {
							// all of these guys get killed
							doPush(); return;
						}

						if (v <= seg[a].v3[z2][z3].max_val) {
							//cerr << "apply " << a << ' ' << alo << '-' << ahi << ' ' << "v3[" << z2 << ',' << z3 << "]" << ' ' << seg[a].v3[z2][z3].max_val << "->" << v << '\n';
							seg[a].v3[z2][z3].max_val = v;
						} else {
							// keep pushing
							if (a >= S) {
								//cerr << "deactivate " << a-S << '\n';

								assert(ans[a-S] == -1 && shouldPrint[a-S]);
								ans[a-S] = i;
								seg[a].deactivate_leaf();
								return;
							} else {
								doPush(); return;
							}
						}
					}
				}
			})(1);
		}
	}

	for (int q = 0; q <= Q; q++) {
		if (shouldPrint[q]) {
			if (ans[q] == -1) {
				cout << -1 << '\n';
			} else {
				cout << ans[q]+1 << '\n';
			}
		}
	}

	return 0;
}
