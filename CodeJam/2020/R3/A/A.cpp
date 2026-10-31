#include<bits/stdc++.h>

int edit_dist(std::string A, std::string B) {
	using namespace std;
	int NA = int(A.size());
	int NB = int(B.size());
	vector<vector<int>> dp(NA+1, vector<int>(NB+1, int(1e9)));
	vector<vector<int>> prv(NA+1, vector<int>(NB+1, 0));
	dp[0][0] = 0;
	for (int i = 0; i <= NA; i++) {
		for (int j = 0; j <= NB; j++) {
			if (i < NA) {
				int cnd = dp[i][j] + 1;
				if (cnd < dp[i+1][j]) {
					dp[i+1][j] = cnd;
					prv[i+1][j] = 1;
				}
			}
			if (j < NB) {
				int cnd = dp[i][j] + 1;
				if (cnd < dp[i][j+1]) {
					dp[i][j+1] = cnd;
					prv[i][j+1] = 2;
				}
			}
			if (i < NA && j < NB) {
				int cnd = dp[i][j] + (A[i] != B[j]);
				if (cnd < dp[i+1][j+1]) {
					dp[i+1][j+1] = cnd;
					prv[i+1][j+1] = 3;
				}
			}
		}
	}

	return dp[NA][NB];
}

std::string solve(std::string A, std::string B) {
	using namespace std;
	int NA = int(A.size());
	int NB = int(B.size());
	vector<vector<int>> dp(NA+1, vector<int>(NB+1, NA+NB+2));
	vector<vector<int>> prv(NA+1, vector<int>(NB+1, 0));
	dp[0][0] = 0;
	for (int i = 0; i <= NA; i++) {
		for (int j = 0; j <= NB; j++) {
			if (i < NA) {
				int cnd = dp[i][j] + 1;
				if (cnd < dp[i+1][j]) {
					dp[i+1][j] = cnd;
					prv[i+1][j] = 1;
				}
			}
			if (j < NB) {
				int cnd = dp[i][j] + 1;
				if (cnd < dp[i][j+1]) {
					dp[i][j+1] = cnd;
					prv[i][j+1] = 2;
				}
			}
			if (i < NA && j < NB) {
				int cnd = dp[i][j] + (A[i] != B[j]);
				if (cnd < dp[i+1][j+1]) {
					dp[i+1][j+1] = cnd;
					prv[i+1][j+1] = 3;
				}
			}
		}
	}

	int dist = dp[NA][NB];

	int curI = NA, curJ = NB;
	while (true) {
		if (dp[curI][curJ] == dist / 2) {
			return A.substr(0, curI) + B.substr(curJ);
		}
		int p = prv[curI][curJ];
		assert(p);
		if (p & 1) curI--;
		if (p & 2) curJ--;
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		string A, B; cin >> A >> B;

		string res = solve(A, B);
		int d0 = edit_dist(A, B);
		int x = edit_dist(A, res);
		int y = edit_dist(B, res);
		assert(d0 == x + y);
		assert(abs(x - y) <= 1);

		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
