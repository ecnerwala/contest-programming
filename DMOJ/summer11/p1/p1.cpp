#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	struct computer {
		int f; int c; int v;
	};
	int N; cin >> N;
	vector<computer> computers(N);
	for (auto& c : computers) cin >> c.c >> c.f >> c.v;
	int M; cin >> M;
	vector<computer> orders(M);
	for (auto& c : orders) cin >> c.c >> c.f >> c.v;

	sort(computers.begin(), computers.end(), [](const computer& a, const computer& b) { return a.f < b.f; });
	sort(orders.begin(), orders.end(), [](const computer& a, const computer& b) { return a.f < b.f; });

	const int64_t INF = 1e18;

	int V = 0;
	for (const auto& c : computers) V += c.c;
	vector<int64_t> dp; dp.reserve(V+1);
	dp.push_back(0);

	while (!orders.empty()) {
		while (!computers.empty() && computers.back().f >= orders.back().f) {
			int c = computers.back().c;
			int64_t v = computers.back().v;
			computers.pop_back();

			dp.resize(int(dp.size())+c, -INF);
			for (int i = int(dp.size())-1; i >= c; i--) {
				dp[i] = max(dp[i], dp[i-c] - v);
			}
		}

		int c = orders.back().c;
		int64_t v = orders.back().v;
		orders.pop_back();
		for (int i = c; i < int(dp.size()); i++) {
			dp[i-c] = max(dp[i-c], dp[i] + v);
		}
	}
	cout << *max_element(dp.begin(), dp.end()) << '\n';

	return 0;
}
