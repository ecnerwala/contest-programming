#include<bits/stdc++.h>
using namespace std;
typedef unsigned long long ull;
typedef long long ll;
typedef long double ld;

#define trav(a, x) for (auto& a : x)
#define all(x) (x).begin(), (x).end()
ull mod_mul(ull a, ull b, ull M) {
	ll ret = a * b - M * ull(ld(a) * ld(b) / ld(M));
	return ret + M * (ret < 0) - M * (ret >= (ll)M);
	//return a * b % M;
}
ull mod_pow(ull b, ull e, ull mod) {
	ull ans = 1;
	for (; e; b = mod_mul(b, b, mod), e /= 2)
		if (e & 1) ans = mod_mul(ans, b, mod);
	return ans;
}
bool isPrime(ull n) {
	if (n < 2 || n % 6 % 4 != 1) return n - 2 < 2;
	ull A[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022},
	    s = __builtin_ctzll(n-1), d = n >> s;
	trav(a, A) {   // ^ count trailing zeroes
		ull p = mod_pow(a, d, n), i = s;
		while (p != 1 && p != n - 1 && a % n && i--)
			p = mod_mul(p, p, n);
		if (p != n-1 && i != s) return 0;
	}
	return 1;
}
ull pollard(ull n) {
	auto f = [n](ull x) { return (mod_mul(x, x, n) + 1) % n; };
	if (!(n & 1)) return 2;
	for (ull i = 2;; i++) {
		ull x = i, y = f(x), p;
		while ((p = __gcd(n + y - x, n)) == 1)
			x = f(x), y = f(f(y));
		if (p != n) return p;
	}
}
vector<ull> factor(ull n) {
	if (n == 1) return {};
	if (isPrime(n)) return {n};
	ull x = pollard(n);
	auto l = factor(x), r = factor(n / x);
	l.insert(l.end(), all(r));
	return l;
}


const int MAXN = 1.1e5;
int N;
int A[MAXN];

namespace small {
map<pair<int, int>, pair<int, pair<int, int>>> dp[MAXN];
void solveSmall() {
	dp[0].emplace(pair<int, int>(A[0], 0), pair<int, pair<int, int>>(1, pair<int, int>(0, 0)));
	for (int i = 1; i < N; i++) {
		for (auto it : dp[i-1]) {
			dp[i].emplace(pair<int, int>(__gcd(it.first.first, A[i]), it.first.second), make_pair(1, it.first));
			dp[i].emplace(pair<int, int>(it.first.first, __gcd(A[i], it.first.second)), make_pair(2, it.first));
		}
	}
	if (dp[N-1].count(pair<int, int>(1, 1))) {
		pair<int, int> state(1, 1);
		vector<int> ans(N);
		for (int i = N-1; i >= 0; i--) {
			auto v = dp[i][state];
			ans[i] = v.first;
			state = v.second;
		}
		cout << "YES" << '\n';
		for (int i = 0; i < N; i++) {
			cout << ans[i] << " \n"[i+1==N];
		}
	} else {
		cout << "NO" << '\n';
	}
	exit(0);
}
}
using small::solveSmall;

vector<int> primes[MAXN];
unordered_map<uint32_t, pair<int, uint32_t>> dp[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> A[i];
	}
	{
		int g = A[0];
		for (int i = 0; i < N; i++) {
			g = __gcd(g, A[i]);
		}
		if (g != 1) {
			cout << "NO\n";
			exit(0);
		}
	}

	int Npref = min(N, 100);
	for (int i = 0; i < Npref; i++) {
		vector<ull> _primes = factor(A[i]);
		set<int> primeSet(all(_primes));
		primes[i] = vector<int>(all(primeSet));
		A[i] = 1;
		for (int p : primes[i]) A[i] *= p;
	}

	if (N <= 30) {
		solveSmall();
		assert(false);
	}


	map<int, int> primeCnt;
	for (int i = 0; i < Npref; i++) {
		for (int p : primes[i]) {
			primeCnt[p] ++;
		}
	}

	int K = int(primes[0].size());
	vector<int> badPrimes;
	for (auto it : primeCnt) {
		if (it.second >= Npref - K - 1) {
			badPrimes.push_back(it.first);
		}
	}
	int L = int(badPrimes.size());

	dp[0].emplace((1 << (L+K))-1, pair<int, uint32_t>(1, 0));
	for (int i = 1; i < N; i++) {
		uint32_t v1 = ((1 << L) - 1) << K;
		for (int z = 0; z < K; z++) {
			if (A[i] % primes[0][z] == 0) {
				v1 |= (1 << z);
			}
		}
		uint32_t v2 = (1 << K)-1;
		for (int z = 0; z < L; z++) {
			if (A[i] % badPrimes[z] == 0) {
				v2 |= (1 << (z+K));
			}
		}
		for (auto it : dp[i-1]) {
			uint32_t state = it.first;
			if ((state & v1) != state) {
				dp[i].emplace(state & v1, pair<int, uint32_t>(1, state));
				if ((state & v2) != state) {
					dp[i].emplace(state & v2, pair<int, uint32_t>(2, state));
				}
			} else {
				dp[i].emplace(state & v2, pair<int, uint32_t>(2, state));
			}
		}
	}
	if (dp[N-1].count(0)) {
		cout << "YES" << '\n';
		vector<int> ans(N);
		uint32_t state = 0;
		for (int i = N-1; i >= 0; i--) {
			auto it = dp[i][state];
			ans[i] = it.first;
			state = it.second;
		}
		for (int i = 0; i < N; i++) {
			cout << ans[i] << " \n"[i+1==N];
		}
		exit(0);
	} else {
		cout << "NO" << '\n';
		exit(0);
	}

	return 0;
}
