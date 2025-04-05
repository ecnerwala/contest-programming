#include <bits/stdc++.h>

void slow(int N) {
	std::vector<std::vector<bool>> winning(N+1);
	for (int n = 1; n <= N; n++) {
		winning[n].resize(1 << n);
		for (int m = 0; m < (1 << n); m++) {
			if (n == 1) {
				winning[n][m] = m;
				continue;
			}
			winning[n][m] = false;
			for (int i = 0; i < n; i++) {
				// you can delete i if it's 1, or it's 0 and it's followed by a 0
				if (((m >> i) & 1) || (i+1 < n && (((m >> i) & 3) == 0))) {
					int nm = (m & ((1 << i) - 1)) | ((m >> (i+1)) << i);
					if (!winning[n-1][(1 << (n-1)) - 1 - nm]) { winning[n][m] = true; break; }
				}
			}
		}
		using std::cerr;
		for (int m = 0; m < (1 << n); m++) {
			for (int i = 0; i < n; i++) { cerr << ((m >> i) & 1); }
			cerr << ' ';
			cerr << winning[n][m] << '\n';
		}
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	slow(6);

	return 0;
}
