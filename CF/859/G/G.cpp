#include<bits/stdc++.h>
using namespace std;

typedef complex<double> pt;

const double PI2 = atan2(1, 0);
const double PI = PI2 * 2;
const double TAU = PI2 * 4;

const int MAXN = 4e5;
int N;
char S[MAXN];
int main() {
	cin >> N;
	cin >> S;
	pt sum = 0;
	for (int i = 0; i < N; i++) {
		int v = S[i] - '0';
		sum += polar(double(v), double(i * TAU / N));
	}
	if (abs(sum) <= 1e-8) {
		cout << "YES\n";
	} else {
		cout << "NO\n";
	}

	return 0;
}
