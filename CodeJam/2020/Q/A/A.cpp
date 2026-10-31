#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		vector<vector<int>> M(N, vector<int>(N));
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cin >> M[i][j];
			}
		}


		int k = 0;
		for (int i = 0; i < N; i++) k += M[i][i];

		int r = 0;
		vector<bool> seen(N);
		for (int i = 0; i < N; i++) {
			fill(seen.begin(), seen.end(), 0);
			bool repeat = false;
			for (int j = 0; j < N; j++) {
				if (seen[M[i][j]-1]) repeat = true;
				seen[M[i][j]-1] = true;
			}
			r += repeat;
		}

		int c = 0;
		for (int j = 0; j < N; j++) {
			fill(seen.begin(), seen.end(), 0);
			bool repeat = false;
			for (int i = 0; i < N; i++) {
				if (seen[M[i][j]-1]) repeat = true;
				seen[M[i][j]-1] = true;
			}
			c += repeat;
		}

		cout << "Case #" << case_num << ": " << k << ' ' << r << ' ' << c << '\n';
	}

	return 0;
}
