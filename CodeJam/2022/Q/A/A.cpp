#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int R, C; cin >> R >> C;
		cout << "Case #" << case_num << ":" << '\n';
		for (int i = 0; i < 2*R+1; i++) {
			for (int j = 0; j < 2*C+1; j++) {
				if (i < 2 && j < 2) {
					cout << '.';
				} else if (i % 2 == j % 2) {
					cout << "+."[i&1];
				} else if (i & 1) {
					cout << '|';
				} else {
					cout << "-";
				}
			}
			cout << '\n';
		}
	}

	return 0;
}
