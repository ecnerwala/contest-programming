#include<bits/stdc++.h>
using namespace std;

const int MAXV = 2e5;
double dp[50][50000];
double proba(int H, string D) {
	int X, Y, Z;
	istringstream str(D);
	char c;
	str >> X >> c >> Y;
	if(str >> c >> Z) {
		if (c == '+') {
			H -= Z;
		} else if (c == '-') {
			H += Z;
		} else assert(false);
	}

	if (H <= 0) return 1;

	if (H <= X) return 1;
	if (H > X * Y) return 0;

	assert(X < 50);
	assert(X * Y + Y < 50000);

	memset(dp, 0, sizeof(dp));
	dp[0][0] = 1;
	for (int d = 0; d < X; d++) {
		for (int s = 0; s <= X * Y; s++) {
			for(int v = 1; v <= Y; v++) {
				dp[d + 1][s + v] += dp[d][s] / Y;
			}
		}
	}

	double res = 0;
	for(int i = H; i <= X * Y; i++) {
		res += dp[X][i];
	}
	return res;
}

int main() {
	int T; cin >> T;
	for(int t = 1; t <= T; t++) {
		cout << "Case #" << t << ": ";
		int H, S;
		cin >> H >> S;
		double res = 0;
		for(int i = 0; i < S; i++) {
			string D;
			cin >> D;
			res = max(res, proba(H, D));
		}
		cout << fixed << setprecision(9) << res << '\n';
	}
	return 0;
}
