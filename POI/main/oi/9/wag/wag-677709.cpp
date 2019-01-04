#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1024;
typedef bitset<MAXN * 2> bs;

const int M = 5e4 + 20;
const int MAXW = M * 2;
int N;
int W[MAXN];
bool vis[MAXW];
bs dp[MAXW];

int main() {
	scanf("%d", &N);
	for(int i = 0; i < N; i++) scanf("%d", &W[i]);
	sort(W, W + N);
	int S = 0;
	vis[M] = true;
	for(int z = 0; z < N; z++) {
		int w = W[z];
		for(int i = -S; i <= S; i++) {
			if(vis[i + M]) {
				vis[i - w + M] = true;
				dp[i - w + M] = dp[i + M];
				dp[i - w + M][z] = true;
			}
		}
		for(int i = S; i >= -S; i--) {
			if(vis[i + M]) {
				if(dp[i + M][z]) {
					if(vis[i - w + M] && dp[i - w + M][z]) {
						vis[i + w + M] = true;
						dp[i + w + M] = dp[i - w + M];
						dp[i + w + M][z] = false;
						dp[i + w + M][z + N] = true;
					}
				} else {
					vis[i + w + M] = true;
					dp[i + w + M] = dp[i + M];
					dp[i + w + M][z + N] = true;
				}
			}
		}
		S += w;
	}
	vector<int> a, b;
	for(int i = 0; i < N; i++) {
		if(dp[M][i] == dp[M][i + N]) {
			continue;
		} else if(dp[M][i]) {
			a.push_back(W[i]);
		} else if(dp[M][i + N]) {
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
