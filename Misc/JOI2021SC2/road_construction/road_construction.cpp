#include <bits/extc++.h>

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

	int N, K; cin >> N >> K;
	vector<array<int64_t, 2>> A(N);
	for (auto& a : A) {
		int64_t x, y; cin >> x >> y;
		a = {x+y, x-y};
	}
	sort(A.begin(), A.end());

	auto cnt_leq = [&](int64_t val) -> int64_t {
		int64_t cnt = 0;
		order_statistic_set<pair<int64_t, int>> s;
		for (int i = 0, j = 0; i < N; i++) {
			while (j < i && A[j][0] < A[i][0] - val) {
				s.erase({A[j][1], j});
				j++;
			}

			{
				int64_t y = A[i][1];
				cnt += int(s.order_of_key({y + val, N})) - int(s.order_of_key({y - val, -1}));
			}

			s.insert({A[i][1], i});
		}
		return cnt;
	};

	int64_t mi = 0;
	int64_t ma = int64_t(4e9) + 100;
	while (ma - mi > 1) {
		int64_t md = (mi + ma) / 2;
		if (cnt_leq(md) <= K) {
			mi = md;
		} else {
			ma = md;
		}
	}

	vector<int64_t> ans; ans.reserve(K);

	{
		int64_t val = mi;
		order_statistic_set<pair<int64_t, int>> s;
		for (int i = 0, j = 0; i < N; i++) {
			while (j < i && A[j][0] < A[i][0] - val) {
				s.erase({A[j][1], j});
				j++;
			}

			{
				int64_t y = A[i][1];
				for (auto it = s.lower_bound({y-val, -1}); it != s.end() && it->first <= y+val; ++it) {
					int k = it->second;
					ans.push_back(max(abs(A[i][0] - A[k][0]), abs(A[i][1] - A[k][1])));
				}
			}

			s.insert({A[i][1], i});
		}
	}

	sort(ans.begin(), ans.end());
	assert(int(ans.size()) <= K);
	ans.resize(K, ma);
	for (auto v : ans) cout << v << '\n';

	return 0;
}
