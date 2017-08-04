#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 1e18;
const int MAXN = 1e6 + 200;
int N;

ll P[MAXN];
ll D[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> P[i];
	}
	D[0] = 0;
	for (int i = 1; i < N; i++) {
		cin >> D[i];
	}
	ll res = 0;
	ll vlong = -INF;
	ll vshort = 0;
	for (int i = 0; i < N; i++) {
		vlong -= D[i];
		vshort -= D[i];
		vlong = max(vlong, -P[i]);
		vshort = max(vshort, P[i]);
		res = max(res, vlong + P[i]);
		res = max(res, vshort - P[i]);
	}

	cout << res << '\n';

	return 0;
}
