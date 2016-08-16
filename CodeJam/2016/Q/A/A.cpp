#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

ll go(ll N) {
	if(N == 0) return -1;
	ll msk = 0;
	//while(N % 10 == 0) msk |= 1, N /= 10;
	ll res = 0;
	for(; true; res += N) {
		for(ll v = res; v; v /= 10) {
			msk |= (1 << (v % 10));
		}
		if(msk == 1023) return res;
	}
	return -1;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;
	for(int case_num = 1; case_num <= T; case_num++) {
		int N; cin >> N;
		ll res = go(N);
		cout << "Case #" << case_num << ": ";
		if(res == -1) cout << "INSOMNIA";
		else cout << res;
		cout << '\n';
	}

	return 0;
}
