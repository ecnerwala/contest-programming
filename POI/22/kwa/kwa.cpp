#include <bits/stdc++.h>
using namespace std;

const int V = 10000;
int K[V+1];
int opt[V+1];

int ssq(int n) {
	return n * (n+1) * (2*n+1) / 6;
}

void precomp() {
	memset(K, -1, sizeof(K));
	K[0] = 0;
	for (int v = 0, i = 1; v <= V; v += i * i, i++) {
		for (int j = 1; j <= i * i && v + j <= V; j++) {
			opt[v+j] = i;
		}
	}
	for (int i = 1; i * i <= V; i++) {
		for (int v = V; v >= i * i; v--) {
			if (K[v] == -1 && K[v - i * i] != -1) K[v] = i;
		}
	}
	for (int i = 1; i <= V; i++) {
		assert((K[ssq(opt[i]) - i] != -1) == (K[i] == opt[i]));
		if (K[i] == opt[i]) continue;
		if (K[i] != -1 && K[i] - opt[i] == 1) continue;
		/*
		   cout << i << ' ';
		   cout << opt[i] << "+";
		   if (K[i] == -1) cout << "∞";
		   else cout << K[i] - opt[i];
		   cout << ' ';
		   cout << "(-" << ssq(opt[i])-i << ")" << '\n';
	   */
	}
}

int slowcnt(int n) {
	int res = 0;
	for (int i = 0; i <= n; i++) {
		if (K[i] != opt[i]) res++;
	}
	return res;
}

int main() {
	precomp();
	long long n; cin >> n;
	assert(n > 0);
	long long mink = 0;
	long long excess = -n;
	while (excess < 0) {
		mink++;
		excess += mink * mink;
	}
	assert(0 <= excess);
	assert(excess < mink * mink);
	if (excess <= V && K[excess] == -1) {
		if (n <= V) {
			if (K[n] == -1) cout << '-';
			else cout << K[n];
		} else {
			cout << mink + 1;
		}
	} else {
		cout << mink;
	}
	cout << ' ';
	long long res = 0;
	for (long long i = 1, v = 0, cnt = 0; i < mink; i++) {
		// we want number of things <= V with excess == -1
		for (; v <= V && v < i * i; v++) {
			if (K[v] == -1) cnt++;
		}
		res += cnt;
	}
	// take care of the mink case separately
	for (long long v = excess; v <= V && v < mink * mink; v++) {
		if (K[v] == -1) res++;
	}
	cout << res;
	cout << '\n';
	if (n <= V) {
		assert(res == slowcnt(n));
	}
	return 0;
}
