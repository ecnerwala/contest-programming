#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;
	if (N == 1) {
		cout << 1 << '\n';
	} else if (N == 2) {
		cout << -1 << '\n';
	} else {
		cout << 1 << ' ' << 3 << ' ' << 2;
		for (int j = 4; j <= N; j++) cout << ' ' << j;
		cout << '\n';
	}

	return 0;
}
