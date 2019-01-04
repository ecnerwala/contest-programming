#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

inline ll _gcd(ll a, ll b) { return a ? _gcd(b % a, a) : b; }
inline ll gcd(ll a, ll b) { return abs(_gcd(a,b)); }

typedef complex<ll> pt;
const pt Z(0,0);
inline ll dot(const pt &a, const pt &b) { return real(conj(a) * b); }
inline ll cross(const pt &a, const pt &b) { return imag(conj(a) * b); }

// returns a < b, Z is the greatest
bool cmp_pts(const pt &a, const pt &b) {
	if(a == b) return false;

	if(b == Z) return true;
	if(a == Z) return false;
	assert(a != Z);
	assert(b != Z);

	if(a.imag() <= 0 && 0 < b.imag()) {
		return true;
	}
	if(b.imag() <= 0 && 0 < a.imag()) {
		return false;
	}
	// either both have imag <= 0 or both have imag > 0

	ll c = cross(a, b);

	if(c == 0) {
		if(dot(a, b) >= 0) {
			return norm(a) < norm(b);
		} else {
			assert(a.imag() == 0 && b.imag() == 0);
			assert(a.real() * b.real() < 0); // on opposite sides of 0
			return a.real() < b.real();
		}
	}

	// now, they're not collinear, so we can directly compute cross product
	return c > 0;
}

vector<ll> get_poly() {
	int K;
	scanf("%d", &K);
	vector<pt> P(K);
	for(int i = 0; i < K; i++) {
		int x, y;
		scanf("%d %d", &x, &y);
		P[i] = pt(x, y);
	}
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
	for(int i = 1, cnd = 0; i <= s;) {
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
	if(p.size() != s.size()) return false;
	int n = int(p.size());
	for(int i = 0, j = 0; i < 2 * n;) {
		if(j < 0 || s[i % n] == p[j]) {
			i++, j++;
			if(j == int(p.size())) return true;
		} else {
			j = f[j];
		}
	}
	for(int i = 2 * n - 1, j = 0; i >= 0;) {
		if(j < 0 || s[i % n] == p[j]) {
			i--, j++;
			if(j == int(p.size())) return true;
		} else {
			j = f[j];
		}
	}
	return false;
}

int main() {
	vector<ll> pattern = get_poly();
	bool has_center = (pattern.size() % 2 == 1);
	if(has_center) {
		pattern.pop_back();
	}
	vector<int> f = build_kmp(pattern);
	int N;
	scanf("%d", &N);
	for(int i = 0; i < N; i++) {
		vector<ll> pointset = get_poly();
		bool hc = (pointset.size() % 2 == 1);
		if(hc) {
			pointset.pop_back();
		}
		printf((hc == has_center && match(pattern, f, pointset)) ? "TAK\n" : "NIE\n");
	}
	return 0;
}
