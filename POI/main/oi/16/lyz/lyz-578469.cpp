#include<bits/stdc++.h>

using namespace std;

typedef long long ll;

const int MAXN = 3e5;
const int MAXM = 1e6;

const int L = 18; // log N, depth of seg tree
const int S = (1 << L); // size of bottom row

int N, M;

ll K, D;

ll sum[2 * S];
ll pref[2 * S], suff[2 * S]; //stores max pref and max suff
ll val[2 * S];

void update(int i) {
	if(i >= S) {
		pref[i] = suff[i] = val[i] = max(sum[i], 0ll);
	} else {
		int l = 2 * i, r = 2 * i + 1;

		sum[i] = sum[l] + sum[r];

		pref[i] = max(pref[l], sum[l] + pref[r]);
		suff[i] = max(suff[r], suff[l] + sum[r]);

		val[i] = max(max(val[l], val[r]), suff[l] + pref[r]);
	}
}

int main() {
	ios_base::sync_with_stdio(0);

	cin >> N >> M >> K >> D;

	memset(sum, 0, sizeof(sum));
	for(int i = 0; i < N; i++) {
		sum[S + i] = -K;
	}
	for(int i = 2 * S - 1; i > 0; i--) {
		update(i);
	}

	for(int i = 0; i < M; i++) {
		int s; ll x;
		cin >> s >> x;
		sum[S + s] += x;
		for(s += S; s > 0; s /= 2) {
			update(s);
		}
		cout << ((val[1] <= D * K) ? "TAK" : "NIE") << '\n';
	}

	return 0;
}
