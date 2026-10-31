#include<bits/extc++.h>

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

	std::vector<order_statistic_set<int>> bit(N);
	for (auto [y, t, k] : L) {
		int mi = 0, ma = t;
		int cnt = k;
		while (ma > mi) {
			int md = mi | ((1 << (31 - __builtin_clz(mi ^ ma))) - 1);
			int v = int(bit[md].order_of_key(y));
			if (v < cnt) {
				cnt -= v;
				mi = md+1;
			} else {
				ma = md;
			}
		}
		T[t] = ma;
		for (int a = ma; a < N; a |= a+1) {
			bit[a].insert(y);
		}
	}

	for (int i = 0; i < N; i++) {
		cout << T[i]+1 << " \n"[i+1==N];
	}

	return 0;
}
