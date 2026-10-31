#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int H, W; cin >> H >> W;
	int R, C; cin >> R >> C;
	cout << (R > 1) + (R < H) + (C > 1) + (C < W) << '\n';

	return 0;
}
