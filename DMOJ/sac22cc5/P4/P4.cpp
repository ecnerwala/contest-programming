#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	std::vector<std::array<int, 2>> intervals(N);
	for (auto& a : intervals) { cin >> a[0] >> a[1]; }
	std::sort(intervals.begin(), intervals.end());

	std::vector<std::array<int, 2>> actual_intervals; actual_intervals.reserve(N);
	for (auto [x, y] : intervals) {
		if (actual_intervals.empty() || x > actual_intervals.back()[1] + 1) {
			actual_intervals.push_back({x, y});
		} else {
			setmax(actual_intervals.back()[1], y);
		}
	}

	while (Q--) {
		int l, r; cin >> l >> r;
		auto it = std::lower_bound(actual_intervals.begin(), actual_intervals.end(), std::array<int, 2>{l+1, l});
		if (it == actual_intervals.begin() || (*std::prev(it))[1] < r) {
			cout << 'N' << '\n';
		} else {
			cout << 'Y' << '\n';
		}
	}

	return 0;
}
