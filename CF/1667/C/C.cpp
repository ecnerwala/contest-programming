#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;
	std::vector<std::array<int, 2>> ans; ans.reserve(N);
	ans.push_back({0, 0});
	std::set<int> unvis;
	for (int i = 1; i < N; i++) unvis.insert(i);
	for (int d = 1; !unvis.empty(); d++) {
		int x = *unvis.begin();
		unvis.erase(x);
		if (x+d >= N) break;
		assert(unvis.count(x+d));
		unvis.erase(x+d);
		if (unvis.empty() || *unvis.begin() + d >= N) {
			ans.push_back({x, x});
			break;
		}
		ans.push_back({x, x+d});
		ans.push_back({x+d, x});
	}

	cout << ans.size() << '\n';
	for (auto [x, y] : ans) {
		cout << x+1 << ' ' << y+1 << '\n';
	}

	return 0;
}
