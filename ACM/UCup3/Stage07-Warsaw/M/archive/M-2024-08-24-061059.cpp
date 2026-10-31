#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int L; cin >> L;
	std::vector<int> V(1 << L);
	for (auto& v : V) cin >> v;
	std::sort(V.begin(), V.end());
	std::reverse(V.begin(), V.end());
	int64_t ans = std::accumulate(V.begin(), V.end(), int64_t(0));
	for (int i = 1; i < L; i++) {
		ans = std::max(ans, std::accumulate(V.begin(), V.end() + (1 << i), int64_t(0)));
	}
	cout << ans << '\n';

	return 0;
}
