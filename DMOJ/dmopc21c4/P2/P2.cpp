#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> J(N);
	for (auto& j : J) cin >> j;

	int V = *std::max_element(J.begin(), J.end());
	std::vector<int> cnt(V+1);
	for (int j : J) cnt[j]++;

	int ans = 0;
	for (int i = 1; i <= V; i++) {
		if (cnt[i]) {
			ans++;
			for (int j = i; j <= V; j += i) cnt[j] = 0;
		}
	}
	cout << ans << '\n';


	return 0;
}
