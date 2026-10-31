#include <bits/stdc++.h>

int main() {
	using namespace std;
	std::mt19937 mt(48);
	int T = 100;
	cout << T << '\n';
	while (T--) {
		int N = 100, Q = 150;
		cout << N << ' ' << Q << '\n';
		for (int i = 0; i < N; i++) {
			cout << ((1 << (mt() % 30)) | (1 << (mt() % 30))) << " \n"[i+1==N];
		}
		for (int q = 0; q < Q; q++) {
			int l = mt() % N + 1;
			int r = mt() % N + 1;
			if (l > r) swap(l, r);
			int u = mt() % (r-l+1) + 1;
			int v = mt() % (r-l+1) + 1;
			if (u > v) swap(u, v);
			cout << l << ' ' << r << ' ' << u << ' ' << v << '\n';
		}
	}
}
