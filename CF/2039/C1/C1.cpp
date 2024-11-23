#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int X; int64_t M; cin >> X >> M;
		// if Y >= 2 * X, then X ^ Y > Y/2 >= X so it's not a divisor of Y or Y
		int cnt = 0;
		for (int Y = 0; Y < 2 * X && Y <= M; Y++) {
			if (X == Y) continue;
			if (Y % (X ^ Y) == 0 || X % (X ^ Y) == 0) {
				cnt++;
			}
		}
		cout << cnt << '\n';
	}

	return 0;
}
