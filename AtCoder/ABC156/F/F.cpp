#define NDEBUG
#include<bits/stdc++.h>
using namespace std;

// number of integer solutions to Ax + By <= C and x,y >= 0
long long lattice_cnt(long long A, long long B, long long C) {
	using ll = long long;

	assert(A >= 0 && B >= 0);
	if (C < 0) return 0;

	assert(A > 0 && B > 0);
	if (A > B) std::swap(A, B);
	assert(A <= B);

	ll ans = 0;
	while (A > 0) {
		assert(0 < A && A <= B);

		ll k = B/A;
		ll l = B%A;
		assert(B == k * A + l);

		if (C >= 0) {
			ll f = C/B;
			ll e = C%B / A;
			ll g = C%B % A;
			assert(C == f * B + e * A + g);
			assert(C == (f * k + e) * A + f * l + g);

			// either x + ky <= f*k+e
			// i.e. 0 <= x <= (f-y) * k + e
			// or x >= fk + e + 1 - ky
			// and Ax + (Ak+l) y <= C = (fk + e + 1) A + fl - A + g
			// Let z = x - (fk + e + 1 - ky)
			// Az + A(fk + e + 1 - ky) + Aky + ly <= C = A (fk + e + 1) + fl - A + g
			// Az + ly <= fl - A + g

			ans += (f+1) * (e+1) + (f+1) * f / 2 * k;

			//C = f*l - A + g;
			C -= (f*k + e + 1) * A;
		}
		B = A;
		A = l;
	}
	return ans;
}

// count the number of 0 <= (a * x % m) < c for 0 <= x < n
long long mod_count(long long a, long long m, long long c, long long n) {
	assert(m > 0);
	if (n == 0) return 0;

	a %= m; if (a < 0) a += m;

	long long extraC = c / m; c %= m;
	if (c < 0) extraC--, c += m;
	assert(0 <= c && c < m);

	long long ans = extraC * n;

	long long extraN = n / m; n %= m;
	if (n < 0) extraN--, n += m;
	assert(0 <= n && n < m);

	if (extraN) {
		long long g = __gcd(a, m);
		ans += extraN * ((c + g - 1) / g * g);
		//ans += extraN * (lattice_cnt(m, a+m, (a+m) * (m-1)) - lattice_cnt(m, a+m, (a+m) * (m-1) - c));
	}

	if (n) {
		// we want solutions to 0 <= a(N-1-x) - my < c with 0 <= x <= N-1
		// a * (N-1) >= ax + my > a * (N-1) - c
		ans -= lattice_cnt(m, a+m, (a+m) * (n-1) - c);
	}

	return ans;
}

long long mod_count_range(long long a, long long m, long long clo, long long chi, long long nlo, long long nhi) {
	return mod_count(a, m, chi, nhi) - mod_count(a, m, chi, nlo) - mod_count(a, m, clo, nhi) + mod_count(a, m, clo, nlo);
}

using ll = long long;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int K, Q; cin >> K >> Q;
	vector<ll> D(K+1);
	for (int i = 1; i <= K; i++) cin >> D[i], D[i] += D[i-1];

	ll CYC = D[K];
	while (Q--) {
		ll N, X, M; cin >> N >> X >> M;
		ll ans = 0;
		ll totCnt = 0;
		for (int i = 0; i < K; i++) {
			ll xa = X + D[i];
			ll xb = X + D[i+1];
			xa %= M; xb %= M;
			if (xb <= xa) xb += M;
			assert(xb - xa <= M);
			// how many of k * CYC land so that 0 <= xa + k * CYC % MOD < xb + k * CYC % MOD < M
			// how many of k * CYC land so that -xa <= k * CYC % MOD < M - xb

			ll cnt = (N - (i+1) + K - 1) / K;
			totCnt += cnt;

			ans += cnt * (xb - xa);
			//ll cur = mod_count_range(CYC, M, -xa, M-xb, 0, cnt);
			//ans += cur;
		}
		assert(totCnt == N-1);
		//cout << ans << '\n';
		cout << N-1 - (ans + X%M) / M << '\n';
	}

	return 0;
}
