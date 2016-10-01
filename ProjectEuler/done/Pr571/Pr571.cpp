#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int N = 12;
const int K = 10;
int dig[N];

bool is_pandigital(ll v, int b) {
	int m = 0;
	for(; v; v /= b) {
		m |= (1 << (v % b));
	}
	return m == (1 << b) - 1;
}

bool is_super_pandigital(ll v, int b) {
	for(int i = b-1; i >= 2; i--) {
		if(!is_pandigital(v, i)) return false;
	}
	return true;
}

int main() {
	dig[0] = 1;
	dig[1] = 0;
	for(int i = 2; i < N; i++) {
		dig[i] = i;
	}

	ll res = 0;
	int cnt = 0;
	do {
		ll v = 0;
		for(int i = 0; i < N; i++) {
			v *= N;
			v += dig[i];
		}
		if(is_super_pandigital(v, N)) {
			cerr << v << '\n';
			res += v;
			cnt++;
		}
	} while(cnt < K && next_permutation(dig, dig + N));
	cout << res << '\n';

	return 0;
}
