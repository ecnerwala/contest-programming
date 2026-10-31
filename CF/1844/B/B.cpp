#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		if (N == 1) {
			A[0] = 1;
		} else if (N == 2) {
			A[0] = 1;
			A[1] = 2;
		} else {
			A[0] = 2;
			A[N/2] = 1;
			A[N-1] = 3;
			int v = 4;
			for (int i = 0; i < N; i++) {
				if (A[i] == 0) {
					A[i] = v++;
				}
			}
			assert(v == N+1);
		}
		for (int i = 0; i < N; i++) {
			cout << A[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
