#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	{
		const int L = 20;
		const int N = 1000;
		std::vector<std::vector<int>> vals(N+1, std::vector<int>(L+1));
		for (int n = 0; n <= N; n++) {
			for (int l = 0; l <= L; l++) {
				std::array<bool, 4> has{false, false, false, false};
				if (l > 0) {
					has[vals[n][l-1]] = true;
				}
				if (n >= (1 << l)) {
					has[vals[n-(1<<l)][l]] = true;
				}
				if (n >= (2 << l)) {
					has[vals[n-(2<<l)][l+1]] = true;
				}
				int v = 0;
				while (has[v]) v++;
				vals[n][l] = v;
			}
		}
		for (int l = 0; l <= 10; l++) {
			for (int n = 0; n <= 100; n++) {
				cerr << vals[n][l] << ' ';
			}
			cerr << '\n';
		}
	}

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		for (int i = 0; i < N; i++) {
			int64_t a, b, c, l, r; cin >> a >> b >> c >> l >> r;
			assert((c - b) == (b - a));
		}
	}

	return 0;
}
