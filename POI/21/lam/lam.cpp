#include<bits/extc++.h>

struct splitmix64_hash {
	static uint64_t splitmix64(uint64_t x) {
		// http://xorshift.di.unimi.it/splitmix64.c
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}

	size_t operator()(uint64_t x) const {
		static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
		return splitmix64(x + FIXED_RANDOM);
	}
};

template <typename K, typename V, typename Hash = splitmix64_hash>
using hash_map = __gnu_pbds::gp_hash_table<K, V, Hash>;

template <typename K, typename Hash = splitmix64_hash>
using hash_set = hash_map<K, __gnu_pbds::null_type, Hash>;

/** Binary-indexed tree
 *
 *  A binary indexed tree with N nodes of type T implicitly provides the
 *  following two functions for 0 <= i <= N:
 *
 *      prefix(int i) -> list<T&>
 *      suffix(int i) -> list<T&>
 *
 *  such that size(suffix(i) intersect prefix(j)) = (1 if i < j else 0).
 *  Furthermore, the resulting lists always have size at most log_2(N).
 *
 *  This can be used to implement either point-update/(prefix|suffix)-query or
 *  (prefix|suffix)-update/point-query over a virtual array of size N of a
 *  commutative monoid. This can be generalized to implement
 *  point-update/range-query or range-update/point-query over a virtual array
 *  of size N of a commutative group.
 *
 *  With 0-indexed data, prefixes are more natural:
 *   * For range update/query, use for_prefix for the ranges and for_suffix for the points.
 *   * For prefix update/query, no change.
 *   * For suffix update/query, use for_prefix(point + 1); 1-index the data.
 */
template <typename T> struct binary_indexed_tree {
private:
	std::vector<T> dat;
public:
	binary_indexed_tree() {}
	explicit binary_indexed_tree(size_t N) : dat(N) {}
	binary_indexed_tree(size_t N, const T& t) : dat(N, t) {}

	template <typename F> void for_prefix(int i, F f) const {
		assert(0 <= i && i <= int(dat.size()));
		for (int a = i; a; a &= a-1) {
			f(dat[a-1]);
		}
	}

	template <typename F> void for_prefix(int i, F f) {
		assert(0 <= i && i <= int(dat.size()));
		for (int a = i; a; a &= a-1) {
			f(dat[a-1]);
		}
	}

	template <typename F> void for_suffix(int i, F f) const {
		assert(0 <= i && i <= int(dat.size()));
		for (int a = i; a < int(dat.size()); a |= a+1) {
			f(dat[a]);
		}
	}

	template <typename F> void for_suffix(int i, F f) {
		assert(0 <= i && i <= int(dat.size()));
		for (int a = i; a < int(dat.size()); a |= a+1) {
			f(dat[a]);
		}
	}
};
template <typename T, typename K = int> struct sparse_binary_indexed_tree {
private:
	K N;
	hash_map<K, T> dat;
public:
	sparse_binary_indexed_tree() {}
	explicit sparse_binary_indexed_tree(K N_) : N(N_) {}

	template <typename F> void for_prefix(K i, F f) const {
		assert(0 <= i && i <= N);
		for (K a = i; a; a &= a-1) {
			f(dat[a-1]);
		}
	}

	template <typename F> void for_prefix(K i, F f) {
		assert(0 <= i && i <= N);
		for (K a = i; a; a &= a-1) {
			f(dat[a-1]);
		}
	}

	template <typename F> void for_suffix(K i, F f) const {
		assert(0 <= i && i <= N);
		for (K a = i; a < N; a |= a+1) {
			f(dat[a]);
		}
	}

	template <typename F> void for_suffix(K i, F f) {
		assert(0 <= i && i <= N);
		for (K a = i; a < N; a |= a+1) {
			f(dat[a]);
		}
	}
};

template <typename K, typename V, typename Comp = std::less<K>>
using order_statistic_map = __gnu_pbds::tree<
	K, V, Comp,
	__gnu_pbds::rb_tree_tag,
	__gnu_pbds::tree_order_statistics_node_update
>;

template <typename K, typename Comp = std::less<K>>
using order_statistic_set = order_statistic_map<K, __gnu_pbds::null_type, Comp>;

