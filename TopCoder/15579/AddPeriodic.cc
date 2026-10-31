#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using ll = __int128_t;

struct frac {
	ll n, d;
	explicit frac(ll n_ = 0, ll d_ = 1) : n(n_), d(d_) {
		//assert(n >= 0 && d >= 0);
		//assert(d);
		ll g = __gcd(n, d);
		n /= g, d /= g;
	}
	friend frac operator + (const frac& a, const frac& b) {
		ll g = __gcd(a.d, b.d);
		return frac{a.n * (b.d/g) + b.n * (a.d/g), a.d / g * b.d};
	}
};

string to_string(ll a) {
	assert(a >= 0);
	if (a == 0) return "0";
	string res;
	while (a) {
		res += char('0' + a % 10);
		a /= 10;
	}
	reverse(res.begin(), res.end());
	return res;
}

class AddPeriodic {

public:

	frac parseString(string S) {
		auto it = S.begin();
		ll X = 0;
		while (*it != '.') {
			X *= 10;
			X += (*it - '0');
			++it;
		}
		assert(*it == '.');
		++it;

		ll Y = 0;
		ll yden = 1;
		while (*it != '(') {
			Y *= 10;
			yden *= 10;
			Y += (*it - '0');
			++it;
		}
		assert(*it == '(');
		++it;

		ll Z = 0;
		ll zden = 1;
		while (*it != ')') {
			Z *= 10;
			zden *= 10;
			Z += (*it - '0');
			++it;
		}
		assert(*it == ')');
		++it;
		assert(it == S.end());
		return frac{X, 1} + frac{Y, yden} + frac{Z, (zden - 1) * yden};
	}

	string buildString(frac f) {
		string X = to_string(f.n / f.d);
		f.n %= f.d;
		string Y = "";
		while (f.d % 2 == 0 || f.d % 5 == 0) {
			f = frac(f.n * 10, f.d);
			Y += char('0' + (f.n / f.d));
			f.n %= f.d;
		}
		int l = 1;
		ll zden = 9;
		while (zden % f.d != 0) {
			l++;
			zden *= 10, zden += 9;
		}
		string Z = to_string(f.n * (zden / f.d));
		while (int(Z.size()) < l) Z = '0' + Z;
		return X + "." + Y + "(" + Z + ")";
	}

	string add(string A, string B) {
		return buildString(parseString(A) + parseString(B));
	}
};

void my_tests() {
	// FILL IN TESTS
	cout << AddPeriodic().add("9.(62668388)", "6.6(7551662)") << '\n';
}

}  // namespace ecnerwala

using ecnerwala::AddPeriodic;

// vim:ft=cpp
