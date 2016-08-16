#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef long double ld;

const int MAXN = 2e5;
int N;
ll A, B;
ll C[MAXN];
ll tot;

ld go(ll M) {
	ld res = 0;
	for(ll i = 0; i < N; i++) {
		res += (M / tot) * ld(C[i]) * C[i] / 2;
	}
	M %= tot;
	for(ll i = 0; M && i < N; i++) {
		ll v = min(M, C[i]);
		res += ld(v) * v / 2;
		M -= v;
	}
	return res;
}

int main() {
	int __T; cin >> __T;
	for(int T = 1; T <= __T; T++) {
		cin >> N >> A >> B;
		tot = 0;
		for(int i = 0; i < N; i++) {
			cin >> C[i];
			tot += C[i];
		}

		B -= (A / tot) * tot;
		A %= tot;

		ld res = go(B) - go(A);

		res /= (B - A);

		cout << "Case #" << T << ": ";
		cout << fixed << setprecision(20) << res << '\n';
	}
	return 0;
}
