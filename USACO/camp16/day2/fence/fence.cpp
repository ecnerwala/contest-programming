#include<bits/stdc++.h>
using namespace std;

struct pt {
	ll x, y;

	pt(ll a, ll b) : x(a), y(b) {}
	pt() : x(0), y(0) {}

};

inline pt operator + (const pt &a, const pt &b) { return pt(a.x + b.x, a.y + b.y); }
inline pt operator - (const pt &a, const pt &b) { return pt(a.x - b.x, a.y - b.y); }
inline pt operator * (const pt &a, const ll &o) { return pt(a.x * o, a.y * o); }
inline pt operator / (const pt &a, const ll &o) { return pt(a.x / o, a.y / o); }
inline pt operator == (const pt &a, const pt &b) { return a.x == b.x && a.y == b.y; }
inline pt operator != (const pt &a, const pt &b) { return !(a == b); }

const int MAXN = 2e5;
int N;

int main() {

	return 0;
}
