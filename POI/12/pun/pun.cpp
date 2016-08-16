#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

inline ll _gcd(ll a, ll b) { return a ? _gcd(b % a, a) : b; }
inline ll gcd(ll a, ll b) { return abs(_gcd(a,b)); }

struct pt{
	ll x, y;
	pt(ll a, ll b) : x(a), y(b) {}
	pt() : x(0), y(0) {}
	inline pt& operator *= (const ll &a) {
		x *= a, y *= a;
		return *this;
	}
	inline pt& operator += (const pt &p) {
		x += p.x, y += p.y;
		return *this;
	}
	inline pt& operator -= (const pt &p) {
		x -= p.x, y -= p.y;
		return *this;
	}
	inline friend bool operator == (const pt &a, const pt &b) {
		return a.x == b.x && a.y == b.y;
	}
	inline friend bool operator != (const pt &a, const pt &b) {
		return a.x != b.x || a.y != b.y;
	}
};
const pt Z(0,0);
inline ll dot(const pt &a, const pt &b) { return a.x * b.x + a.y * b.y; }
inline ll cross(const pt &a, const pt &b) { return a.x * b.y - a.y * b.x; }
inline ll norm(const pt &a) { return dot(a, a); }

// returns a < b, Z is the greatest
bool cmp_pts(const pt &a, const pt &b) {
	if(a == b) return false;

	if(b == Z) return true;
	if(a == Z) return false;
	assert(a != Z);
	assert(b != Z);

	if(a.y <= 0 && 0 < b.y) {
		return true;
	}
	if(b.y <= 0 && 0 < a.y) {
		return false;
	}
	// either both have y <= 0 or both have y > 0

	ll c = cross(a, b);

	if(c == 0) {
		if(dot(a, b) >= 0) {
			return norm(a) < norm(b);
		} else {
			assert(a.y == 0 && b.y == 0);
			assert(a.x * b.x < 0); // on opposite sides of 0
			return a.x < b.x;
		}
	}

	// now, they're not collinear, so we can directly compute cross product
	return c > 0;
}

vector<ll> normalize(vector<pt> P) {
	int K = int(P.size());
	pt sum;
	for(int i = 0; i < K; i++) {
		sum += P[i];
	}
	for(int i = 0; i < K; i++) {
		P[i] *= K;
		P[i] -= sum;
	}

	sort(P.begin(), P.end(), cmp_pts);

	bool has_center = (P.back() == Z);
	if(has_center) P.pop_back(), K--;

	vector<ll> res;

	if(P.size()) {
		assert(P.size() >= 2);
		P.push_back(P[0]);
		for(int i = 0; i < K; i++) {
			// we don't need to distinguish because the max value is always uniquely a norm (no duplicates)
			assert(norm(P[i]) > 0);
			res.push_back(norm(P[i]));
			res.push_back(dot(P[i], P[i + 1]));
		}
		ll g = 0;
		for(int i = 0; i < 2 * K; i++) {
			g = gcd(g, res[i]);
		}
		assert(g != 0);
		assert(g > 0);
		for(int i = 0; i < 2 * K; i++) {
			res[i] /= g;
		}
	}

	if(has_center) res.push_back(0);
	return res;
}

template <typename T> vector<int> build_kmp(const vector<T> &p) {
	int s = int(p.size());
	vector<int> f(s+1);
	f[0] = -1;
	f[1] = 0;
	for(int i = 1, cnd = 0; i < s;) {
		if(cnd < 0 || p[i] == p[cnd]) {
			i++;
			cnd++;
			f[i] = cnd;
		} else {
			cnd = f[cnd];
		}
	}
	return f;
}

// match p and s cyclically
template <typename T> bool match(const vector<T> &p, const vector<int> &f, const vector<T> &s) { 
	assert(f.size() == p.size() + 1);
	if(p.size() != s.size()) return false;
	int n = int(p.size());
	if(!n) return true;
	for(int i = 0, j = 0; i < 2 * n;) {
		if(j < 0 || s[i % n] == p[j]) {
			i++, j++;
			if(j == int(p.size())) return true;
		} else {
			j = f[j];
		}
	}
	return false;
}

vector<pt> read_poly() {
	int K;
	scanf("%d", &K);
	vector<pt> P(K);
	for(int i = 0; i < K; i++) {
		int x, y;
		scanf("%d %d", &x, &y);
		P[i] = pt(x, y);
	}
	return P;
}

vector<ll> pattern;
bool has_center;
vector<int> f;

bool query() {
	vector<pt> P = read_poly();
	for(int z = 0; z < 2; z++) {
		vector<ll> pointset = normalize(P);
		bool hc = (pointset.size() % 2 == 1);
		if(hc) {
			pointset.pop_back();
		}
		if(hc == has_center && match(pattern, f, pointset)) return true;
		for(int i = 0; i < int(P.size()); i++) {
			P[i].y *= -1;
		}
	}
	return false;
}

int main() {
	pattern = normalize(read_poly());
	has_center = (pattern.size() % 2 == 1);
	if(has_center) {
		pattern.pop_back();
	}
	f = build_kmp(pattern);
	int N;
	scanf("%d", &N);
	for(int i = 0; i < N; i++) {
		printf(query() ? "TAK\n" : "NIE\n");
	}
	return 0;
}
