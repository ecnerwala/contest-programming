#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		cout << "Case #" << case_num << ":";
		int N; cin >> N;
		string S; cin >> S;
		char last = 'Z';
		int v = 0;
		for (int i = 0; i < N; i++) {
			char c = S[i];
			if (c > last) {
				v++;
			} else {
				v = 1;
			}
			last = c;
			cout << ' ' << v;
		}
		cout << '\n';

	}

	return 0;
}
