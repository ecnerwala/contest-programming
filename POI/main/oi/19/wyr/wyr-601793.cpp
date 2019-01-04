#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll INF = 1e17;

typedef pair<ll, ll> pii;
#define A first
#define B second

ll gcd(ll a, ll b) { return a ? gcd(b % a, a): b; }
ll inv(ll a, ll m) { return (a == 1 || a == m - 1) ? a : m - inv(m % a, a) * m / a; }

const int MAXN = 2e5;
int N;

ll A, B;
ll H[MAXN];

pii O[MAXN]; // operations

inline ll mod(ll a, ll m) { a %= m; if(a < 0) a += m; return a; }

inline ll norm(pii a) { return abs(a.A) + abs(a.B); }

inline pii best(pii a, pii b) { return (norm(a) <= norm(b)) ? a : b; }

inline pii op(pii a) { return pii(a.A + B, a.B - A); }

ll go() {
	H[N] = 0;
	for(int i = N; i > 0; i--) {
		H[i] -= H[i - 1];
	}
	N++;

	ll g = gcd(A, B);
	A /= g, B /= g;
	for(int i = 0; i < N; i++) {
		if(H[i] % g != 0) return -1;
		H[i] /= g;
	}

	if(A == B) {
		assert(A == 1 && B == 1);
		ll res = 0;
		for(int i = 0; i < N; i++) {
			res += abs(H[i]);
		}
		assert(res % 2 == 0);
		return res / 2;
	}

	if(A > B) swap(A, B);

	ll x = inv(A, B);
	ll y = inv(B, A);

	for(int i = 0; i < N; i++) {

		ll a = mod(x * (H[i] % B), B);
		O[i] = pii(INF, INF);
		if((H[i] - a * A) % B != 0) {
			cerr << H[i] << ' ' << A << ' ' << B << ' ' << x << ' ' << a << '\n';
		}
		assert((H[i] - a * A) % B == 0);
		O[i] = best(O[i], pii(a, (H[i] - a * A) / B));
		if(a) {
			a -= B;
			assert((H[i] - a * A) % B == 0);
			O[i] = best(O[i], pii(a, (H[i] - a * A) / B));
		}

		ll b = mod(y * (H[i] % A), A);
		assert((H[i] - b * B) % A == 0);
		O[i] = best(O[i], pii((H[i] - b * B) / A, b));
		if(b) {
			b -= A;
			assert((H[i] - b * B) % A == 0);
			O[i] = best(O[i], pii((H[i] - b * B) / A, b));
		}

		assert(O[i].A * A + O[i].B * B == H[i]);
	}

	ll sumA = 0;
	ll sumB = 0;
	for(int i = 0; i < N; i++) {
		sumA += O[i].A;
		sumB += O[i].B;
	}
	assert(sumA % B == 0);
	assert(sumB % A == 0);

	assert(sumA * A + sumB * B == 0);

	if(sumB < 0) {
		swap(sumA, sumB);
		swap(A, B);
		for(int i = 0; i < N; i++) {
			swap(O[i].A, O[i].B);
		}
		swap(x, y);
	}

	assert(sumA % B == 0);
	assert(sumB % A == 0);

	assert(sumA * A + sumB * B == 0);

	sumA /= B;
	sumB /= A;

	priority_queue<pair<ll, int> > pq;
	for(int i = 0; i < N; i++) {
		pq.push(make_pair(norm(O[i]) - norm(op(O[i])), i));
	}

	while(sumA < 0) {
		assert(!pq.empty());
		int i = pq.top().second;
		pq.pop();
		ll cnt = 1;
		if(O[i].A >= 0 && O[i].B <= 0) {
			cnt = -sumA;
		} else if(O[i].A + B < 0 && O[i].B - A > 0) {
			cnt = min(-O[i].A / B, O[i].B / A);
		}
		O[i].A += B * cnt;
		O[i].B -= A * cnt;
		sumA += cnt;
		pq.push(make_pair(norm(O[i]) - norm(op(O[i])), i));
	}

	ll res = 0;
	for(int i = 0; i < N; i++) {
		res += norm(O[i]);
	}
	//assert(res % 2 == 0);
	return res / 2;
}

int main() {
	scanf("%d", &N);
	scanf("%lld %lld", &A, &B);
	for(int i = 0; i < N; i++) {
		scanf("%lld", &H[i]);
	}
	printf("%lld\n", go());
	return 0;
}
