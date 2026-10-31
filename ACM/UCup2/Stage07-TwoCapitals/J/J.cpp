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

	int N, Q; cin >> N >> Q;

	const int MAXX = Q;
	std::vector<std::map<int, bool>> transitions(MAXX);
	for (int x = 0; x < Q; x++) {
		transitions[x][-1] = false;
	}

	std::array<order_statistic_set<std::pair<int, int>>, 2> combined;

	auto set_range = [&](int x, int l, int r, bool d) {
		auto it = std::prev(transitions[x].lower_bound(l));
		auto jt = transitions[x].upper_bound(r);
		// everything strictly between it and jt should change
		int initial = it->second;
		int final = std::prev(jt)->second;
		for (auto cur = std::next(it); cur != jt; ) {
			combined[cur->second].erase({cur->first, x});
			cur = transitions[x].erase(cur);
		}
		if (initial != d) {
			transitions[x].insert({l, d});
			combined[d].insert({l, x});
		}
		if (final != d) {
			transitions[x].insert({r, final});
			combined[final].insert({r, x});
		}
	};

	for (int q = 0; q < Q; q++) {
		char op; cin >> op;
		if (op == '+') {
			int l, r, x; cin >> l >> r >> x; l--, x--;
			set_range(x, l, r, true);
		} else if (op == '-') {
			int l, r, x; cin >> l >> r >> x; l--, x--;
			set_range(x, l, r, false);
		} else if (op == '?') {
			int k; cin >> k; k--;
			int ans = int(combined[true].order_of_key({k, Q})) - int(combined[false].order_of_key({k, Q}));
			cout << ans << '\n' << flush;
		}
	}

	return 0;
}
