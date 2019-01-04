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
		int len = 1;
		vis[i] = true;
		for(int cur = B[i]; cur != i; cur = B[cur]) {
			len++;
			vis[cur] = true;
		}
		cyc[len].push_back(i);
	}

	for(int b = 1; b <= N; b++) {
		if(int(cyc[b].size()) % G[b] != 0) {
			assert(false);
		}
		int a = b * G[b];
		while(!cyc[b].empty()) {
			vector<int> c(a);
			for(int i = 0; i < G[b]; i++) {
				for(int j = 0, k = i, cur = cyc[b].back(); j < b; j++, k += L, k %= a, cur = B[cur]) {
					c[k] = cur;
				}
				cyc[b].pop_back();
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
