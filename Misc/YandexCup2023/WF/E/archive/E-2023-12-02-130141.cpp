#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }

		std::vector<int> initial_cnt(N);
		for (int i = 0; i < N; i++) initial_cnt[A[i]]++;
		if (N % 2 == 1) {
			bool good = true;
			for (int i = 0; i < N; i++) {
				if (initial_cnt[i] != 1) {
					good = false;
					break;
				}
			}
			// m[i][j] = f(i+j)
			// f(2*i) = A[i]
			// m[i][j] = f((i+j)/ 2 % N);
			if (good) {
				for (int i = 0; i < N; i++) {
					for (int j = 0; j < N; j++) {
						int c = i+j;
						if (c >= N) c -= N;
						if (c & 1) c += N;
						c /= 2;
						cout << A[c/2]+1 << " \n"[j+1==N];
					}
				}
			} else {
				cout << -1 << '\n';
			}
		} else {
			bool good = true;
			for (int i = 0; i < N; i++) {
				if (initial_cnt[i] & 1) {
					good = false;
					break;
				}
			}
			if (!good) {
				cout << -1 << '\n';
				continue;
			}
			if (N == 2) {
				cout << 1+A[0] << ' ' << 1+!A[0] << '\n';
				cout << 1+!A[1] << ' ' << 1+A[1] << '\n';
				continue;
			}
		}
	}

	return 0;
}
