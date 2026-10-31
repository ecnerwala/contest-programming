#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t N, X; int Y; cin >> N >> X >> Y;
	if (!(N & 1)) {
		cout << 0 << '\n';
	} else {
		int ans = 0;
		for (int64_t y = Y; true; y = Y & (y-1)) {
			for (int b = 0; y >> b; b++) {
				if ((y >> b) & 1) {
					if (((X - (1<<b)) | (y*N - (1 << b))) == (y*N - (1 << b)))  {
						ans ^= (1 << b);
					}
				}
			}
			if (y == 0) break;
		}
		cout << ans << '\n';
	}

	return 0;
}
