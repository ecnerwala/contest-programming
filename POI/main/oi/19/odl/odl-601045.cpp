#include<bits/stdc++.h>
using namespace std;

const int V = 1e6 + 20;
const int MAXV = V + 20;

const int MAXN = 2e5;

const int INF = 1e9;

int N;
int A[MAXN];

int val[MAXN];
int res[MAXN];

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
		val[i] = INF;
		res[i] = -1;
	}

	for(int g = 1; g <= V; g++) {
		int m1 = INF, m2 = INF;
		int i1 = -1, i2 = -1;
		for(int i = g; i <= V; i += g) {
			if(vals[i].empty()) continue;
			int d = tau[i];
			if(d < m1) {
				m1 = d;
				i1 = vals[i][0];
				if(int(vals[i].size()) >= 2) {
					m2 = d;
					i2 = vals[i][1];
				}
			} else if (d < m2) {
				m2 = d;
				i2 = vals[i][0];
			}
		}

		for(int i = g; i <= V; i += g) {
			for(__typeof(vals[i].begin()) it = vals[i].begin(); it != vals[i].end(); it++) {
				int a = *it;
				int b = (a == i1 ? i2 : i1);
				int v = (a == i1 ? m2 : m1) + tau[i] - 2 * tau[g];
				if(v < val[a] || (v == val[a] && b < res[a])) {
					val[a] = v;
					res[a] = b;
				}
			}
		}
	}
	for(int i = 1; i <= N; i++) cout << res[i] << '\n';
	return 0;
}
