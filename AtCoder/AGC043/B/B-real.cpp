#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	assert(N >= 2);
	string s; cin >> s;
	vector<int> A(N);
	for (int i = 0; i < N; i++) { A[i] = s[i] - '1'; }

	N--; // number of iterations instead

	int ans2 = 0;
	for (int i = 0; i <= N; i++) {
		if ((N & i) == i) {
			ans2 += A[i]; ans2 %= 2;
		}
	}

	if (ans2) {
		cout << 1 << '\n';
		exit(0);
	} else {
		int ans4 = 0;
		for (int i = 0; i <= N; i++) {
			if (A[i] == 1) {
				cout << 0 << '\n';
				exit(0);
			}
			assert(A[i] % 2 == 0);
			if ((N & i) == i) {
				ans4 += A[i]; ans4 %= 4;
			}
		}
		cout << ans4 << '\n';
	}

	return 0;
}

// 2
// 02
// 002
// 0002
// 00002
// 000002
