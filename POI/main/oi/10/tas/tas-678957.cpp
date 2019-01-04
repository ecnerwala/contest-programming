#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 1e6;
int N, L;
int B[MAXN];
bool vis[MAXN];
typedef vector<int> vi;
vector<vi> cyc[MAXN];
int G[MAXN];

vector<pii> primes;

int main() {
	cin >> N >> L;
	for(int i = 0; i < N; i++) {
		cin >> B[i];
		B[i] --;
	}

	for(int i = 1; i <= N; i++) G[i] = 1;

	for(int i = 2, cur = L; cur > 1; i++) {
		if(cur % i == 0) {
			int cnt = 0;
			int q = 1;
			while(cur % i == 0) cur /= i, q *= i, cnt++;
			primes.push_back(pii(i, cnt));
			for(int j = i; j <= N; j += i) {
				G[j] *= q;
			}
		}
	}

	for(int i = 0; i < N; i++) {
		if(vis[i]) continue;
		vi c;
		c.push_back(i);
		vis[i] = true;
		for(int cur = B[i]; cur != i; cur = B[cur]) {
			c.push_back(cur);
			vis[cur] = true;
		}
		cyc[c.size()].push_back(c);
	}

	for(int b = 1; b <= N; b++) {
		assert(int(cyc[b].size()) % G[b] == 0);
		int a = b * G[b];
		while(!cyc[b].empty()) {
			vector<int> c(a);
			for(int i = 0; i < G[b]; i++) {
				for(int j = 0, k = i; j < b; j++, k += L, k %= a) {
					c[k] = cyc[b].back()[j];
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
