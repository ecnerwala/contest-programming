#include <bits/stdc++.h>

bool winning_fast(int n, int m) {
	if ((m & 1) == (n & 1) || ((m >> (n-1)) & 1) == (n & 1)) return n & 1;
	else return !(n & 1);
}

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
			assert(winning[n][m] == winning_fast(n, m));
		}
	}
}

// if it's odd, it's the max of the first and last, otherwise it's the min

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	slow(20);
	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }
	}

	return 0;
}
