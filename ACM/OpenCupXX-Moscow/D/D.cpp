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

const int INF = int(1e9) + 200;

struct can_kill_data {
	int min_nonzero_val;
	bool lazy, nonempty;
	can_kill_data() : min_nonzero_val(INF), lazy(false), nonempty(false) {}

	void reset() {
		min_nonzero_val = INF;
		lazy = false;
		nonempty = false;
	}

	void activate() {
		min_nonzero_val = INF;
		lazy = false;
		nonempty = true;
	}

	void update_with(const can_kill_data& a) {
		assert(!lazy);
		if (a.nonempty) {
			nonempty = true;
			min_nonzero_val = min(min_nonzero_val, a.min_nonzero_val);
		}
	}

	void downdate_to(can_kill_data& a) {
		if (a.nonempty) {
			if (lazy) {
				a.lazy = true;
				a.min_nonzero_val = min_nonzero_val;
			}
		}
	}

	bool apply_update(int v) {
		if (!nonempty) return false;
		assert(v);
		if (v > min_nonzero_val) return true;
		lazy = true;
		min_nonzero_val = v;
		return false;
	}
};

template <typename T> struct max_2_data {
	int max_val;
	int max_val_2;
	T ch[2];

	max_2_data() : max_val(0), max_val_2(-1), ch() {}

	void reset() {
		max_val = 0;
		max_val_2 = -1;
		ch[0].reset();
		ch[1].reset();
	}

	void activate() {
		max_val = INF;
		max_val_2 = 0;
		ch[0].reset();
		ch[1].activate();
	}
	
	void update_with(const max_2_data& a) {
		if (a.max_val > max_val) {
			max_val_2 = max(max_val, a.max_val_2);
			ch[0].update_with(ch[1]);
			ch[0].update_with(a.ch[0]);

			max_val = a.max_val;
			ch[1].reset();
			ch[1].update_with(a.ch[1]);
		} else if (a.max_val == max_val) {
			max_val_2 = max(max_val_2, a.max_val_2);
			ch[0].update_with(a.ch[0]);
			ch[1].update_with(a.ch[1]);
		} else {
			max_val_2 = max(max_val_2, a.max_val);
			ch[0].update_with(a.ch[0]);
			ch[0].update_with(a.ch[1]);
		}
	}

	bool is_max_ch(const max_2_data& a) const {
		return a.max_val >= max_val;
	}

	void downdate_to(max_2_data& a) {
		if (is_max_ch(a)) {
			a.max_val = max_val;
			ch[0].downdate_to(a.ch[0]);
			ch[1].downdate_to(a.ch[1]);
		} else {
			ch[0].downdate_to(a.ch[0]);
			ch[0].downdate_to(a.ch[1]);
		}
	}

	bool apply_update(int v) {
		if (!max_val) return false;

		if (v <= max_val_2) {
			return true;
		}
		if (ch[0].apply_update(v)) {
			return true;
		}
		if (v <= max_val) {
			max_val = v;
		} else {
			if (ch[1].apply_update(v)) {
				return true;
			}
		}
		return false;
	}
};

using seg_node = max_2_data<max_2_data<can_kill_data>>;

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
			seg[a].downdate_to(seg[2*a]);
			seg[a].downdate_to(seg[2*a+1]);
		}
	};
	auto downdate_all = std::y_combinator([&](auto self, int a) -> void {
		if (!a) return;
		self(a/2);
		downdate_node(a);
	});

	auto update_node = [&](int a) {
		if (a < S) {
			seg[a].reset();
			seg[a].update_with(seg[2*a]);
			seg[a].update_with(seg[2*a+1]);
		}
	};
	auto update_all = [&](int a) {
		for (; a; a /= 2) {
			update_node(a);
		}
	};

	for (int i = 0; i < S; i++) seg[S+i].reset();
	for (int i = S-1; i; i--) update_node(i);

	for (int i = 0; i < N; i++) {
		//cerr << "index " << i << '\n';

		for (int t : queries[i]) {
			// activate t
			//cerr << "activate " << t << '\n';
			downdate_all(S+t);
			seg[S+t].activate();
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

				if (seg[a].apply_update(v)) {
					if (a >= S) {
						//cerr << "deactivate " << a-S << '\n';

						assert(ans[a-S] == -1 && shouldPrint[a-S]);
						ans[a-S] = i;
						seg[a].reset();
						return;
					} else {
						doPush(); return;
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
