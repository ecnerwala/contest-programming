#include <bits/stdc++.h>

int main() {
	using std::cout;
	int T = 2;
	int N = 5000;
	int K = 1e6;
	cout << T << ' ' << N << ' ' << K << '\n';
	for (int i = 0; i < N; i++) {
		cout << "GH"[i&1] << ' ' << 1000 * i << ' ' << 10*i << '\n';
	}
}
