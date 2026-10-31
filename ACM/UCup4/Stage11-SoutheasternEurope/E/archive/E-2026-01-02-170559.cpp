#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int K; cin >> K;
	int N = 2 * K + 1;
	cout << N << '\n';
	for (int a = 0; a < N; a++) {
		for (int i = 1; i <= K; i++) {
			cout << (a+i)%N+1 << '\n';
		}
	}

	return 0;
}
