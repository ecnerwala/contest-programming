#include<bits/stdc++.h>
using namespace std;

int sum[600];
double dp[2][600];
double val[2][600][60]; // minimizing and maximizing
int mve[2][600][60];

const double INF = 1e18;

void setmin(double& a, double b) {
	if (b < a) a = b;
}
void setmax(double& a, double b) {
	if (b > a) a = b;
}

void go() {
	for (int m = 0; m < 512; m++) {
		for (int i = 0; i < 9; i++) {
			if (m & (1 << i)) {
				sum[m] += i+1;
			}
		}
	}
	for (int m = 0; m < 512; m++) {
		int v = 0;
		for (int i = 0; i < 9; i++) {
			if (m & (1 << i)) {
				v *= 10;
				v += i+1;
			}
		}
		for (int s = 0; s < 60; s++) {
			mve[0][m][s] = -1;
			mve[1][m][s] = -1;
			val[0][m][s] = +INF;
			val[1][m][s] = -INF;
		}
		// nonempty subsets
		for (int k = m; k > 0; k = m & (k-1)) {
			int s = sum[k];
			assert(s > 0);
			assert((m ^ k) < m);
			if (dp[0][m^k] < val[0][m][s]) {
				val[0][m][s] = dp[0][m^k];
				mve[0][m][s] = k;
			}
			if (dp[1][m^k] > val[1][m][s]) {
				val[1][m][s] = dp[1][m^k];
				mve[1][m][s] = k;
			}
		}
		for (int s = 0; s < 60; s++) {
			if (mve[0][m][s] == -1) {
				val[0][m][s] = v;
			}
			if (mve[1][m][s] == -1) {
				val[1][m][s] = v;
			}
		}
		dp[0][m] = dp[1][m] = 0;
		for (int a = 1; a <= 6; a++) {
			for (int b = 1; b <= 6; b++) {
				dp[0][m] += val[0][m][a+b];
				dp[1][m] += val[1][m][a+b];
			}
		}
		dp[0][m] /= 36;
		dp[1][m] /= 36;
	}
}

int toMask(string s) {
	int msk = 0;
	for (char c : s) {
		assert('1' <= c && c <= '9');
		msk |= (1 << (c -'1'));
	}
	return msk;
}

string fromMask(int m) {
	if (m == -1) return "-1";
	string res;
	for (int i = 0; i < 9; i++) {
		if (m & (1 << i)) {
			res += char('1' + i);
		}
	}
	return res;
}

int main() {
	go();
	string str; int a, b; cin >> str >> a >> b;
	int s = a+b;
	int m = toMask(str);
	cout << fromMask(mve[0][m][s]) << ' ' << fixed << setprecision(5) << val[0][m][s] << '\n';
	cout << fromMask(mve[1][m][s]) << ' ' << fixed << setprecision(5) << val[1][m][s] << '\n';

	return 0;
}
