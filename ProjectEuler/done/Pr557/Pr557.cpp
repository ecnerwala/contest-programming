#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

int gcd(int a, int b) {
	return a ? gcd(b % a, a) : b;
}

pii make_frac(int p, int q) {
	int g = gcd(p, q);
	p /= g, q /= g;
	return pii(p, q);
}

pii v(int a, int b) {
	return make_frac(a + b, a * b);
}

long long S(int n) {
	map<pii, int> cnt;
	long long res = 0;
	for(int s = 1; s <= n; s++) {
		for(int a = 1; a <= s; a++) {
			res += (long long)(s) * cnt[v(s, a)];
			cnt[v(s, a)] ++;
		}
	}
	return res;
}

int main() {
	assert(S(20) == 259);
	cout << S(10000) << '\n';
	return 0;
}
