#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXP = 600;
const int MAXH = 60;
int P, H;
ll V[MAXP][MAXH];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> P >> H;
	for (int i = 0; i < P; i++) {
		for (int j = 0; j < H; j++) {
			cin >> V[i][j];
		}
		sort(V[i], V[i] + H);
	}
	for (int i = 0; i < P; i++) {
		map<ll, int> deltas;
		for (int j = 0; j < P; j++) {
			if (j == i) continue;
			//cerr << "Solve " << i << ' ' << j << '\n';
			set<ll> cutoffs;
			for (int k = 0; k < H; k++) {
				cutoffs.insert(V[i][k]);
				cutoffs.insert(V[j][k]);
			}
			ll prefI = 0, prefJ = 0;
			int indI = 0, indJ = 0;
			ll curX = 0;
			for (ll nxtX : cutoffs) {
				assert(indI == H || V[i][indI] >= nxtX);
				assert(indJ == H || V[j][indJ] >= nxtX);

				// d = i - j
				// increases if i >= j <=> d >= 0
				ll intercept = prefI - prefJ;
				ll slope = (H - indI) - (H - indJ);
				ll d0 = intercept + curX * slope;
				ll d1 = intercept + nxtX * slope;
				//cerr << curX << ' ' << d0 << ' ' << nxtX << ' ' << d1 << '\n';

				if ((d0 >= 0) == (d1 >= 0)) {
					// no change
				} else if (d0 >= 0 && d1 < 0) {
					// first one s.t. intercept + slope * x < 0 => intercept < (-slope) * x
					ll x = (intercept) / (-slope) + 1;
					deltas[x] --;
					//cerr << x << ' ' << -1 << '\n';
				} else if (d0 < 0 && d1 >= 0) {
					// find the first integer that's >=
					// intercept + slope * x >= 0
					ll x = (-intercept + slope - 1) / (slope);
					deltas[x] ++;
					//cerr << x << ' ' << +1 << '\n';
				} else assert(false);

				curX = nxtX;
				while (indI < H && V[i][indI] <= curX) {
					prefI += V[i][indI++];
				}
				while (indJ < H && V[j][indJ] <= curX) {
					prefJ += V[j][indJ++];
				}
			}
			assert(indI == H);
			assert(indJ == H);
		}
		int curRank = P;
		int bestRank = P;
		for (auto it : deltas) {
			curRank += it.second;
			bestRank = min(bestRank, curRank);
		}
		//cerr << i << ' ' << bestRank << '\n';
		cout << bestRank << '\n';
	}

	return 0;
}
