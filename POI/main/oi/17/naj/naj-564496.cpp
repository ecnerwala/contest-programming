#include<iostream>
#include<iomanip>
#include<algorithm>

using namespace std;

typedef long long ll;

const ll MAXN = 700;
const ll MAXV = 1e6 + 50;
const ll MAXP = MAXV + MAXN;

ll P;
ll primes[MAXP];
ll cnt[MAXP];
ll primeFac[MAXV + 1];

ll N;
ll A[MAXN];

ll gcd(ll a, ll b) { return a ? gcd(b % a, a) : b; }

ll sum(ll a, ll b, ll m) {
	ll res = a + b;
	if(res >= m) res -= m;
	if(res < 0) res += m;
	return res;
}
ll prod(ll a, ll b, ll m) {
	if(b < 0) b *= -1, a *= -1;
	ll res = 0;
	while(b) {
		if(b % 2) {
			res = sum(res, a, m);
		}
		b /= 2;
		a = sum(a, a, m);
	}
	return res;
}
ll exp(ll a, ll b, ll m) {
	//assert(b >= 0);
	ll res = 1;
	while(b) {
		if(b % 2) {
			res = prod(a, res, m);
		}
		b /= 2;
		if(a < 0) a = -a;
		a = prod(a, a, m);
	}
	return res;
}

ll sqrt(ll a) {
	if(a == 0) return 0;
	ll mi = 0;
	ll ma = 1;
	while(ma * ma <= a) {
		mi = ma;
		ma *= 2;
	}
	while(ma - mi > 1) {
		ll md = (mi + ma) / 2;
		if(md * md <= a) {
			mi = md;
		} else {
			ma = md;
		}
	}
	return mi;
}

ll jacobi(ll a, ll n) {
	a %= n;

	if(a == 0) {
		if(n == 1) return 1;
		else return 0;
	}

	ll res = 1;
	if(a < 0) { 
		a *= -1;
		if(n % 4 == 3) {
			res *= -1;
		}
	}
	bool nonres2 = (n % 8 == 3) || (n % 8 == 5);
	while(a % 2 == 0) {
		a /= 2;
		if(nonres2) res *= -1;
	}
	if(n % 4 == 3 && a % 4 == 3) res *= -1;
	res *= jacobi(n, a);
	return res;
}

//returns is "prime"
bool MillerRabin(ll n, ll b) {
	ll d = n - 1;
	ll s = 0;
	while(d % 2 == 0) d /= 2, s++;
	ll v = exp(b, d, n);
	if(v == 1 || v == n - 1) {
		return true;
	}
	while(s > 1) {
		v = prod(v, v, n);
		if(v == 1) return false;
		if(v == n - 1) return true;
		s--;
	}
	return false;
}
bool StrongLucas(ll n) {
	ll D = 5;
	while(jacobi(D, n) != -1) {
		if(D > 0) D += 2;
		else D -= 2;
		D *= -1;
	}
	ll P = 1;
	ll Q = (1 - D) / 4;
	//cerr << D << ' ' << Q << ' ' << n << '\n';
	if(gcd(abs(Q), n) != 1) return false;
	ll d = n + 1;
	ll s = 0;
	while(d % 2 == 0) {
		d /= 2;
		s++;
	}

	ll pow = 0;
	while((1ll << pow) <= d) {
		pow++;
	}
	//cerr << D << ' ' << P << ' ' << Q << ' ' << d << ' ' << s << ' ' << pow << '\n';

	ll u = 0, v = 2, q = 1;
	for(; pow >= 0; pow--) {
		u = prod(u, v, n);
		v = sum(prod(v, v, n), -sum(q, q, n), n);
		q = prod(q, q, n);
		//cerr << (d >> pow) / 2 * 2 << ' ' << u << ' ' << v << ' ' << q << '\n';
		if(d & (1ll << pow)) {
			ll nu = sum(prod(P, u, n), v, n);
			if(nu % 2 == 1) nu += n;
			nu /= 2;
			ll nv = sum(prod(D, u, n), prod(P, v, n), n);
			if(nv % 2 == 1) nv += n;
			nv /= 2;
			ll nq = prod(q, Q, n);
			u = nu, v = nv, q = nq;
			//cerr << (d >> pow) << ' ' << u << ' ' << v << ' ' << q << '\n';
		}
		//assert(q == exp(Q, d >> pow, n));
	}
	//cerr << u << ' ' << v << ' ' << q << '\n';
	if(u == 0) return true;
	for(; s; s--) {
		if(v == 0) return true;
		u = prod(u, v, n);
		v = sum(prod(v, v, n), -sum(q, q, n), n);
		q = prod(q, q, n);
		//cerr << u << ' ' << v << ' ' << q << '\n';
	}
	return false;
}
bool primeTest(ll n) {
	if(n % 2 == 0) return n == 2;

	if(!MillerRabin(n, 2)) {
		//cerr << "failed Miller-Rabin\n";
		return false;
	}
	if(!StrongLucas(n)) {
		//cerr << "failed Strong Lucas\n";
		return false;
	}

	return true;
}

