#include <bits/stdc++.h>

int main() {
	using namespace std;
	int N = int(1e5);
	int Q = int(1e5);
	cout << N << ' ' << Q << '\n';
	for (int i = 0; i < N-1; i++) {
		int u = i, v = i+1;
		cout << u+1 << ' ' << v+1 << '\n';
	}
	for (int q = 0; q < Q; q++) {
		int a = 0, v = N-1;
		if (q & 1) cout << '+' << ' ' << a << ' ' << v << ' ' << int(1e4) << '\n';
		else cout << '?' << ' ' << a << ' ' << v << '\n';
	}
}
