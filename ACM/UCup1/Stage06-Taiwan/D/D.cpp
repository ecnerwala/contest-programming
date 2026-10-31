#pragma GCC target("popcnt")
#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, TH; cin >> N >> TH;
	using bs = bitset<10001>;
	std::vector<std::array<int, 2>> vals(N);
	std::vector<bs> by_w(6);
	for (int i = 0; i < N; i++) {
		int x, y, z, w; cin >> x >> y >> z >> w;
		assert(x == i+2);
		y -= 2, z -= 2;
		y = std::max(y, -1);
		z = std::max(z, -1);
		vals[i] = {y, z};
		by_w[w][i] = true;
	}

	std::vector<int> ans(N);
	std::vector<bs> msks(N);
	for (int i = N-1; i >= 0; i--) {
		msks[i][i] = true;
		for (int w = 1; w <= 5; w++) {
			ans[i] += w * (msks[i] & by_w[w]).count();
		}
		for (int j : vals[i]) {
			if (j == -1) continue;
			msks[j] |= msks[i];
		}
	}

	int cnt = 0;
	for (int i = 0; i < N; i++) {
		if (ans[i] >= TH) {
			cout << i+2 << ' ' << ans[i] << '\n';
			cnt++;
		}
	}
	cout << cnt << '\n';

	return 0;
}
