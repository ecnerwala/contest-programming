#include<iostream>

using namespace std;

typedef long long ll;

const int MAXN = 1e6 + 10;

int N;
int P, Q;
int X, Y;

char C[MAXN];
int pref[MAXN];

int minPref[MAXN];

int main() {
	cin >> N >> P >> Q >> X >> Y;
	cin >> C;
	pref[0] = 0;
	for(int i = 1; i <= N; i++) pref[i] = pref[i - 1] + (C[i - 1] == '+' ? 1 : -1) ;
	minPref[0] = 0;
	for(int i = 1; i <= N; i++) minPref[i] = min(minPref[i - 1], pref[i]);

	int suff = 0;
	int minSuff = 0;
	int res = X * N;
	for(int i = 0; i < N; i++) {
		int lowVal = min(minSuff, suff + minPref[N - i]) + P;
		int numAdd = (max(max(Q - P - pref[N], -lowVal), 0) + 1) / 2;

		int numSub = ((P + pref[N] + 2 * numAdd) - Q) / 2;

		int cost = (numSub + numAdd) * X + i * Y;
		if(cost < res) {
			//cerr << i << '\n';
			res = cost;
		}

		suff += (C[N - i - 1] == '+' ? 1 : -1);
		minSuff = min(minSuff, suff);
	}
	cout << res << '\n';
	return 0;
}
