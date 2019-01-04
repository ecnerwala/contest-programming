#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "modnum.h"
#endif
//#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

int minv(int a, int m) {
    assert(a);
    a %= m;
    return a == 1 ? 1 : int(m - ll(minv(m, a)) * ll(m) / a);
}

template <int MOD> struct modnum {
    static_assert(MOD > 0, "MOD must be positive");

    int v;

    modnum() : v(0) {}
    modnum(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
    explicit operator int() const { return v; }

    modnum inv() const {
        modnum res;
        res.v = minv(v, MOD);
        return res;
    }
    modnum neg() const {
        modnum res;
        res.v = v ? MOD-v : 0;
        return res;
    }
    modnum operator- () const {
        return neg();
    }
    modnum operator+ () const {
        return modnum(*this);
    }

    modnum& operator += (const modnum& o) {
        v += o.v;
        if (v >= MOD) v -= MOD;
        return *this;
    }
    modnum& operator -= (const modnum& o) {
        v -= o.v;
        if (v < 0) v += MOD;
        return *this;
    }
    modnum& operator *= (const modnum& o) {
        v = int(ll(v) * ll(o.v) % MOD);
        return *this;
    }
    modnum& operator /= (const modnum& o) {
        return *this *= o.inv();
    }

    friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
    friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
    friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
    friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }
    friend modnum operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
    friend modnum operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

    friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
};

template <typename T, typename U> struct pairnum {
    T t;
    U u;

    pairnum() : t(0), u(0) {}
    pairnum(ll v) : t(v), u(v) {}
    pairnum(const T& t_, const U& u_) : t(t_), u(u_) {}

    pairnum inv() const {
        return pairnum(t.inv(), u.inv());
    }
    pairnum neg() const {
        return pairnum(t.neg(), u.neg());
    }
    pairnum operator- () const {
        return pairnum(-t, -u);
    }
    pairnum operator+ () const {
        return pairnum(+t, +u);
    }

    pairnum& operator += (const pairnum& o) {
        t += o.t;
        u += o.u;
        return *this;
    }
    pairnum& operator -= (const pairnum& o) {
        t -= o.t;
        u -= o.u;
        return *this;
    }
    pairnum& operator *= (const pairnum& o) {
        t *= o.t;
        u *= o.u;
        return *this;
    }
    pairnum& operator /= (const pairnum& o) {
        t /= o.t;
        u /= o.u;
        return *this;
    }

    friend pairnum operator + (const pairnum& a, const pairnum& b) { return pairnum(a) += b; }
    friend pairnum operator - (const pairnum& a, const pairnum& b) { return pairnum(a) -= b; }
    friend pairnum operator * (const pairnum& a, const pairnum& b) { return pairnum(a) *= b; }
    friend pairnum operator / (const pairnum& a, const pairnum& b) { return pairnum(a) /= b; }
    friend pairnum operator == (const pairnum& a, const pairnum& b) { return a.v == b.v; }
    friend pairnum operator != (const pairnum& a, const pairnum& b) { return a.v != b.v; }

    friend std::ostream& operator << (std::ostream& out, const pairnum& n) { return out << '(' << n.t << ',' << ' ' << n.u << ')'; }
};

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

using num1 = modnum<int(1e9)+7>;
using num2 = modnum<int(1e9)+9>;
using pnum = pairnum<num1, num2>;

int main() {
	if (ID == 0) {
		num1 a = 20;
		num2 b = 3;
		cout << 3 + a << ' ' << 2/b << '\n';
		pnum p(a,b);
		cout << p / 5 << '\n';
	}

	return 0;
}