void insertPrime(ll p) {
	primes[P] = p;
	for(ll i = 0; i < N; i++) {
		while(A[i] % p == 0) {
			A[i] /= p;
			cnt[P] ++;
		}
	}
	P++;
}

void sieve() {
	for(ll i = 2; i <= MAXV; i++) primeFac[i] = i;
	for(ll i = 2; i * i <= MAXV; i++) {
		//cerr << i << '\n';
		if(primeFac[i] == i) {
			for(ll j = i * i; j <= MAXV; j+=i) {
				primeFac[j] = i;
			}
		}
	}
}

ll K;

const ll BASE = 1e18;
const int LEN = 100;

ll D[LEN];

void doubleD() {
	for(int i = LEN - 1; i >= 0; i--) {
		D[i] *= 2;
		if(D[i] >= BASE) {
			D[i + 1] ++;
			D[i] -= BASE;
		}
	}
}

void go() {
	sieve();

	P = 0;

	//small primes
	for(ll i = 2; i <= MAXV; i++) {
		if(primeFac[i] == i) {
			insertPrime(i);
		}
	}
	//all that are left are primes and semiprimes

	//small-ish primes
	for(int i = 0; i < N; i++) {
		if(A[i] == 1) continue;
		if(A[i] <= MAXV * MAXV) {
			insertPrime(A[i]);
		}
	}

	//squares
	for(int i = 0; i < N; i++) {
		if(A[i] == 1) continue;
		ll p = sqrt(A[i]);
		if(p * p == A[i]) {
			insertPrime(p);
		}
	}

	//gcd's
	for(int i = 0; i < N; i++) {
		if(A[i] == 1) continue;
		for(int j = i + 1; j < N; j++) {
			if(A[j] == 1) continue;
			if(A[i] == A[j]) continue;
			ll g = gcd(A[i], A[j]);
			if(g > 1) {
				insertPrime(g);
				if(A[i] > 1) insertPrime(A[i]);
				if(A[j] > 1) insertPrime(A[j]);
				break;
			}
		}
	}

	//insert remaining as negatives
	for(int i = 0; i < N; i++) {
		if(A[i] == 1) continue;
		insertPrime(A[i]);
		primes[P - 1] *= -1;
	}

	//find maximum count
	K = 0;
	for(int i = 0; i < P; i++) {
		if(cnt[i] > K) K = cnt[i];
	}

	D[0] = 1;
	//find number of primes
	for(int i = 0; i < P; i++) {
		if(cnt[i] == K) {
			ll p = primes[i];
			doubleD();
			if(p < 0 && !primeTest(-p)) {
				doubleD();
			}
		}
	}

	D[0] --;
	for(int i = 0; D[i] < 0; i++) {
		D[i] += BASE;
		D[i + 1] --;
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for(int i = 0; i < N; i++) cin >> A[i];

	//cerr << MAXV << '\n';
	go();

	cout << K << '\n';
	bool start = false;
	for(int i = LEN - 1; i >= 0; i--) {
		if(start) {
			cout << D[i];
		} else if(D[i]) {
			cout << D[i];
			cout << setw(18) << setfill('0');
			start = true;
		}
	}
	cout << setw(0);
	cout << '\n';
	return 0;
}
