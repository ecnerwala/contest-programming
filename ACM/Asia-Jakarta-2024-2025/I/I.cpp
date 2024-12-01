#include <bits/stdc++.h>
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

	int N, M; cin >> N >> M;
	std::vector<int> A(N);
	for (auto& a : A) { cin >> a; a--; }
	std::vector<int> prev(M, -1);

	int64_t ans = 0;
	order_statistic_set<int> lasts;
	for (int i = 0; i < N; i++) {
		int a = A[i];
		int p = prev[a];
		if (p != -1) {
			lasts.erase(p);
		}
		int cnt = int(lasts.size()) - int(lasts.order_of_key(p));
		ans += cnt;
		prev[a] = i;
		lasts.insert(i);
	}
	int tot = int(lasts.size());
	ans += int64_t(tot) * int64_t(tot - 1) / 2;
	cout << ans << '\n';

	return 0;
}
