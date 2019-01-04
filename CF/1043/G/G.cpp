#include<bits/stdc++.h>
using namespace std;

const int MAXN = 3e5;
int N;
char S[MAXN];

int suffind[MAXN]; // suffix start point
int suffval[MAXN]; // value of the ith suffix
int nsuffind[MAXN];
int nsuffval[MAXN];

int bucketSz[MAXN];

int lcp[MAXN];

int V;
int getVal(int a) {
	return a < N ? suffval[a] : V;
}
void suffixArray() {
	V = 26;
	for (int i = 0; i < N; i++) {
		suffval[i] = S[i] - 'a';
	}
	for (int l = 1; l <= N; l *= 2) {
		for (int i = 0; i <= V+1; i++) bucketSz[i] = 0;
		for (int i = 0; i < N; i++) bucketSz[getVal(i+l)+1]++;
		for (int i = 0; i <= V; i++) bucketSz[i+1] += bucketSz[i];
		assert(bucketSz[V+1] == N);

		for (int i = 0; i < N; i++) nsuffind[bucketSz[getVal(i+l)]++] = i;

		for (int i = 0; i <= V+1; i++) bucketSz[i] = 0;
		for (int i = 0; i < N; i++) bucketSz[suffval[nsuffind[i]]+1] ++;
		for (int i = 0; i <= V; i++) bucketSz[i+1] += bucketSz[i];
		assert(bucketSz[V+1] == N);

		for (int i = 0; i < N; i++) suffind[bucketSz[suffval[nsuffind[i]]]++] = nsuffind[i];

		nsuffval[suffind[0]] = 0;
		for (int i = 1; i < N; i++) {
			nsuffval[suffind[i]] = nsuffval[suffind[i-1]];
			if (suffval[suffind[i]] != suffval[suffind[i-1]] || getVal(suffind[i]+l) != getVal(suffind[i-1]+l)) {
				nsuffval[suffind[i]]++;
			}
		}

		V = nsuffval[suffind[N-1]] + 1;
		for (int i = 0; i < N; i++) {
			suffval[i] = nsuffval[i];
		}
	}
	assert(V == N);
	for (int i = 0; i < N; i++) {
		cerr << suffind[i] << ' ' << &S[suffind[i]] << '\n';
	}
	for (int i = 0; i < N; i++) {
		lcp[i] = 0;
	}

	for (int i = 0, len = 0; i < N; i++) {
		int val = suffval[i];
		if (val == N-1) {
			assert(len == 0);
			continue;
		}

		int j = suffind[val+1];
		while (i+len < N && j+len < N && S[i+len] == S[j+len]) {
			len++;
		}

		lcp[val] = len;

		if (len > 0) len--;
	}
}

template <typename T> T& setmax(T& a, T b) {
	if (b > a) a = b;
	return a;
}
template <typename T> T& setmin(T& a, T b) {
	if (b < a) a = b;
	return a;
}

int minAdj[MAXN];
set<int> genAdjHelper(int a, int b) {
	assert(a <= b);
	if (a == b) {
		set<int> res;
		res.insert(suffind[a]);
		return res;
	}
	int m = a;
	for (int i = a; i < b; i++) {
		if (lcp[i] < lcp[m]) m = i;
	}

	int l = lcp[m];

	assert(m < b);
	set<int> x = genAdjHelper(a, m);
	set<int> y = genAdjHelper(m+1, b);
	if (x.size() < y.size()) swap(x, y);
	for (int v : y) {
		auto it = x.insert(v).first;
		if (it != x.begin()) {
			auto pt = it;
			pt--;
			if (abs(*pt - v) <= l) {
				setmin(minAdj[min(*pt, v)], 2 * max(*pt, v) - min(*pt, v));
			}
		}
		assert(it != x.end());
		auto nt = it;
		nt++;
		if (nt != x.end()) {
			if (abs(*nt - v) <= l) {
				setmin(minAdj[min(*nt, v)], 2 * max(*nt, v) - min(*nt, v));
			}
		}
	}
	return x;
}

void genAdj() {
	suffixArray();
	for (int i = 0; i < N; i++) {
		minAdj[i] = N+1;
	}
	genAdjHelper(0, N-1);
	for (int i = 0; i < N; i++) {
		cerr << minAdj[i] << ' ';
	}
	cerr << '\n';
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> S;
	genAdj();

	return 0;
}
