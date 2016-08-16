#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;

int main(int argc, char * argv[]) {

	registerGen(argc, argv, 1);

	int N = atoi(argv[1]);
	int T = atoi(argv[2]);
	int Q = atoi(argv[3]);
	int type = atoi(argv[4]);

	vector<int> P(N);
	vector<int> L(N);
	vector<int> cnt(N);
	vector<pii> queries(Q); // (1/2, [0,N-1))

	if(type == 1) {

		// equal value, even tickets
		P[0] = rnd.next(1, 1000);
		for(int i = 1; i < N; i++) P[i] = P[0];

		for(int i = 0; i < N; i++) L[i] = 1;

		for(int i = 0; i < Q; i++) {
			queries[i] = pii(1, i % N);
		}

	} else if(type == 2) {

		// very uneven value
		P[0] = 1000;
		for(int i = 0; i < N; i++) P[i] = 1;

		for(int i = 0; i < N; i++) L[i] = 1;

		for(int i = 0; i < Q; i++) {
			if(rnd.next(0, 5) == 0) queries[i] = pii(1, 0);
			else queries[i] = pii(1, rnd.next(1, N - 1));
		}

	} else if(type == 3) {

		for(int i = 0; i < N; i++) P[i] = rnd.next(1, 1000);
		for(int i = 0; i < N; i++) L[i] = rnd.next(1, 1000);

		int r = rnd.next(0, N - 1);
		for(int i = 0; i < Q; i++) {
			queries[i] = pii(1 + (i % 2), r);
		}

	} else if(type == 4) {

		P[0] = 1000;
		for(int i = 1; i < N; i++) P[i] = 1;
		for(int i = 0; i < N; i++) L[i] = rnd.next(1, 1000);

		int r = rnd.next(0, N - 1);
		for(int i = 0; i < Q; i++) {
			if(i < (Q + 2) / 3) {
				queries[i] = pii(1, r);
			} else if(i < 2 * Q / 3) {
				queries[i] = pii(2, r);
			} else {
				queries[i] = pii(1, r);
			}
		}

	} else {
		// just do random
		for(int i = 0; i < N; i++) P[i] = rnd.next(1, 1000);
		for(int i = 0; i < N; i++) cnt[i] = L[i] = rnd.next(1, 1000);
		for(int i = 0; i < Q; i++) {
			int t, r;
			do {
				t = (rnd.next(1, 3) > 2) + 1;
				r = rnd.next(0, N - 1);
			} while (t == 2 && cnt[r] == 1);
			queries[i] = pii(t, r);
		}
	}

	cout << N << ' ' << T << ' ' << Q << '\n';

	for(int i = 0; i < N; i++) {
		if(i) cout << ' ';
		cout << P[i];
	}
	cout << '\n';

	for(int i = 0; i < N; i++) {
		if(i) cout << ' ';
		cout << L[i];
	}
	cout << '\n';

	for(int i = 0; i < Q; i++) {
		cout << queries[i].first << ' ' << queries[i].second + 1 << '\n';
	}

}
