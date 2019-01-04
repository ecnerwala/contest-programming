#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "khazaddum.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll INF = 1e9;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const ll R = GetRangeLength() * 2;
const ll G = GramsToExcavate() * 8; // we double width and height for 45, and then again for triangles

ll split (ll a, ll i, ll n) {
	return (a / n) * i + min(a % n, i);
}

const ll ST = split(R/2, ID, NODES) * 2;
const ll EN = split(R/2, ID+1, NODES) * 2;

const ll MAXSPLIT = ll(1.1e6 * 2);
ll _H[MAXSPLIT+1];
ll * const H = _H - ST;
ll _B[MAXSPLIT+1];
ll * const B = _B - ST;

static_assert(sizeof(ll) == sizeof(double), "Can't convert between double and ll");
void PutDouble(int target, double v) {
	PutLL(target, reinterpret_cast<ll&>(v));
}
double GetDouble(int source) {
	ll v = GetLL(source);
	return reinterpret_cast<double&>(v);
}

struct poly2 {
	double c0, c1, c2;
	poly2& operator + (const poly2& a) {
		c0 += a.c0;
		c1 += a.c1;
		c2 += a.c2;
		return *this;
	}
	friend poly2 operator + (const poly2& a, const poly2& b) {
		return poly2{a.c0+b.c0, a.c1+b.c1, a.c2+b.c2};
	}
	double operator () (double v) {
		return (v * c2 + c1) * v + c0;
	}
};

poly2 value(ll k) {
	poly2 res{0,0,0};
	for (ll i = ST; i < EN; i++) {
		ll a = H[i] + k;
		ll b = H[i+1] + k;
		if (a < 0 && b < 0) {
			// no-op
		} else if (a >= 0 && b >= 0) {
			res.c0 += double(a + b);
			res.c1 += 2;
		} else {
			if (a < b) swap(a,b);
			assert(a >= 0 && 0 > b);
			res.c0 += double(a)*double(a)/double(a-b);
			res.c1 += double(2)*double(a)/double(a-b);
			res.c2 += double(1)/double(a-b);
		}
	}
	return res;
}

void PutPoly2(int target, const poly2& p) {
	PutDouble(target, p.c0);
	PutDouble(target, p.c1);
	PutDouble(target, p.c2);
}
poly2 GetPoly2(int source) {
	poly2 res;
	res.c0 = GetDouble(source);
	res.c1 = GetDouble(source);
	res.c2 = GetDouble(source);
	return res;
}

int main() {
	{
		for (ll i = ST; i <= EN; i+=2) {
			H[i] = GetHeight(i/2) * 2;
		}
		for (ll i = ST+1; i < EN; i+=2 ) {
			H[i] = H[i-1]/2 + H[i+1]/2;
		}
		for (ll i = ST; i <= EN; i++) {
			B[i] = H[i];
		}
	}

	{
		ll minL = 0;
		for (ll i = ST; i <= EN; i+=2) {
			minL = max(minL-2, H[i]);
		}
		ll minR = 0;
		for (ll i = EN; i >= ST; i-=2) {
			minR = max(minR-2, H[i]);
		}

		if (ID-1 >= 0) {
			Receive(ID-1);
			ll a = GetLL(ID-1);
			B[ST] = max(B[ST], a);
			minL = max(minL, a - (EN - ST));
		}
		if (ID + 1 < NODES) {
			PutLL(ID+1, minL);
			Send(ID+1);
		}

		if (ID+1 < NODES) {
			Receive(ID+1);
			ll a = GetLL(ID+1);
			B[EN] = max(B[EN], a);
			minR = max(minR, a - (EN - ST));
		}
		if (ID - 1 >= 0) {
			PutLL(ID-1, minR);
			Send(ID-1);
		}
	}
	{
		for (ll i = ST; i < EN; i++) {
			B[i+1] = max(B[i+1], B[i]-1);
		}
		for (ll i = EN; i > ST; i--) {
			B[i-1] = max(B[i-1], B[i]-1);
		}
	}

	for (ll i = ST; i <= EN; i++) {
		assert(B[i] >= H[i]);
	}
	for (ll i = ST; i < EN; i++) {
		assert(abs(B[i] - B[i+1]) <= 1);
	}
	cerr << ST << '=' << B[ST] << ' ' << EN << '=' << B[EN] << '\n';

	// We push H's up by k, we want the amount exposed above 0 to be at least G
	for (ll i = ST; i <= EN; i++) {
		H[i] -= B[i];
		B[i] = 0;
	}
	const int ARY = 10;
	ll mi = 0;
	ll ma = int(4e8) + 20;
	while (ma - mi > 1) {
		for (int i = 0; i <= ARY; i++) {
			ll h = split(ma-mi, i, ARY) + mi;
			poly2 v = value(h);
			PutPoly2(0, v);
		}
		Send(0);
		if (ID == 0) {
			for (int n = 0; n < NODES; n++) {
				Receive(n);
			}
			ll nmi = mi;
			ll nma = ma;
			for (int i = 0; i <= ARY; i++) {
				ll h = split(ma-mi, i, ARY) + mi;
				poly2 v{0,0,0};
				for (int n = 0; n < NODES; n++) {
					v = v + GetPoly2(n);
				}
				assert(v.c1 >= 0);
				assert(v.c2 >= 0);
				v.c0 -= double(G);
				//cerr << h << ':' << v(0) << ' ' << v(1) << '\n';
				if (v(0) > 0) {
					nma = min(nma, h);
				} else if (v(1) < 0) {
					nmi = max(nmi, h);
				} else {
					// we have our answer
					// analytically solve v(i) == G
					// -c1 +- sqrt(c1^2 - c0 c2)
					// sqrt(c1^2 - c0 c2) - c1
					double hfrac;
					if (v.c2 < 1e-10) {
						if (v.c1 < 1e-10) {
							assert(abs(v.c0) < 1e-10);
							hfrac = 0.;
						} else {
							hfrac = -v.c0 / v.c1;
						}
					} else {
						v.c1 /= 2*v.c2;
						v.c0 /= v.c2;
						hfrac = sqrt(v.c1 * v.c1 - v.c0) - v.c1;
					}
					assert(0 <= hfrac && hfrac-1 <= 1e-9);
					cout << fixed << setprecision(8) << (double(h)+hfrac)/2 << '\n';
					for (int n = 1; n < NODES; n++) {
						PutLL(n, -1);
						PutLL(n, -1);
						Send(n);
					}
					for (int n = 0; n < NODES; n++) {
						for (int j = i+1; j <= ARY; j++) {
							// read out the rest of the messages
							GetPoly2(n);
						}
					}
					return 0;
				}
			}
			for (int n = 0; n < NODES; n++) {
				PutLL(n, nmi);
				PutLL(n, nma);
				Send(n);
			}
			cerr << nmi << ' ' << nma << '\n';
		}
		Receive(0);
		mi = GetLL(0);
		ma = GetLL(0);
	}

	return 0;
}
