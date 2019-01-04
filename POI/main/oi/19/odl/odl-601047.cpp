#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

const int V = 1e6 + 20;
const int MAXV = V + 20;

const int MAXN = 2e5;

const int INF = 1e9;

int N;
int A[MAXN];

pii res[MAXN];

vector<int> vals[MAXV];

int tau[MAXV];

int main() {
	for(int i = 1; i <= V; i++) {
		for(int j = i; j <= V; j += i) {
			tau[j] ++;
		}
	}

	scanf("%d", &N);
	for(int i = 1; i <= N; i++) {
		scanf("%d", &(A[i]));
		vals[A[i]].push_back(i);
		res[i] = pii(INF, INF);
	}

	for(int g = 1; g <= V; g++) {
		pii m1(INF, INF), m2(INF, INF);
		for(int i = g; i <= V; i += g) {
			if(vals[i].empty()) continue;
			for(int j = 0; j < min(2, int(vals[i].size())); j++) {
				pii d(tau[i] - 2 * tau[g], vals[i][j]);;
				if(d < m1) {
					m2 = m1;
					m1 = d;
				} else if (d < m2) {
					m2 = d;
				}
			}
		}

		for(int i = g; i <= V; i += g) {
			for(__typeof(vals[i].begin()) it = vals[i].begin(); it != vals[i].end(); it++) {
				int a = *it;
				pii v = (a == m1.second ? m2 : m1);
				v.first += tau[i];
				if(v < res[a]) {
					res[a] = v;
				}
			}
		}
	}
	for(int i = 1; i <= N; i++) printf("%d\n", res[i].second);
	return 0;
}
