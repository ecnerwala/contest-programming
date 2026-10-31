#include<bits/stdc++.h>
using namespace std;

using ll = long long;

ll A[61]; // number of movements of a given size
ll B[61]; // number of collections of movemenets 
ll C[61];

ll F[61]; // number of flips of a given size
ll FS[61][61]; // number of flip sequences of a given length with a given number of flips

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	A[0] = 0;
	B[0] = 1;
	C[0] = 0;
	F[0] = 0;
	FS[0][0] = 1;

	for (int i = 1; i <= 60; i++) {
		F[i] = B[i-1];
		FS[i][0] = 0;
		for (int k = 1; k <= i; k++) {
			FS[i][k] = 0;
			for (int j = 1; j <= i; j++) {
				FS[i][k] += F[j] * FS[i-j][k-1];
			}
		}

		for (int k = 1; k <= i; k++) {
			ll cur = 0;
			for (int j = 0; j < k; j++) {
				int p = k / __gcd(j, k);
				if (i % p != 0) continue;
				cur += FS[i / p][k / p];
			}
			assert(cur % k == 0);
			cur /= k;
			cerr << i << ' ' << k << ' ' << cur << '\n';
			A[i] += cur;
		}

		C[i] = 0;
		for (int d = 1; d <= i; d++) {
			if (i % d == 0) C[i] += A[d] * d;
		}

		B[i] = 0;
		for (int k = 0; k < i; k++) {
			B[i] += B[k] * C[i-k];
		}
		assert(B[i] % i == 0);
		B[i] /= i;
		cerr << i << '\n';
		cerr << "F = " << F[i] << " figures" << '\n';
		cerr << "A = " << A[i] << " movements" << '\n';
		cerr << "B = " << B[i] << " movement collections" << '\n';
		cerr << "C = " << C[i] << " movement stuff" << '\n';
	}

	//for (int i = 1; i <= 60; i++) cerr << i << ' ' << A[i] << '\n';

	for (int casenum = 1; true; casenum++) {
		int N; cin >> N;
		if (N == 0) exit(0);
		cout << "Case " << casenum << ": " << A[N] << '\n';
	}

	return 0;
}
