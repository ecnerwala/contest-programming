#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 6e3;
int N;
int D;

ll V[MAXN];

ll C[MAXN];

ll go() {

	for(int i = 0; i + 2 * D < N; i++) {
		if(V[i] != V[i + 2 * D]) {
			return -1;
		}
	}

	assert(N >= 2 * D);

	N = 2 * D;
	V[N] = V[0];

	//cerr << D << '\n';
	//for(int i = 0; i < 2 * D; i++) { cerr << V[i] << ' '; } cerr << '\n';

	for(int i = 0; i < N; i++) {
		C[i] = V[i + 1] - V[i];
	}

	ll res = 0;

	for(int d = D; d >= 1; d /= 2) {
		//for(int i = 0; i < 2 * d; i++) cerr << V[i] << ' '; cerr << '\n';
		for(int i = 0; i < d; i++) {
			ll diff = C[i] - C[i + d];
			if(diff % 2 != 0) {
				cerr << "hi\n";
				return -1;
			}
			diff /= 2;
			res += abs(diff);
			C[i] -= diff;
			C[i + d] += diff;
			for(int j = 0; j < N; j++) {
				if(bool((j - i - 1 + d) & d) == (diff > 0)) {
					V[j] -= abs(diff);
				}
			}
		}
	}

	for(int i = 0; i < N; i++) {
		if(V[i] < 0) return -1;
	}

	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;
	for(int t = 1; t <= T; t++) {
		cin >> N >> D;
		for(int i = 0; i < N; i++) {
			cin >> V[i];
		}
		ll res = go();
		//cerr << res << '\n';
		//cerr << '\n';

		cout << "Case #" << t << ": ";
		if(res == -1) {
			cout << "CHEATERS!" << '\n';
		} else {
			cout << res << '\n';
		}

	}
	return 0;
}
