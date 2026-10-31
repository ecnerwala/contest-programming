#include<bits/stdc++.h>
using namespace std;

using ll = long long;

struct frac {
	ll p, q;
	frac() : p(0), q(1) {}
	frac(ll a) : p(a), q(1) {}
	frac(ll p_, ll q_) : p(p_), q(q_) {
		assert(q);
		if (q < 0) p = -p, q = -q;
		ll g = __gcd(abs(p), q);
		p /= g, q /= g;
	}

	friend frac operator + (const frac& a, const frac& b) {
		ll g = __gcd(a.q, b.q);
		return frac((b.q / g) * a.p + (a.q / g) * b.p, a.q / g * b.q);
	}
	friend frac operator - (const frac& a, const frac& b) {
		ll g = __gcd(a.q, b.q);
		return frac((b.q / g) * a.p - (a.q / g) * b.p, a.q / g * b.q);
	}
	friend frac operator * (const frac& a, const frac& b) {
		return frac(a.p * b.p, a.q * b.q);
	}
	friend frac operator / (const frac& a, const frac& b) {
		return frac(a.p * b.q, a.q * b.p);
	}
	friend bool operator < (const frac& a, const frac& b) { return a.p * b.q < b.p * a.q; }
	friend bool operator > (const frac& a, const frac& b) { return a.p * b.q > b.p * a.q; }
	friend bool operator <= (const frac& a, const frac& b) { return a.p * b.q <= b.p * a.q; }
	friend bool operator >= (const frac& a, const frac& b) { return a.p * b.q >= b.p * a.q; }
	friend bool operator == (const frac& a, const frac& b) { return a.p * b.q == b.p * a.q; }
	friend bool operator != (const frac& a, const frac& b) { return a.p * b.q != b.p * a.q; }

	friend std::ostream& operator << (std::ostream& o, const frac& f) { return o << f.p << '/' << f.q; }
};

struct pt {
	frac x, y;
	friend pt operator + (const pt& a, const pt& b) { return pt{a.x + b.x, a.y + b.y}; }
	friend pt operator - (const pt& a, const pt& b) { return pt{a.x - b.x, a.y - b.y}; }
	friend pt operator * (const pt& a, const frac& f) { return pt{a.x * f, a.y * f}; }
	friend pt operator / (const pt& a, const frac& f) { return pt{a.x / f, a.y / f}; }

	friend frac dot(const pt& a, const pt& b) { return a.x * b.x + a.y * b.y; }
	friend frac cross(const pt& a, const pt& b) { return a.x * b.y - a.y * b.x; }
	friend frac area(const pt& a, const pt& b, const pt& c) { return cross(b - a, c - a); }
};

const int MAXN = 300;
const int MAXK = 15;
int N, K;
pt P[MAXN * 3];

bool symmetric(pt a, pt b, pt x, pt y) {
	return dot(x-a, b-a) == dot(y-a, b-a) && area(x, a, b) == area(y, b, a);
}

void go() {
	cin >> N >> K;
	for (int i = 0; i < N; i++) {
		ll x, y; cin >> x >> y;
		P[i].x = frac(x);
		P[i].y = frac(y);
	}
	for (int i = 0; i < N; i++) {
		P[i+N+N] = P[i+N] = P[i];
	}

	if (K == 2) {
		for (int i = 0; i < N; i++) {
			pt a = P[i];
			pt b = (P[i + N / 2] + P[i + N - N/2]) / 2;
			bool good = true;
			for (int j = i+1, k = N + i-1; j < k; j ++, k--) {
				if (!symmetric(a, b, P[j], P[k])) {
					good = false;
					break;
				}
			}
			if (good) {
				cout << "POSSIBLE\n";
				cout << a.x << ' ' << a.y << ' ' << b.x << ' ' << b.y << '\n';
				return;
			}
		}

		if (N % 2 == 0) {
			for (int i = 0; i < N; i++) {
				pt a = (P[i] + P[i+1]) / 2;
				pt b = (P[i + N/2] + P[i+1 + N/2]) / 2;
 				bool good = true;
				for (int j = i+1, k = N + i; j < k; j ++, k--) {
					if (!symmetric(a, b, P[j], P[k])) {
						good = false;
						break;
					}
				}
				if (good) {
					cout << "POSSIBLE\n";
					cout << a.x << ' ' << a.y << ' ' << b.x << ' ' << b.y << '\n';
					return;
				}
			}
		}
	}
	cout << "IMPOSSIBLE\n";
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {
		cout << "Case #" << case_num << ": ";

		go();
	}

	return 0;
}
