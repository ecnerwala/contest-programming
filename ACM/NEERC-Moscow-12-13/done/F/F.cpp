#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e5;

int N, K;

int indeg[MAXN];
vector<int> nxt[MAXN];

double val[MAXN];
int cap[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> K;
	for(int i = 0; i < N; i++) {
		int P, A; cin >> P >> A;
		cap[i] = P;
		val[i] = A;
	}
	for(int i = 0; i < K; i++) {
		int a, b; cin >> a >> b;
		a--, b--;
		indeg[b] ++;
		nxt[a].push_back(b);
	}

	stack<int> q;
	for(int i = 0; i < N; i++) {
		if(indeg[i] == 0) q.push(i);
	}

	int X, Y, Z; cin >> X >> Y >> Z;
	X--, Z--;
	val[X] += Y;

	while(!q.empty()) {
		int a = q.top(); q.pop();
		double v = max(0., val[a] - cap[a]);
		val[a] -= v;
		if(nxt[a].empty()) continue;
		v = v / nxt[a].size();
		for(int b : nxt[a]) {
			val[b] += v;
			indeg[b] --;
			if(indeg[b] == 0) q.push(b);
		}
	}

	cout << fixed << setprecision(10) << val[Z] << '\n';
	return 0;
}
