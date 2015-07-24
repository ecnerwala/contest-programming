#include<bits/stdc++.h>
using namespace std;

#define A first
#define B second

typedef long long ll;
const ll MOD = 1e9 + 7;

const int MAXN = 2e6;
const int MAXC = 2e6;
const int MAXQ = 2e6;

int N, C, Q;

int P[MAXN];
vector<int> ch[MAXN];
int depth[MAXN];

int A[MAXQ];
int L[MAXQ]; // max depth, inclusive
int V[MAXQ];
vector<int> queries[MAXN];

ll res;

set<int> evts[MAXN];

void insert(int cur) {
	for(int t : queries[cur]) {
		assert(0 <= t && t <= Q);
		if(V[t] == 0) {
			// c is 0, query
			int p = -1;
			for(int i = depth[cur]; i <= N; i += i & (-i)) {
				auto it = evts[i].lower_bound(t);
				if(it == evts[i].begin()) continue;
				it--;
				p = max(p, *it);
			}
			assert(p < t);
			assert(p >= 0);
			int v = V[p];
			res = (res + (t % MOD) * (v % MOD)) % MOD;
		} else {
			for(int i = L[t]; i; i -= i & (-i)) {
				evts[i].insert(t);
			}
		}
	}
}

void remove(int cur) {
	for(int t : queries[cur]) {
		if(V[t]) {
			for(int i = L[t]; i; i -= i & (-i)) {
				evts[i].erase(t);
			}
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0);

	int T; cin >> T;

	while(T--) {
		cin >> N >> C >> Q;
		P[0] = -1;
		depth[0] = 1;
		for(int i = 0; i < N; i++) ch[i].clear(), queries[i].clear();
		for(int i = 1; i < N; i++) {
			cin >> P[i];
			P[i] --;
			depth[i] = depth[P[i]] + 1;

			ch[P[i]].push_back(i);
		}

		A[0] = 0;
		L[0] = N;
		V[0] = 1;
		queries[0].push_back(0);

		for(int i = 1; i <= Q; i++) {
			cin >> A[i] >> L[i] >> V[i];
			A[i] --;
			assert(0 <= A[i] && A[i] < N);
			L[i] += depth[A[i]];
			L[i] = min(L[i], N);
			queries[A[i]].push_back(i);
		}

		res = 0;

		for(int i = 1; i <= N; i++) {
			assert(evts[i].empty());
		}

		stack<int> st;
		insert(0);
		st.push(0);
		while(!st.empty()) {
			int cur = st.top();
			if(ch[cur].empty()) {
				remove(cur);
				st.pop();
			} else {
				int nxt = ch[cur].back();
				ch[cur].pop_back();
				insert(nxt);
				st.push(nxt);
			}
		}

		cout << res << '\n';
	}

	return 0;
}
