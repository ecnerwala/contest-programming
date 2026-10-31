#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	struct f_t {
		int P, W, D;
	};
	vector<f_t> F(N);
	vector<pair<int, int>> transitions; transitions.reserve(2*N);
	int tot_w = 0;
	for (auto& f : F) {
		cin >> f.P >> f.W >> f.D;
		tot_w += f.W;
		transitions.push_back({f.P - f.D, f.W});
		transitions.push_back({f.P + f.D, f.W});
	}
	sort(transitions.begin(), transitions.end());
	int64_t cur_x = int(2.01e9);
	while (tot_w > 0) {
		cur_x = transitions.back().first;
		tot_w -= transitions.back().second;
		transitions.pop_back();
	}
	int64_t ans = 0;
	for (auto f : F) {
		ans += std::max<int64_t>(std::abs(cur_x - f.P) - f.D, 0) * f.W;
	}
	cout << ans << '\n';

	return 0;
}