// Supports
//  auto iterator = order_statistic_set().find_by_order(idx); // (0-indexed)
//  int num_strictly_smaller = order_statistic_set().order_of_key(key);

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

	int N; cin >> N;

	struct lamp_t {
		int y;
		int t;
		int k;
	};
	std::vector<lamp_t> L(N);

	{
		int64_t X1, Y1, X2, Y2; cin >> X1 >> Y1 >> X2 >> Y2;
		bool is_laser;
		if (X1 * Y2 - Y1 * X2 == 0) {
			// It's the 1D case
			assert(X1 * X2 + Y1 * Y2 > 0);
			is_laser = true;
			// let x2, y2 be the perpendicular direction
			X2 = -Y1;
			Y2 = X1;
		} else {
			is_laser = false;
			if (X1 * Y2 - Y1 * X2 < 0) {
				swap(X1, X2);
				swap(Y1, Y2);
			}
		}
		assert(X1 * Y2 - Y1 * X2 > 0);
		struct pt_t {
			int64_t x, y; int t;
		};
		std::vector<pt_t> pts(N);
		for (int i = 0; i < N; i++) {
			int64_t x, y; cin >> x >> y;
			pts[i].x = x * Y2 - y * X2;
			pts[i].y = X1 * y - Y1 * x;
			pts[i].t = i;
		}

		if (is_laser) {
			sort(pts.begin(), pts.end(), [](const pt_t& a, const pt_t& b) { return make_pair(-a.y, a.x) < make_pair(-b.y, b.x); });
		} else {
			sort(pts.begin(), pts.end(), [](const pt_t& a, const pt_t& b) { return make_pair(a.x, a.y) < make_pair(b.x, b.y); });
		}
		for (int i = 0; i < N; i++) {
			pts[i].x = i;
		}

		sort(pts.begin(), pts.end(), [](const pt_t& a, const pt_t& b) { return make_pair(a.y, a.x) < make_pair(b.y, b.x); });
		for (int i = 0; i < N; i++) {
			L[i] = {int(pts[i].x), pts[i].t, 0};
		}
	}

	{
		std::vector<int> K(N); for (auto& k : K) cin >> k;
		for (auto& lmp : L) {
			lmp.k = K[lmp.t];
		}
	}

	std::vector<int> T(N);

	if constexpr (true) {
		binary_indexed_tree<int> bit(N);
		// mi-ma is the inclusive range of possible end times
		std::y_combinator([&bit, &T](auto self, std::vector<lamp_t> L, int mi, int ma) -> void {
			if (L.empty()) return;
			int min_end = ma;
			int max_end = mi;
			for (const auto& [y, t, k] : L) {
				if (t > ma) continue;
				assert(t >= mi);
				min_end = min(min_end, t);
				max_end = max(max_end, t);
			}
			mi = max(mi, min_end);
			ma = min(ma, max_end);
			assert(mi <= ma);
			if (mi == ma) {
				for (const auto& [y, t, k] : L) {
					T[t] = mi;
				}
				return;
			}

			int md = mi + (ma - mi) / 2;
			assert(mi <= md && md < ma);
			std::vector<lamp_t> lo; lo.reserve(L.size());
			std::vector<lamp_t> hi; hi.reserve(L.size());
			for (const auto& [y, t, k] : L) {
				if (t <= md) {
					lo.push_back({y, t, k});
					bit.for_suffix(y, [](int& a) { a++; });
				} else {
					int c = 0;
					bit.for_prefix(y, [&c](int a) { c += a; });
					if (c < k) {
						hi.push_back({y, t, k-c});
					} else {
						lo.push_back({y, t, k});
						bit.for_suffix(y, [](int& a) { a++; });
					}
				}
			}

			L = {};

			for (const auto& [y, t, k] : lo) {
				bit.for_suffix(y, [](int& a) { --a; });
			}

			self(std::move(lo), mi, md);
			self(std::move(hi), md+1, ma);
		})(L, 0, N-1);
	} else {
		std::vector<sparse_binary_indexed_tree<int>> bits(N, sparse_binary_indexed_tree<int>(N));
		for (const auto& [y, t, k] : L) {
			int mi = 0, ma = t;
			int cnt = k;
			while (ma > mi) {
				int md = mi | ((1 << (31 - __builtin_clz(mi ^ ma))) - 1);
				int v = 0; bits[md].for_prefix(y, [&v](int x) { v += x; });
				if (v < cnt) {
					cnt -= v;
					mi = md+1;
				} else {
					ma = md;
				}
			}
			T[t] = ma;
			for (int a = ma; a < N; a |= a+1) {
				bits[a].for_suffix(y, [](int& x) { x++; });
			}
		}
	}

	for (int i = 0; i < N; i++) {
		cout << T[i]+1 << " \n"[i+1==N];
	}

	return 0;
}
