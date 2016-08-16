#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll MOD = 1004535809;

const int MAXN = 5e4;

bool is_prime[MAXN * 200];
vector<ll> cnt;

vector<ll> mult(const vector<ll> &a, const vector<ll> &b) {
	vector<ll> res(a.size() + b.size() - 1);
	for(int i = 0; i < int(a.size()); i++) {
		for(int j = 0; j < int(b.size()); j++) {
			res[i + j] += a[i] * b[j];
			res[i + j] %= MOD;
		}
	}
	return res;
}

ll T(ll N, ll K) {

	is_prime[1] = false;
	for(ll i = 2; i < MAXN * 200; i++) is_prime[i] = true;
	ll prv = 1;
	for(ll i = 2; cnt.size() <= N; i++) {
		if(!is_prime[i]) continue;
		cnt.push_back(i - prv);
		prv = i;
		for(ll j = i * i; j < MAXN * 200; j += i) is_prime[j] = false;
	}

	cerr << "hi\n";

	vector<ll> res;
	res.push_back(1);

	while(K) {
		cerr << K << '\n';
		if(K % 2) {
			res = mult(res, cnt);
			res.resize(N + 1);
		}
		cnt = mult(cnt, cnt);
		cnt.resize(N + 1);
		K /= 2;
		assert(res.size() <= N + 1);
		assert(cnt.size() == N + 1);
	}

	return res[N];

}

int main() {
	cout << T(20000, 20000) << '\n';
	return 0;
}
