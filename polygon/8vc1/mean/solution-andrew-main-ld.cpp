#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef long double ld;

const int MAXN = 1e6;

int N;
ll X[MAXN];
ll P[MAXN]; // prefix sums

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for(int i = 0; i < N; i++) {
		cin >> X[i];
	}
	sort(X, X + N);

	P[0] = 0;
	for(int i = 0; i < N; i++) {
		P[i + 1] = P[i] + X[i];
	}

	int besti = -1, bestj = -1;
	ld res = -1;

	for(int i = 0; i < N; i++) {
		// fix the median element as i

		// binary search on the number of elements to take
		int mi = 0, ma = min(i, N - i - 1) + 1;
		while(ma - mi > 1) {
			int md = (mi + ma) / 2;
			if((X[N - md] + X[i - md]) * (2 * md + 1) >= 2 * ((P[N] - P[N - md]) + (P[i] - P[i - md]) + X[i])) {
				// this increases the mean, so it's good
				mi = md;
			} else {
				ma = md;
			}
		}

		ld val = (P[N] - P[N - mi]) + (P[i] - P[i - mi]) + X[i];
		val /= 2 * mi + 1;
		val -= X[i];
		if(res < val) {
			res = val;
			besti = i;
			bestj = mi;
		}
	}

	assert(besti != -1);

	cout << 2 * bestj + 1 << '\n';

	for(int j = -bestj; j <= 0; j++) {
		cout << X[besti + j] << ' ';
	}
	for(int j = -bestj; j < 0; j++) {
		cout << X[N + j] << ' ';
	}
	cout << '\n';

	return 0;
}
