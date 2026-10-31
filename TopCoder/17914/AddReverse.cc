#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace um_nik {
const int N = 5050;
int a[N];
int b[N];
int n;
bool solve2(int L) {
	int p = 0;
	int q = L-1;
	for (int i = 0; i < L; i++) {
		b[i] = 0;
	}
	while (p < q) {
		if (a[p] < 0) {
			a[p] += 10;
			a[p+1]--;
		}
		if (a[q] > a[p] + 2) {
			a[p] += 10;
			a[p+1]--;
		}
		a[q] -= a[p];
		if (a[q] < 0) return false;
		if (a[q] > 1) return false;
		if (a[p] > 18) return false;
		b[q] = min(9, a[p]);
		a[p] -= b[q];
		b[p] = a[p];
		a[p] = 0;
		p++;
		a[q-1] += a[q] * 10;
		a[q] = 0;
		q--;
	}
	if (p == q) {
		if (a[p] > 18 || a[p] < 0) return false;
		if (a[p] % 2 == 1) return false;
		b[p] = a[p] / 2;
	}
	if (b[L-1] <= 0) return false;
	return true;
}

class AddReverse {
public:
	string solve(string s) {
		n = s.length();
		for (int L = n; L > n-2; L--) {
			if (L == 0) continue;
			for (int i = 0; i < N; i++) a[i] = 0;
			for (int i = 0; i < n; i++) a[i] = (int)(s[i] - '0');
			reverse(a, a+n);
			for (int i = n-1; i >= L; i--) {
				a[i-1] += a[i]*10;
				a[i] = 0;
			}
			if (!solve2(L)) continue;
			s = "";
			assert(b[L-1] != 0);
			for (int i = L-1; i >= 0; i--) {
				assert(0 <= b[i] && b[i] < 10);
				s.push_back('0' + b[i]);
			}
			return s;
		}
		return "";
	}
};

}

namespace ecnerwala {

class AddReverse {

public:

	string solve(string N) {
		return um_nik::AddReverse().solve(N);
	}
};

void my_tests() {
	// FILL IN TESTS
	cerr << um_nik::AddReverse().solve("21") << '\n';
	cerr << um_nik::AddReverse().solve("110001") << '\n';
}

}  // namespace ecnerwala

using ecnerwala::AddReverse;

// vim:ft=cpp
