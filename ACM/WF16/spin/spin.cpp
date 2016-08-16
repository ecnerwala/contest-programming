#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

inline ll sign(ll a) { return (a > 0) - (a < 0); }

struct pt {
	ll x, y;

	pt() : x(0), y(0) {}
	pt(ll a, ll b) : x(a), y(b) {}

	pt operator + (const pt &o) const {
		return pt(x + o.x, y + o.y);
	}

	pt operator - (const pt &o) const {
		return pt(x - o.x, y - o.y);
	}

	pt operator * (const ll &o) const {
		return pt(x * o, y * o);
	}

	pt operator / (const ll &o) const {
		return pt(x / o, y / o);
	}

};

ll cross(const pt &a, const pt &b) {
	return a.x * b.y - a.y * b.x;
}

ll area(const pt &a, const pt &b, const pt &c) {
	return cross(b - a, c - a);
}

bool operator == (const pt &a, const pt &b) { return a.x == b.x && a.y == b.y; }
bool operator != (const pt &a, const pt &b) { return !(a == b); }

bool operator < (const pt &a, const pt &b) {
	if(a == b) return false;
	if(a == pt(0, 0)) return true;
	if(cross(a,b) == 0) {
		sign(a.x) == sign(b.x) && sign(a.y) == sign(b.y)
		return abs(a.x) + abs(a.y) < abs(b.x) + abs(b.y);
	}
	if(sign(a.y) != sign(b.y)) {
	}
}

bool operator > (const pt &a, const pt &b) { return b < a; }
bool operator <= (const pt &a, const pt &b) { return !(a > b); }
bool operator >= (const pt &a, const pt &b) { return !(a < b); }

const int MAXN = 1e6;
int main() {
}
