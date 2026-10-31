#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		if (K & 1) {
			for (int i = 0; i < K; i++) {
				cout << N << " \n"[i+1==K];
			}
		} else {
			std::vector<int> A(K);
			int num_smaller = 0;
			for (int b = 30; b >= 0; b--) {
				if ((N >> b) & 1) {
					if (num_smaller < K) num_smaller++;
					for (int i = 0; i < K; i++) {
						if (i == num_smaller-1) continue;
						A[i] += (1 << b);
					}
				} else {
					for (int i = 0; i < num_smaller / 2 * 2; i++) {
						A[i] += (1 << b);
					}
				}
			}
			for (int i = 0; i < K; i++) {
				cout << A[i] << " \n"[i+1==K];
			}
		}
	}

	return 0;
}
