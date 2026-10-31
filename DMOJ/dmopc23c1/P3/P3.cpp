#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

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
	std::vector<int> B(N);
	order_statistic_set<int> last_occs;
	int V = 0;
	auto query = [&](int lo, int hi) -> int {
		cout << '?' << ' ' << lo+1 << ' ' << hi+1 << '\n' << flush;
		int res; cin >> res;
		if (res == -1) exit(0);
		return res;
	};
	for (int i = 0; i < N; i++) {
		int mi = -1;
		int ma = int(last_occs.size());
		if (i > 0 && query(0, i) > int(last_occs.size())) {
			ma = 0;
		} else {
			mi = 0;
		}
		// we equal mi
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			auto it = last_occs.find_by_order(md);
			int res = query(*it, i);
			assert(res <= 1 + int(last_occs.size()) - md);
			if (res <= int(last_occs.size()) - md) {
				mi = md;
			} else {
				ma = md;
			}
		}
		if (mi == -1) {
			B[i] = V++;
		} else {
			auto it = last_occs.find_by_order(mi);
			B[i] = B[*it];
			last_occs.erase(it);
		}
		last_occs.insert(i);
	}
	cout << '!';
	for (int i = 0; i < N; i++) {
		cout << ' ' << B[i]+1;
	}
	cout << '\n' << flush;

	return 0;
}
