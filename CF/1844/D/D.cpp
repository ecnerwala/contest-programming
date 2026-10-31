#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int a = 1;
		while (N % a == 0) a++;
		for (int z = 0; z < N; z++) {
			cout << char('a' + (z%a));
		}
		cout << '\n';
	}

	return 0;
}
