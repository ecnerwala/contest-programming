#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

ll D[20];
ll cnt[20];

void add_num(ll v) {
	for(; v; v /= 10) cnt[v % 10] ++;
}

// generates counts of 0 .. N-1
void gen_cnt(ll N) {
	if(!N) return;
	ll V = N / 10;
	gen_cnt(V);
	for(int d = 0; d < 10; d++) cnt[d] *= 10, cnt[d] += V;
	for(ll v = V * 10; v < N; v++) {
		add_num(v);
	}
	if(V) cnt[0] --;
}

// can you number 1..N
bool is_good(ll N) {
	memset(cnt, 0, sizeof(cnt));
	gen_cnt(N + 1);
	for(int d = 0; d < 10; d++) {
		if(cnt[d] > D[d]) return false;
	}
	return true;
}

int main() {
	int T; cin >> T;
	while(T--) {

		for(int d = 0; d < 10; d ++) cin >> D[d];

		ll mi = 0, ma = 1e18;
		while(ma - mi > 1) {
			ll md = (mi + ma) / 2;
			if(is_good(md)) mi = md;
			else ma = md;
		}
		cout << mi << '\n';
	}

	return 0;
}
