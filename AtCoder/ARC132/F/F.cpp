#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int K; cin >> K;
	const int S = 1 << (2*K);

	std::array<int, 2> N; cin >> N[0] >> N[1];

	std::vector<std::array<int, 2>> A(S, std::array<int, 2>{0, 0});
	for (int z = 0; z < 2; z++) {
		for (int it = 0; it < N[z]; it++) {
			string s; cin >> s;
			std::reverse(s.begin(), s.end());
			int v = 0;
			for (int i = 0; i < K; i++) {
				int c;
				if (s[i] == 'R') c = 1;
				else if (s[i] == 'S') c = 2;
				else if (s[i] == 'P') c = 3;
				else assert(false);
				v += c << (2*i);
			}
			A[v][z]++;
		}
	}
	for (int d = 0; d < K; d++) {
		for (int v = 0; v < S; v++) {
			if (!(v & (3 << (2*d)))) continue;
			A[v & ~(3 << (2*d))][0] += A[v][0];
			A[v & ~(3 << (2*d))][1] += A[v][1];
		}
	}

	int msk = 0x55555555;

	std::vector<int64_t> B(S);
	for (int v = 0; v < S; v++) {
		int cnt = __builtin_popcount(((v >> 1) | v) & msk);
		int coeff;
		if (cnt == 0) {
			coeff = 0;
		} else if (cnt & 1) {
			coeff = 1;
		} else {
			coeff = -1;
		}
		B[v] = int64_t(A[v][0]) * int64_t(A[v][1]) * coeff;
	}

	for (int d = 0; d < K; d++) {
		for (int v = 0; v < S; v++) {
			if (!(v & (3 << (2*d)))) continue;
			B[v] += B[v & ~(3 << (2*d))];
		}
	}

	for (int v = 0; v < S; v++) {
		int cnt = __builtin_popcount(((v >> 1) | v) & msk);
		if (cnt == K) {
			cout << B[v] << '\n';
		}
	}
	return 0;
}
