#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.2e3;
typedef bitset<MAXN * 2> bs;
const int MAXW = 1.2e5;

const int M = 6e4;
int N;
int W[MAXN];
bs* dp[MAXW];
bs* ndp[MAXW];

int main() {
	scanf("%d", &N);
	for(int i = 0; i < N; i++) scanf("%d", &W[i]);
	sort(W, W + N);
	dp[M] = new bs;
	int S = 0;
	for(int z = 0; z < N; z++) {
		int w = W[z];
		for(int i = -S; i <= S; i++) {
			if(dp[i + M]) {
				if(!ndp[i - w + M]) {
					ndp[i - w + M] = new bs(*dp[i + M]);
					ndp[i - w + M]->set(z);
				}
				if(!ndp[i + w + M]) {
					ndp[i + w + M] = new bs(*dp[i + M]);
					ndp[i + w + M]->set(z + N);
				}
			}
		}
		S += w;
		for(int i = -S; i <= S; i++) {
			if(ndp[i + M]) {
				delete(dp[i + M]);
				dp[i + M] = ndp[i + M];
				ndp[i + M] = NULL;
			}
		}
	}
	assert(dp[M]);
	vector<int> a, b;
	for(int i = 0; i < N; i++) {
		if(dp[M]->test(i)) {
			a.push_back(W[i]);
		}
	}
	for(int i = 0; i < N; i++) {
		if(dp[M]->test(i + N)) {
			b.push_back(W[i]);
		}
	}

	cout << a.size() << ' ' << b.size() << '\n';
	sort(a.begin(), a.end());
	sort(b.begin(), b.end());
	assert(!a.empty());
	assert(!b.empty());
	if(!a.empty()) {
		cout << a[0];
		for(int i = 1; i < int(a.size()); i++) {
			cout << ' ' << a[i];
		}
	}
	cout << '\n';

	if(!b.empty()) {
		cout << b[0];
		for(int i = 1; i < int(b.size()); i++) {
			cout << ' ' << b[i];
		}
	}
	cout << '\n';

	/*
	int val = 0;
	for(int i = 0, j = 0; i < int(a.size()); i++) {
		assert(a[i] > 0);
		val += a[i];
		while(W[j] != a[i]) {
			j++;
			assert(j < N);
		}
		assert(W[j] == a[i]);
		W[j] = -1;
	}
	for(int i = 0, j = 0; i < int(b.size()); i++) {
		assert(b[i] > 0);
		val -= b[i];
		while(W[j] != b[i]) {
			j++;
			assert(j < N);
		}
		assert(W[j] == b[i]);
		W[j] = -1;
	}
	assert(val == 0);
	*/
}
