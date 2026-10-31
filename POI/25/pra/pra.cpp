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
	vector<tuple<int, int, int>> times(N);
	for (int i = 0; i < N; i++) {
		int l, r; cin >> l >> r;
		times[i] = {l, r, i};
	}

	sort(times.begin(), times.end());

	pair<int, int> ans{-1, -1};
	order_statistic_set<pair<int, int>> s;
	for (int i = 0; i < N; i++) {
		auto [l, r, x] = times[i];
		s.insert({r, x});
		if (int(s.size()) >= K) {
			ans = max(ans, {s.find_by_order(int(s.size()) - K)->first - l, i});
		}
	}

	cout << ans.first << '\n';

	s.clear();
	for (int i = 0; i <= ans.second; i++) {
		auto [l, r, x] = times[i];
		s.insert({r, x});
	}
	assert(int(s.size()) >= K);
	vector<bool> take(N);
	for (auto it = s.find_by_order(int(s.size()) - K); it != s.end(); ++it) {
		take[it->second] = true;
	}

	int cnt = 0;
	for (int i = 0; i < N; i++) {
		if (take[i]) {
			cout << i+1 << " \n"[++cnt == K];
		}
	}

	return 0;
}
