#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, X; cin >> N >> X;
		vector<int> A(N);
		for (auto& a : A) cin >> a;

		int must_burn = N == 2 || X == 1;

		for (int i = 0; i+1 < N; i++) {
			for (int l = 29; l >= 0; l--) {
				if (X > 0 && (A[i] & (1 << l))) {
					int j = i+1;
					while (j+1 < N && !(A[j] & (1 << l))) j++;
					A[i] ^= (1 << l);
					A[j] ^= (1 << l);
					X--;
				}
			}
		}

		if (must_burn && (X & 1)) {
			A[N-2] ^= 1;
			A[N-1] ^= 1;
		}

		for (int i = 0; i < N; i++) {
			cout << A[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
