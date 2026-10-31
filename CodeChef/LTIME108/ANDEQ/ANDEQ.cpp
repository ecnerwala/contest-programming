#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (int i = 0; i < N; ++i) {
			cin >> A[i];
		}
		int tgt = A[0];
		for (int i = 1; i < N; i++) tgt &= A[i];

		int num_ranges = 0;

		int v = A[0];
		for (int i = 0; i < N; i++) {
			v &= A[i];
			if (v == tgt) {
				num_ranges++;
				v = int32_t(-1);
			}
		}
		cout << N - num_ranges << '\n';
	}

	return 0;
}
