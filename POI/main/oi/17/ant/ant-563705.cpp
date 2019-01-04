#include<iostream>

using namespace std;

typedef long long ll;

typedef pair<ll, ll> pii;

const ll BASE = 13; // i like 13, and it's a primitive root
const pii BS(BASE, BASE);
const ll MOD1 = 1e9 + 7;
const ll MOD2 = 1e9 + 9;
const pii MOD(MOD1, MOD2);

inline ll mod(ll a, ll m) {
	a %= m;
	if(a < 0) a += m;
	return a;
}
inline pii operator +(const pii &a, const pii &b) {
	return pii(a.first + b.first, a.second + b.second);
}
inline pii operator -(const pii &a, const pii &b) {
	return pii(a.first - b.first, a.second - b.second);
}
inline pii operator *(const pii &a, const pii &b) {
	return pii(a.first * b.first, a.second * b.second);
}
inline pii operator %(const pii &a, const pii &b) {
	return pii(mod(a.first, b.first), mod(a.second, b.second));
}

const int MAXN = 6e5;
int N;
char S[MAXN];

pii pref[MAXN];
pii suff[MAXN];

pii bases[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	cin >> S;

	bases[0] = pii(1, 1);
	for(int i = 1; i <= N; i++) {
		bases[i] = (bases[i - 1] * BS) % MOD;
	}

	cerr << "base done\n";

	pref[0] = suff[0] = pii(0, 0);
	for(int i = 0; i < N; i++) {
		ll v = S[i] - '0';
		pref[i + 1] = (pref[i] * BS + pii(v, v)) % MOD;
	}
	cerr << "pref done\n";
	for(int i = N - 1; i >= 0; i--) {
		ll v = S[i] - '0';
		v = !v;
		suff[i] = (suff[i + 1] * BS + pii(v, v)) % MOD;
	}
	cerr << "suff done\n";

	//cerr << "p\n";
	//for(int i = 0; i <= N; i++) cerr << i << ' ' << pref[i].first << ' ' << pref[i].second << '\n';
	//cerr << "s\n";
	//for(int i = 0; i <= N; i++) cerr << i << ' ' << suff[i].first << ' ' << suff[i].second << '\n';

	ll res = 0;
	for(int i = 1; i < N; i++) {
		int mi = 0;
		int ma = min(i, N - i) + 1;
		while(ma - mi > 1) {
			int md = (mi + ma) / 2;
			if((pref[i] - pref[i - md] * bases[md]) % MOD == (suff[i] - suff[i + md] * bases[md]) % MOD) {
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

