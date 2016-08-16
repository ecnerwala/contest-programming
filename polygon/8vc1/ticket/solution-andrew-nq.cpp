#include<bits/stdc++.h>
using namespace std;

typedef long double ld;

const ld INF = 1e99;

const int MAXN = 2e5 + 20;

int N, T, Q;

int P[MAXN]; // prize in ith raffle
int O[MAXN]; // number of other tickets in i'th raffle
int R[MAXN]; // total number of tickets in i'th raffle

int main() {

	ios_base::sync_with_stdio(0);

	cin >> N >> T >> Q;

	for(int i = 0; i < N; i++) cin >> P[i];

	for (int i = 0; i < N; i++) cin >> O[i];

	for(int q = 0; q < Q; q++) {
		int t, r; cin >> t >> r;
		r--;
		if(t == 1) {
			O[r] ++;
		} else if(t == 2) {
			assert(O[r]);
			O[r] --;
		} else assert(false);

		priority_queue< pair<ld, int> > pq;

		ld res = 0;

		for(int i = 0; i < N; i++) {
			R[i] = O[i];
			assert(O[i]);
			assert(R[i] < 2 * O[i]);
			pq.emplace(ld(P[i]) * O[i] / (R[i]) / (R[i] + 1), i);
		}

		for(int z = 0; !pq.empty() && z < T; z++) {
			int i = pq.top().second;
			res += pq.top().first;

			pq.pop();

			R[i] ++;

            if (R[i] < 2 * O[i]) {
				pq.emplace(ld(P[i]) * O[i] / (R[i]) / (R[i] + 1), i);
			}
		}

		cout << fixed << setprecision(10) << res << '\n';
	}
	return 0;
}
