#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		vector<char> res;
		for (int i = 0; i < N; i++) {
			char c = char('A' + i);
			int p; cin >> p;
			for (int j = 0; j < p; j++) {
				res.push_back(c);
			}
		}

		cout << "Case #" << case_num << ":";

		int M = int(res.size());

		if (M % 2) {
			cout << ' ' << res.back();
		}
		for (int i = 0; i < M / 2; i++) {
			cout << ' ' << res[i] << res[i + M / 2];
		}
		cout << '\n';
	}

	return 0;
}
