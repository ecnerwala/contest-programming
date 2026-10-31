#include <bits/stdc++.h>
using namespace std;

int main() {
	int T = 100;
	cout << T << '\n';
	for (int t = 1; t <= T; t++) {
		int N = (t <= 20 ? 5000 : 1000);
		cout << N << ' ' << int(1e9) << ' ' << int(1e9) << ' ' << int(1e9) << '\n';
		for (int i = 0; i < N; i++) {
			cout << 1 << " \n"[i+1==N];
		}
	}
}
