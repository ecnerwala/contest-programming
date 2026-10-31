#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	if (fopen("folding.in", "r")) {
		freopen("folding.in", "r", stdin);
		freopen("folding.out", "w", stdout);
	}

	int W, H; cin >> W >> H;
	int w, h; cin >> w >> h;

	int ans = -1;
	for (int z = 0; z < 2; z++, std::swap(w, h)) {
		int cnd = 0;
		for (int v = W; v > w; v++, v /= 2) {
			cnd++;
		}
		for (int v = H; v > h; v++, v /= 2) {
			cnd++;
		}
		if (ans == -1 || cnd < ans) ans = cnd;
	}
	cout << ans << '\n';

	return 0;
}
