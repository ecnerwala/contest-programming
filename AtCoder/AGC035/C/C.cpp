#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	if (N == 1) {
		cout << "No\n";
		exit(0);
	}
	if ((N & (N-1)) == 0) {
		cout << "No\n";
		exit(0);
	}

	cout << "Yes\n";

	for (int i = 2; i+1 <= N; i += 2) {
		cout << 1 << ' ' << i << '\n';
		cout << i << ' ' << i+1 << '\n';
		cout << 1 << ' ' << N+i+1 << '\n';
		cout << N+i+1 << ' ' << N+i << '\n';
	}
	cout << 3 << ' ' << N+1 << '\n';

	if (N % 2 == 0) {
		int a = 1 << (31 - __builtin_clz(N));
		int b = N ^ a ^ 1;
		cout << ((a % 2 == 0) ? a : (N+a)) << ' ' << N << '\n';
		cout << ((b % 2 == 0) ? b : (N+b)) << ' ' << N+N << '\n';
	}

	return 0;
}
