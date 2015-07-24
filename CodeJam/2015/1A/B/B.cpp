#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll INF = 1e18;

int B;
ll N;
ll M[2000];

int go() {
	ll mi = -1;
	ll ma = INF;
	while(ma - mi > 1) {
		ll md = (mi + ma) / 2;
		ll cnt = 0;
		for(int i = 0; i < B; i++) {
			cnt += (md + M[i]) / M[i];
		}

		if(cnt >= N) ma = md;
		else mi = md;
	}
	ll cnt = 0;
	for(int i = 0; i < B; i++) {
		cnt += (mi + M[i]) / M[i];
	}
	assert(cnt < N);
	for(int i = 0; i < B; i++) {
		cnt += (ma + M[i]) / M[i] - (mi + M[i]) / M[i];
		if(cnt == N) return i;
	}
	assert(cnt >= N);
	assert(false);
	return -1;
}

int main() {
	int T;
	scanf("%d", &T);

	for(int t = 1; t <= T; t++) {
		scanf("%d%lld", &B, &N);
		for(int i = 0; i < B; i++) {
			scanf("%lld", &M[i]);
		}

		int res = go();
		printf("Case #%d: %d\n", t, res + 1);
	}
	return 0;
}
