#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		vector<pair<pair<int, int>, int>> A(N);
		for (int i = 0; i < N; i++) {
			int s, t; cin >> s >> t;
			A[i] = {{s, t}, i};
		}
		sort(A.begin(), A.end());

		string res(N, '\0');
		int curT[2] = {0,0};
		char curC[2] = {'C', 'J'};
		for (int z = 0; z < N; z++) {
			int s, t; tie(s, t) = A[z].first;
			int i = A[z].second;
			if (s >= curT[1]) {
				res[i] = curC[1];
				curT[1] = t;
			} else if (s >= curT[0]) {
				res[i] = curC[0];
				curT[0] = t;
				if (curT[0] > curT[1]) {
					swap(curT[0], curT[1]);
					swap(curC[0], curC[1]);
				}
			} else {
				res = "IMPOSSIBLE";
				break;
			}
		}

		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
