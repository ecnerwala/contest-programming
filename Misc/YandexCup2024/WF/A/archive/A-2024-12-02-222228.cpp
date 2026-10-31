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
	std::vector<std::pair<int, int>> P(N);
	for (auto& [x, y] : P) cin >> x >> y;

	std::vector<int> Xs(N);
	std::vector<int> Ys(N);
	for (int i = 0; i < N; i++) {
		Xs[i] = P[i].first;
		Ys[i] = P[i].second;
	}
	std::sort(Ys.begin(), Ys.end());
	Ys.resize(std::unique(Ys.begin(), Ys.end()) - Ys.begin());
	std::sort(Xs.begin(), Xs.end());
	Xs.resize(std::unique(Xs.begin(), Xs.end()) - Xs.begin());
	for (int i = 0; i < N; i++) {
		P[i].first = int(std::lower_bound(Xs.begin(), Xs.end(), P[i].first) - Xs.begin());
		P[i].second = int(std::lower_bound(Ys.begin(), Ys.end(), P[i].second) - Ys.begin());
	}

	std::sort(P.begin(), P.end());

	auto check = [&](int cnt) -> std::optional<std::pair<int, int>> {
		assert(cnt >= 1);
		order_statistic_set<std::pair<int, int>> los;
		order_statistic_set<std::pair<int, int>> his;
		for (int i = 0; i < N; i++) {
			his.insert({P[i].second, i});
		}
		for (int i = 0; i+1 < N; i++) {
			// move P[i] from lo to hi
			{
				his.erase({P[i].second, i});
				los.insert({P[i].second, i});
			}
			if (P[i].first < P[i+1].first) {
				// check this cut
				if (int(los.size()) >= 2 * cnt && int(his.size()) >= 2 * cnt) {
					int ylo = std::max(los.find_by_order(cnt - 1)->first, his.find_by_order(cnt - 1)->first);
					int yhi = std::min(los.find_by_order(int(los.size()) - cnt)->first, his.find_by_order(int(his.size()) - cnt)->first);
					if (ylo < yhi) return std::pair<int, int>{Xs[P[i+1].first], Ys[yhi]};
				}
			}
		}
		return std::nullopt;
	};

	int mi = 0;
	std::pair<int, int> best{0, 0};
	int ma = N+1;
	while (ma - mi > 1) {
		int md = (mi + ma) / 2;
		auto res = check(md);
		if (res) {
			mi = md;
			best = *res;
		} else {
			ma = md;
		}
	}
	cout << mi << '\n';
	cout << best.first << ' ' << best.second << '\n';

	return 0;
}
