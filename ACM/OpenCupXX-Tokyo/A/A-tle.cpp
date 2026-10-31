#include<bits/stdc++.h>
#include<bits/extc++.h>
using namespace std;

template <typename K, typename V, typename Comp = std::less<K>>
using order_statistic_map = __gnu_pbds::tree<
	K, V, Comp,
	__gnu_pbds::rb_tree_tag,
	__gnu_pbds::tree_order_statistics_node_update
>;
template <typename K, typename Comp = std::less<K>>
using order_statistic_set = order_statistic_map<K, __gnu_pbds::null_type, Comp>;

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
	int tot_sum;
	int min_pref;
	int max_pref;

	range_data() { tot_sum = min_pref = max_pref = 0; }

	explicit range_data(int d) {
		tot_sum = d;
		min_pref = min(0, d);
		max_pref = max(0, d);
	}
	range_data(range_data a, range_data b) {
		tot_sum = a.tot_sum + b.tot_sum;
		min_pref = min(a.min_pref, a.tot_sum + b.min_pref);
		max_pref = max(a.max_pref, a.tot_sum + b.max_pref);
	}

	int get_range() const {
		return max_pref - min_pref;
	}

	int final_value() const {
		return tot_sum - min_pref;
	}
};

const int SEG_SZ = int(1e7);
int seg_ind = 1;
struct seg_node {
	int c[2] = {0,0};
	range_data dat;
} seg[SEG_SZ];

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int N; cin >> N;
	vector<int> A_enc(N);
	for (int& a_enc : A_enc) cin >> a_enc;

	int M; cin >> M;
	vector<pair<int, int>> B(M);
	for (int i = 0; i < M; i++) {
		cin >> B[i].first;
		B[i].second = i;
	}

	sort(B.begin(), B.end());

	string S; cin >> S;
	assert(int(S.size()) == M);

	auto new_node = [&]() -> int {
		assert(seg_ind < SEG_SZ);
		assert(seg_ind);
		return seg_ind++;
	};

	// we store # of small ones; B means remove small one

	auto init_tree = [&]() -> int {
		return std::y_combinator([&](auto self, int l, int r) -> int {
			int n = new_node();
			if (r - l == 1) {
				seg[n].c[0] = seg[n].c[1] = 0;
				seg[n].dat = range_data((S[l] == 'B') ? -1 : 0);
			} else {
				int m = (l + r) / 2;
				seg[n].c[0] = self(l, m);
				seg[n].c[1] = self(m, r);
				seg[n].dat = range_data(seg[seg[n].c[0]].dat, seg[seg[n].c[1]].dat);
			}
			return n;
		})(0, M);
	};

	auto incr = [&](int root, int v) -> int {
		return std::y_combinator([&](auto self, int cur, int l, int r) -> int {
			assert(l <= v && v < r);
			int n = new_node();
			seg[n] = seg[cur];
			if (r - l == 1) {
				seg[n].dat = range_data(seg[n].dat.tot_sum+1);
			} else {
				int m = (l + r) / 2;
				if (v < m) {
					seg[n].c[0] = self(seg[n].c[0], l, m);
				} else {
					seg[n].c[1] = self(seg[n].c[1], m, r);
				}
				seg[n].dat = range_data(seg[seg[n].c[0]].dat, seg[seg[n].c[1]].dat);
			}
			return n;
		})(root, 0, M);
	};

	auto get_suff = [&](int root, int d, int v0) -> int {
		assert(root);
		assert(0 <= v0 && v0 <= d);
		if (seg[root].dat.get_range() <= d) {
			range_data tree = seg[root].dat;
			range_data cnd(range_data(v0), tree);
			if (cnd.get_range() >= d) {
				return d - (tree.max_pref - tree.tot_sum);
			} else {
				return cnd.tot_sum - cnd.min_pref;
			}
			assert(false);
		}

		assert(seg[root].dat.get_range() > d);
		range_data ans;
		for (int cur = root; seg[cur].c[1]; ) {
			range_data cnd(seg[seg[cur].c[1]].dat, ans);
			if (cnd.get_range() <= d) {
				ans = cnd;
				cur = seg[cur].c[0];
			} else {
				cur = seg[cur].c[1];
			}
		}
		return ans.final_value();
	};

	vector<int> roots(M+1);
	roots[0] = init_tree();
	for (int i = 0; i < M; i++) {
		roots[i+1] = incr(roots[i], B[i].second);
	}

	int64_t lastAns = 0;

	order_statistic_set<pair<int, int>> curA;
	order_statistic_set<pair<int, int>> curAns;

	for (int i = 1; i <= N; i++) {
		int a = (A_enc[i-1] + lastAns) % int(1e9);
		//cerr << "i = " << i << ' ' << "A = " << a << '\n';
		curA.insert({a, i});

		int mi = -1, ma = int(1e9);
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			int ind = int(lower_bound(B.begin(), B.end(), pair<int, int>(md, -1)) - B.begin());
			int v = get_suff(roots[ind], i, int(curA.order_of_key(pair<int, int>(md, -1))));
			v -= int(curAns.order_of_key(pair<int, int>(md, -1)));
			if (v) {
				ma = md;
			} else {
				mi = md;
			}
		}
		//cerr << mi << '\n';
		curAns.insert({mi, i});
		lastAns += mi;
		cout << lastAns << " \n"[i==N];
	}

	return 0;
}
