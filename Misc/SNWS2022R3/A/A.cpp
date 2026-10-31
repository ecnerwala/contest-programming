#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> cntX(int(1e6)+10);
	std::vector<int> cntY(int(1e6)+10);
	std::vector<std::pair<int, int>> A(N);
	for (auto& [x, y] : A) {
		cin >> x >> y;
		cntX[x]++;
		cntY[y]++;
	}
	int a = 0; for (auto v : cntX) a += bool(v);
	int b = 0; for (auto v : cntY) b += bool(v);
	int64_t ans = int64_t(a) * int64_t(b);
	for (auto [x, y] : A) {
		if (cntX[x] == 1 && cntY[y] == 1) ans--;
	}
	cout << ans << '\n';

	return 0;
}
