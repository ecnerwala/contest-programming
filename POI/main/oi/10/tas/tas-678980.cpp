#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 1e6 + 500;
int N, L;
int B[MAXN];
bool vis[MAXN];
vector<int> cyc[MAXN];
int V;
int vals[MAXN];
int G[MAXN];

int main() {
	cin >> N >> L;
	for(int i = 0; i < N; i++) {
		cin >> B[i];
		B[i] --;
	}

	for(int i = 1; i <= N; i++) G[i] = 1;

	for(int i = 2, cur = L; cur > 1; i++) {
		if(cur % i == 0) {
			int q = 1;
			while(cur % i == 0) cur /= i, q *= i;
			for(int j = i; j <= N; j += i) {
				G[j] *= q;
			}
		}
	}

	for(int i = 0; i < N; i++) {
		if(vis[i]) continue;
		int s = V;
		vals[V++] = i;
		vis[i] = true;
		for(int cur = B[i]; cur != i; cur = B[cur]) {
			vals[V++] = cur;
			vis[cur] = true;
		}
		int t = V;
		cyc[t - s].push_back(s);
	}
	assert(V == N);

	for(int b = 1; b <= N; b++) {
		if(int(cyc[b].size()) % G[b] != 0) {
			assert(false);
		}
		int a = b * G[b];
		while(!cyc[b].empty()) {
			vector<int> c(a);
			for(int i = 0; i < G[b]; i++) {
				int s = cyc[b].back(), t = s + b;
				cyc[b].pop_back();
				for(int j = s, k = i; j < t; j++, k += L, k %= a) {
					c[k] = vals[j];
				}
			}
			c.push_back(c[0]);
			for(int i = 0; i < a; i++) {
				B[c[i]] = c[i + 1];
			}
		}
	}

	for(int i = 0; i < N; i++) {
		cout << B[i] + 1 << '\n';
	}
}
