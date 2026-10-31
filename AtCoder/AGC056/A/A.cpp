#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;

	int cur = 0;
	for (int i = 0; i < N; i++) {
		int a, b, c;
		if (cur == N-1) {
			a = N-2;
			b = N-1;
			c = 0;
		} else if (cur == N-2) {
			a = N-1;
			b = 0;
			c = 1;
		} else {
			a = cur;
			b = cur+1;
			c = cur+2;
		}
		for (int j = 0; j < N; j++) {
			cout << (j == a || j == b || j == c ? '#' : '.');
		}
		cout << '\n';
		cur += 3;
		cur %= N;
	}

	return 0;
}
