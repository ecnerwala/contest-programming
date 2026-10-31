#include<bits/stdc++.h>

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

template <typename F> struct reverse_comparator : private F {
	template <typename Arg1, typename Arg2> inline bool operator() (Arg1&& arg1, Arg2&& arg2) {
		return F::operator()(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
};

template <typename T, typename C = int, typename Comp = std::less<T>> struct cnt_min {
	T v;
	C cnt;

	cnt_min() : v(), cnt(0) {}
	explicit cnt_min(T v_) : v(v_), cnt(1) {}
	cnt_min(T v_, C cnt_) : v(v_), cnt(cnt_) {}

	friend cnt_min operator + (const cnt_min& a, const cnt_min& b) {
		if (!b.cnt) return a;
		else if (!a.cnt) return b;
		else if (Comp().operator()(a.v, b.v)) return a;
		else if (Comp().operator()(b.v, a.v)) return b;
		else return cnt_min(a.v, a.cnt + b.cnt);
	}

	cnt_min& operator += (const cnt_min& o) {
		return *this = (*this + o);
	}
};

template <typename T, typename C = int, typename Comp = std::less<T>> using cnt_max = cnt_min<T, C, reverse_comparator<Comp>>;

template <typename T, typename C> struct range_cnt {
	C tot_cnt = 0;
	cnt_min<T, C> mi{};
	cnt_max<T, C> ma{};

	range_cnt() {}
	explicit range_cnt(T v, C c = 1) : tot_cnt(c), mi(v, c), ma(v, c) {}

	friend range_cnt operator + (const range_cnt& a, const range_cnt& b) {
		range_cnt res;
		res.tot_cnt = a.tot_cnt + b.tot_cnt;
		res.mi = a.mi + b.mi;
		res.ma = a.ma + b.ma;
		return res;
	}

	range_cnt& operator += (int v) { mi.v += v, ma.v += v; return *this; }
	range_cnt& operator += (const range_cnt& o) { return *this = (*this + o); }
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	array<int, 2> N; int Q; cin >> N[0] >> N[1] >> Q;
	struct seg_node { int lazy = 0; range_cnt<int, int> d; };
	array<vector<seg_node>, 2> seg;
	for (int z = 0; z < 2; z++) {
		seg[z] = vector<seg_node>(2*N[z]);
		for (int i = 0; i < N[z]; i++) {
			int v; cin >> v;
			seg[z][N[z]+i].d = range_cnt<int, int>(v);
		}
		for (int a = N[z]-1; a; --a) {
			seg[z][a].d = seg[z][2*a].d + seg[z][2*a+1].d;
		}
	}

	auto first_cover = [](int a) {
		if (!a) return 0;
		return a >> __builtin_ctz(a) >> 1;
	};

	for (int q = 0; q < Q; q++) {
		int op; cin >> op;
		if (op == 1 || op == 2) {
			bool z = op == 1 ? 0 : 1;
			int l, r, v; cin >> l >> r >> v; l--;
			for (int a = N[z]+l, b = N[z]+r; a < b; a /= 2, b /= 2) {
				if (a & 1) {
					seg[z][a].lazy += v;
					seg[z][a].d += v;
					a++;
				}
				if (b & 1) {
					--b;
					seg[z][b].lazy += v;
					seg[z][b].d += v;
				}
			}
			for (int a = first_cover(N[z]+l); a; a /= 2) {
				seg[z][a].d = seg[z][2*a].d + seg[z][2*a+1].d;
				seg[z][a].d += seg[z][a].lazy;
			}
			for (int a = first_cover(N[z]+r); a; a /= 2) {
				seg[z][a].d = seg[z][2*a].d + seg[z][2*a+1].d;
				seg[z][a].d += seg[z][a].lazy;
			}
		} else if (op == 3 || op == 4) {
			array<vector<pair<int64_t, int64_t>>, 2> values;
			for (int z = 0; z < 2; z++) {
				auto downdate_node = [&](int a) -> void {
					int v = seg[z][a].lazy;
					if (a < N[z]) {
						seg[z][2*a].lazy += v;
						seg[z][2*a].d += v;
						seg[z][2*a+1].lazy += v;
						seg[z][2*a+1].d += v;
					}
					seg[z][a].lazy = 0;
				};
				auto downdate_all = [&](int leaf) -> void {
					std::y_combinator([&](auto self, int a) -> void {
						if (!a) return;
						self(a/2);
						downdate_node(a);
					})(first_cover(leaf));
				};
				int l, r; cin >> l >> r; l--;
				downdate_all(N[z]+l);
				downdate_all(N[z]+r);

				range_cnt<int, int> res;
				for (int a = N[z]+l, b = N[z]+r; a < b; a /= 2, b /= 2) {
					if (a & 1) res += seg[z][a++].d;
					if (b & 1) res += seg[z][--b].d;
				}

				assert(res.tot_cnt > 0);
				assert(res.tot_cnt == r - l);
				if (res.mi.v == res.ma.v) {
					assert(res.tot_cnt == res.mi.cnt);
					assert(res.tot_cnt == res.ma.cnt);
					values[z] = {{res.mi.v * 2, res.tot_cnt}};
				} else {
					values[z] = {
						{res.mi.v * 2, res.mi.cnt},
						{res.mi.v + res.ma.v, res.tot_cnt - res.mi.cnt - res.ma.cnt},
						{res.ma.v * 2, res.ma.cnt},
					};
				}
			}

			range_cnt<int64_t, int64_t> prod_cnt;
			for (auto [v0, c0] : values[0]) {
				for (auto [v1, c1] : values[1]) {
					prod_cnt += range_cnt<int64_t, int64_t>(int64_t(v0) * int64_t(v1), int64_t(c0) * int64_t(c1));
				}
			}
			int64_t v = (op == 3) ? prod_cnt.mi.v : prod_cnt.ma.v;
			int64_t cnt = (op == 3) ? prod_cnt.mi.cnt : prod_cnt.ma.cnt;
			assert(v % 4 == 0);
			v /= 4;
			cout << v << ' ' << cnt << '\n';
		}
	}

	return 0;
}
