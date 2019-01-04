#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 3e5;
const int MAXM = 1.5e6;
int N;
int M;

int P[MAXN];
vector<int> ch[MAXN];
int C[MAXN];
priority_queue<int> pq[MAXN];

ll dfs(int cur) {
	ll res = 0;
	for (int nxt : ch[cur]) {
		res += dfs(nxt);
		if (pq[nxt].size() > pq[cur].size()) {
			swap(pq[cur], pq[nxt]);
		}
		while (!pq[nxt].empty()) {
			pq[cur].push(pq[nxt].top()); pq[nxt].pop();
		}
	}
	pq[cur].push(cur);
	while (!pq[cur].empty() && C[cur]) {
		res += pq[cur].top(); pq[cur].pop();
		C[cur] --;
	}
	return res;
}

ll bfs() {
	vector<int> vals;
	vals.push_back(0);
	for (int i = 0; i < N; i++) {
		int cur = vals[i];
		for (int nxt : ch[cur]) {
			vals.push_back(nxt);
		}
	}
	ll res = 0;
	for (int i = N-1; i >= 0; i--) {
		int cur = vals[i];
		for (int nxt : ch[cur]) {
			if (pq[nxt].size() > pq[cur].size()) {
				swap(pq[cur], pq[nxt]);
			}
			while (!pq[nxt].empty()) {
				pq[cur].push(pq[nxt].top()); pq[nxt].pop();
			}
		}
		pq[cur].push(cur);
		while (!pq[cur].empty() && C[cur]) {
			res += pq[cur].top(); pq[cur].pop();
			C[cur] --;
		}
	}
	return res;
}

int getpar(int i) {
	assert(-1 <= i && i < N);
	if (i == -1) return i;
	if (C[i]) return i;
	return P[i] = getpar(P[i]);
}

ll crazy() {
	P[0] = -1;

	ll res = 0;
	for (int i = N-1; i >= 0; i--) {
		int p = getpar(i);
		if (p != -1) {
			assert(C[p]);
			C[p] --;
			res += i;
		}
	}
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		cin >> N >> M;
		ll A, B; cin >> A >> B;
		for (int i = 0; i < M; i++) {
			C[(ll(A)*ll(i)+B) % N] ++;
		}
		for (int i = 1; i < N; i++) {
			cin >> P[i];
			assert(0 <= P[i] && P[i] < N);
			ch[P[i]].push_back(i);
		}

		//ll res = dfs(0);
		//ll res = bfs();
		ll res = crazy();

		cout << "Case #" << t << ": " << res << '\n';

		for (int i = 0; i < MAXN; i++) {
			P[i] = 0;
			ch[i] = vector<int>();
			C[i] = 0;
			pq[i] = priority_queue<int>();
		}
	}

	return 0;
}
