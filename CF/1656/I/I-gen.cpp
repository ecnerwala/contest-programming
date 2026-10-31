#include <bits/stdc++.h>
int main() {
	using namespace std;
	cout << 1 << '\n';
	int M = int(3e5);
	int N = 1 + M/5 * 3;
	cout << N << ' ' << M << '\n';
	for (int i = 1; i < N; i += 3) {
		cout << 0 << ' ' << i << '\n';
		cout << 0 << ' ' << i+1 << '\n';
		cout << i+1 << ' ' << i << '\n';
		cout << 0 << ' ' << i+2 << '\n';
		cout << i+2 << ' ' << i << '\n';
	}
}
