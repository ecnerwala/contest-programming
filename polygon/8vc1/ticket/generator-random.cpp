#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 3e5;
const int MAXQ = 3e5;
int N, T, Q;

int P[MAXN];

int cutoffs[MAXN];

int main(int argc, char* argv[]) {
	registerGen(argc, argv, 1);

	N = atoi(argv[1]);
	T = atoi(argv[2]);
	Q = atoi(argv[3]);

	for(int i = 0; i < N; i++) {
		P[i] = rnd.next(1, 1000);
	}

	for(int i = 0; i < N; i++) {
		cutoffs[i + 1] = cutoffs[i] + P[i];
	}

	vector<int> cnt(N);
	for(int i = 0; i < N; i++) {
		cnt[i] = rnd.next(1, 1000);
	}

	cout << N << ' ' << T << ' ' << Q << '\n';
	for(int i = 0; i < N; i++) {
		cout << P[i];
		if(i + 1 < N) cout << ' ';
	}
	cout << '\n';
	for (int i = 0; i < N; i++) {
		cout << cnt[i];
		if(i + 1 < N) cout << ' ';
	}
	cout << '\n';

	for(int q = 0; q < Q; q++) {
		int t = (rnd.next(1, 3) > 2) + 1; // add:remove with frequency 2:1

		// weight by prize value
		int r = lower_bound(cutoffs, cutoffs + N + 1, rnd.next(1, cutoffs[N])) - cutoffs;
		assert(0 < r && r <= N);

		r--;

		// can't remove the first ticket
		if(t == 2 && cnt[r] == 1) {
			q--;
			continue;
		}

		if(t == 2) cnt[r] --;
		else if(t == 1) cnt[r] ++;
		else assert(false);

		r++;

		cout << t << ' ' << r << '\n';
	}

	return 0;
}
