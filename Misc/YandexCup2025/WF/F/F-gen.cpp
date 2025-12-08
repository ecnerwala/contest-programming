#include <bits/stdc++.h>

int main() {
	std::ofstream fout("t-big.in");
	int T = 1;
	fout << T << '\n';
	for (int t = 0; t < T; t++) {
		int N = 1e5;
		int M = N;
		fout << N << ' ' << M << '\n';
		for (int n = 0; n < N; n++) fout << n+1 << " \n"[n+1==N];
		for (int m = 0; m < M; m++) fout << 0 << " \n"[m+1==M];
	}
}
