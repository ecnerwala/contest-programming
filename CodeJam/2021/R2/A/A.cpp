#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T, N; cin >> T >> N;
	for (int case_num = 1; case_num <= T; case_num ++) {

		for (int i = 1; i < N; i++) {
			cout << 'M' << ' ' << i << ' ' << N << '\n' << flush;
			int p; cin >> p;
			if (p > i) {
				cout << 'S' << ' ' << i << ' ' << p << '\n' << flush;
				int resp; cin >> resp; assert(resp == 1);
			}
		}

		cout << 'D' << '\n' << flush;
		int resp; cin >> resp;
		if (resp == 1) continue;
		else if (resp == -1) exit(0);
		else assert(false);
	}

	return 0;
}
