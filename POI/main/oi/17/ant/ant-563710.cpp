#include<iostream>

using namespace std;

typedef long long ll;

const ll BASE = 13; // i like 13, and it's a primitive root
const ll MOD = 1e9 + 9;

const int MAXN = 6e5;
int N;
char S[MAXN];

ll pref[MAXN];
ll suff[MAXN];

ll bases[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	cin >> S;

	bases[0] = 1;
	for(int i = 1; i <= N; i++) {
		bases[i] = (bases[i - 1] * BASE) % MOD;
	}
	//cerr << "base done\n";

	pref[0] = suff[0] = 0;
	for(int i = 0; i < N; i++) {
		ll v = S[i] - '0';
		pref[i + 1] = (pref[i] * BASE + v) % MOD;
	}
	//cerr << "pref done\n";
	for(int i = N - 1; i >= 0; i--) {
		ll v = S[i] - '0';
		v = !v;
		suff[i] = (suff[i + 1] * BASE + v) % MOD;
	}
	//cerr << "suff done\n";

	//cerr << "precomp done\n";

	//cerr << "p\n";
	//for(int i = 0; i <= N; i++) cerr << i << ' ' << pref[i][0] << ' ' << pref[i][1] << '\n';
	//cerr << "s\n";
	//for(int i = 0; i <= N; i++) cerr << i << ' ' << suff[i][0] << ' ' << suff[i][1] << '\n';

	ll res = 0;
	for(int i = 1; i < N; i++) {
		int mi = 0;
		int ma = min(i, N - i) + 1;
		while(ma - mi > 1) {
			int md = (mi + ma) / 2;
			if(
					((pref[i] - pref[i - md] * bases[md]) - 
					(suff[i] - suff[i + md] * bases[md])) % MOD == 0 
			  ) {
				mi = md;
			} else {
				ma = md;
			}
		}
		res += mi;
	}
	cout << res << '\n';
	return 0;
}

