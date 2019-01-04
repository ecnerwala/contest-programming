#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1024;

int N;
int W[MAXN];
vector<char> prv[MAXN];

int main() {
	scanf("%d", &N);
	for(int i = 0; i < N; i++) scanf("%d", &W[i]);
	sort(W, W + N);
	int S = 0;
	prv[0].push_back(1);
	for(int z = 0; z < N; z++) {
		int w = W[z];
		prv[z + 1].resize(S + w + 1);
		for(int i = 0; i <= S; i++) {
			if(prv[z][i]) {
				prv[z + 1][i] = 1;
			}
		}
		for(int i = 0; i <= S; i++) {
			if(prv[z][i]) {
				prv[z + 1][i + w] = 2;
				if(i >= w) prv[z + 1][i - w] = 3;
				else prv[z + 1][w - i] = 4;
			}
		}
		S += w;
	}
	vector<int> a, b;
	for(int i = N, val = 0; i > 0; i--) {
		int t = prv[i][val];
		int w = W[i - 1];
		switch(t) {
			case 1: 
				break;
			case 2: 
				a.push_back(w);
				val -= w;
				break;
			case 3:
				b.push_back(w);
				val += w;
				break;
			case 4:
				a.push_back(w);
				val -= w;
				val *= -1;
				swap(a, b);
				break;
			case 0: assert(false);
		}
		assert(val >= 0);
	}
	for(int i = 0; i < N; i++) {
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
}
