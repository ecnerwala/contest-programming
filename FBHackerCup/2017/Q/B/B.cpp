#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e3;
int N;
int W[MAXN];

const int V = 50;

int ceil(int a, int b) {
	return (a + b - 1) / b;
}

int go() {
	sort(W, W + N);
	reverse(W, W + N);
	int res = 0;
	for(int l = 0, r = N; l < r; l++) {
		assert(W[l] > 0);
		if (W[l] * (r - l) >= 50) {
			res++;
			int cnt = ceil(50, W[l]);
			assert((r-l) >= cnt);
			assert(W[l] * cnt >= 50);
			assert(W[l] * (cnt - 1) < 50);
			r -= (cnt - 1);
		} else {
			break;
		}
	}
	assert(res >= 1);
	return res;
}

int main() {
	int T; cin >> T;
	for(int t = 1; t <= T; t++) {
		cout << "Case #" << t << ": ";
		cin >> N;
		assert (MAXN >= N * 2);
		for(int i = 0; i < N; i++) cin >> W[i];
		cout << go() << '\n';
	}

	return 0;
}
