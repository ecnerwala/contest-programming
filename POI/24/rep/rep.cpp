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

	const int V = int(1e9) + 20;
	order_statistic_map<int, array<int, 2>> diffs;
	vector<int> A; A.push_back(1);
	int cur_diff = 1;
	for (int i = 1; true; i++) {
		if (i & 1) {
			while (diffs.find(cur_diff) != diffs.end()) cur_diff++;
			A.push_back(A.back() + cur_diff);
		} else {
			if (A.back() > V) break;
			A.push_back(A.back() * 2);
		}
		for (int j = 0; j < i; j++) {
			diffs[A[i] - A[j]] = {i, j};
		}
	}

	int Q; cin >> Q;
	while (Q--) {
		int X; cin >> X;
		assert(X <= V);
		array<int, 2> res;
		auto it = diffs.find(X);
		if (it != diffs.end()) {
			res = it->second;
		} else {
			res[1] = int(A.size()) + 2 * (X - 1 - int(diffs.order_of_key(X)));
			res[0] = res[1]+1;
		}
		cout << res[0]+1 << ' ' << res[1]+1 << '\n';
	}

	return 0;
}
