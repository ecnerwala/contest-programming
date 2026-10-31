#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int X; int64_t M; cin >> X >> M;
		assert(X >= 1);
		// we want (X^Y) % X == 0 || (X^Y) % Y == 0
		int64_t cnt = 0;
		// count the first case first
		M++;
		for (int b = 0; M >> b; b++) {
			if ((M >> b) & 1) {
				int64_t lo = (M ^ X) >> (b+1) << (b+1);
				int64_t hi = lo + (int64_t(1) << b);
				cerr << lo << ' ' << hi << '\n';
				cnt += (hi + X-1) / X - (lo + X-1) / X;
			}
		}
		// Y >= (1 << (b+1)), then Y > (X ^ Y) / 2, so the second case can't happen
		{
			int b = 31 - __builtin_clz(X);
			for (int Y = 0; Y < (1 << (b+1)) && Y < M; Y++) {
				if ((X ^ Y) % X == 0) {
					cnt--;
				}
			}
			for (int Y = 1; Y < (1 << (b+1)) && Y < M; Y++) {
				if ((X ^ Y) % Y == 0 || (X ^ Y) % X == 0) {
					cnt++;
				}
			}
		}
		cout << cnt << '\n';
	}

	return 0;
}
