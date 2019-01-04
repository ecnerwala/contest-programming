#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXV = 1e6 + 60;
int V;

const int MAXN = 2e5;

const int INF = 1e9;

int N;
int A[MAXN];

pii res[MAXN];

vector<int> vals[MAXV];

int tau[MAXV];

int main() {
	scanf("%d", &N);
	V = 0;
	for(int i = 1; i <= N; i++) {
		scanf("%d", &(A[i]));
		vals[A[i]].push_back(i);
		res[i] = pii(INF, INF);
		V = max(V, A[i]);
	}

	for(int i = 2; i <= V; i++) {
		if(tau[i] == 0) {
			for(int j = i; j <= V; j += i) {
				for(int v = j; v % i == 0; v /= i) tau[j] ++;
			}
		}
	}

	for(int g = 1; g <= V; g++) {
		pii m1(INF, INF), m2(INF, INF);
		for(int i = g; i <= V; i += g) {
			if(vals[i].empty()) continue;
			for(int j = 0; j < min(2, int(vals[i].size())); j++) {
				pii d(tau[i] - 2 * tau[g], vals[i][j]);
				if(d < m1) {
					m2 = m1;
					m1 = d;
				} else if (d < m2) {
					m2 = d;
				}
			}
		}
		if(m2 == pii(INF, INF)) continue;
		//cerr << g << '\n';
		//cerr << m1.A << ' ' << m1.B << '\n';
		//cerr << m2.A << ' ' << m2.B << '\n';

		for(int i = g; i <= V; i += g) {
			if(vals[i].empty()) continue;
			for(__typeof(vals[i].begin()) it = vals[i].begin(); it != vals[i].end(); it++) {
				int a = *it;
			//for(int j = 0; j < int(vals[i].size()); j++) {
				//int a = vals[i][j];
				pii v = (a == m1.second ? m2 : m1);
				if(v < res[a]) {
					res[a] = v;
				}
			}
		}
	}
	for(int i = 1; i <= N; i++) printf("%d\n", res[i].second);
	return 0;
}
