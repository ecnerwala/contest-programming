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
	unordered_map<int, pair<order_statistic_set<pair<int, int>>, order_statistic_set<pair<int, int>>>> mp;
	int Q; cin >> Q;
	for (int q = 0; q < Q; q++) {
		int op; cin >> op;
		int t, x; cin >> t >> x;
		auto& sets = mp[x];
		if (op == 1) {
			sets.first.insert({t, q});
		} else if (op == 2) {
			sets.second.insert({t, q});
		} else if (op == 3) {
			int ans = int(sets.first.order_of_key({t, -1})) - int(sets.second.order_of_key({t, -1}));
			cout << ans << '\n';
		} else assert(false);
	}

	return 0;
}
