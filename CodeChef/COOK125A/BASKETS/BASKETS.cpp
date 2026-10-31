#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int M = 24;
		vector<int> C(M);
		for (int i = 0; i < M; i++) {
			C[i] = min((2 << i), N);
		}
		cout << M << '\n';
		for (int i = 0; i < M; i++) {
			cout << C[i] << " \n"[i+1==M];
		}

		vector<int> cur_val(M);
		int i = 0;
		for (int t = 1; t <= N; t++) {
			while (t > C[i]) { i++; }

			int K = 4;
			cout << K << '\n';
			int j = i;
			while (K > 0) {
				if (cur_val[j] < t) {
					cur_val[j]++;
					K--;
					cout << cur_val[j] << ' ' << j+1 << '\n';
				} else {
					j++;
				}
			}
		}
	}

	return 0;
}
