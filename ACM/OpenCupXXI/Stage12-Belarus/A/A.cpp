#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;
	vector<array<array<bool, 2>, 2>> F(N);
	for (int i = 0; i < N; i++) {
		for (int a = 0; a < 2; a++) {
			for (int b = 0; b < 2; b++) {
				char c; cin >> c;
				assert(c == '0' || c == '1');
				F[i][a][b] = (c == '1');
			}
		}
	}
	const int S = 1 << N;
	vector<int> A(S);
	vector<int> B(S);
	for (int i = 0; i < S; i++) {
		cin >> A[i];
	}
	for (int i = 0; i < S; i++) {
		cin >> B[i];
	}

	vector<bool> flip_ans(N, false);
	vector<bool> is_xor(N, false);

	for (int i = 0; i < N; i++) {
		auto& f = F[i];
		int s = f[0][0] + f[0][1] + f[1][0] + f[1][1];
		if (s >= 3 || (s == 2 && f[0][0])) {
			flip_ans[i] = true;
			for (int a = 0; a < 2; a++) {
				for (int b = 0; b < 2; b++) {
					f[a][b] ^= 1;
				}
			}
		}
		s = f[0][0] + f[0][1] + f[1][0] + f[1][1];
		assert(s <= 2);
		if (s == 0) {
			for (int x = 0; x < S; x++) {
				if (x & (1 << i)) continue;
				int y = x | (1 << i);
				tie(A[x], A[y]) = make_pair(A[x] + A[y], 0);
				tie(B[x], B[y]) = make_pair(B[x] + B[y], 0);
			}
		} else if (s == 1) {
			// decide whether to flip or not
			int hi_a = -1;
			int hi_b = -1;
			for (int a = 0; a < 2; a++) {
				for (int b = 0; b < 2; b++) {
					if (f[a][b] == 1) {
						hi_a = a;
						hi_b = b;
					}
				}
			}
			for (int x = 0; x < S; x++) {
				if (x & (1 << i)) continue;
				int y = x | (1 << i);
				tie(A[x], A[y]) = make_pair(A[x] + A[y], A[hi_a ? y : x]);
				tie(B[x], B[y]) = make_pair(B[x] + B[y], B[hi_b ? y : x]);
			}
		} else {
			assert(s == 2);
			assert(!f[0][0]);
			if (f[0][0] == f[1][1]) {
				is_xor[i] = true;
				for (int x = 0; x < S; x++) {
					if (x & (1 << i)) continue;
					int y = x | (1 << i);
					tie(A[x], A[y]) = make_pair(A[x] + A[y], A[x] - A[y]);
					tie(B[x], B[y]) = make_pair(B[x] + B[y], B[x] - B[y]);
				}
			} else if (f[0][0] == f[0][1]) {
				for (int x = 0; x < S; x++) {
					if (x & (1 << i)) continue;
					int y = x | (1 << i);
					tie(A[x], A[y]) = make_pair(A[x] + A[y], A[y]);
					tie(B[x], B[y]) = make_pair(B[x] + B[y], B[x] + B[y]);
				}
			} else {
				for (int x = 0; x < S; x++) {
					if (x & (1 << i)) continue;
					int y = x | (1 << i);
					tie(A[x], A[y]) = make_pair(A[x] + A[y], A[x] + A[y]);
					tie(B[x], B[y]) = make_pair(B[x] + B[y], B[y]);
				}
			}
		}
	}

	vector<int64_t> C(S);
	for (int x = 0; x < S; x++) {
		C[x] = int64_t(A[x]) * int64_t(B[x]);
	}
	for (int i = 0; i < N; i++) {
		for (int x = 0; x < S; x++) {
			if (x & (1 << i)) continue;
			int y = x | (1 << i);
			if (is_xor[i]) {
				tie(C[x], C[y]) = make_pair((C[x] + C[y]) / 2, (C[x] - C[y]) / 2);
			} else {
				tie(C[x], C[y]) = make_pair(C[x] - C[y], C[y]);
			}
			if (flip_ans[i]) swap(C[x], C[y]);
		}
	}

	for (int x = 0; x < S; x++) {
		cout << C[x] << " ";
	}
	cout << '\n';

	return 0;
}
