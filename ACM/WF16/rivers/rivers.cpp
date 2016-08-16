#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll INF = 1e18;

typedef pair<ll, int> pli;

const int MAXN = 1e6;
const int MAXM = MAXN;
const int MAXV = MAXN + MAXM;
// rivers are M+1..M+N

int par[MAXV]; // parent pointer
ll len[MAXV]; // length to parent

vector<int> ch[MAXV];

ll depth[MAXV];
ll val[MAXV]; // min dist to any sink + len of parent

int deg[MAXV];

int N, M, V;
string names[MAXN];
int res[MAXN];


void dfs(int i) {
	if(i) depth[i] = depth[par[i]] + len[i];
	else depth[i] = 0;

	if(ch[i].empty()) {
		val[i] = 0;
	} else {
		val[i] = INF;
		for(int nxt : ch[i]) {
			dfs(nxt);
			val[i] = min(val[i], val[nxt]);
		}
	}
	val[i] += len[i];
}

int main() {
	cin >> N >> M;
	V = N + M + 1;
	for(int i = 1; i <= N; i++) {
		cin >> names[i];
		cin >> par[M + i] >> len[M + i];
		ch[par[M + i]].push_back(M + i);
	}
	for(int i = 1; i <= M; i++) {
		cin >> par[i] >> len[i];
		ch[par[i]].push_back(i);
	}

	dfs(0);

	vector<pli> queries(N);
	for(int i = 1; i <= N; i++) {
		queries[i-1] = pli(depth[M + i], i);
	}
	sort(queries.begin(), queries.end());

	priority_queue<pli, vector<pli>, greater<pli>> pq;

	for(int i = 0; i < V; i++) {
		deg[i] = int(ch[i].size());
		assert((deg[i] == 0) == (i > M));
		if(deg[i] == 0) {
			pq.emplace(val[i],i);
		}
	}

	for(auto it : queries) {
		ll v = it.first;
		int ind = it.second;
		//cerr << "query " << v << ' ' << ind << ' ' << names[ind] << '\n';
		while(!pq.empty() && pq.top().first <= v) {
			int a = pq.top().second;
			assert(pq.top().first == val[a]);
			pq.pop();

			//cerr << a << ' ' << par[a] << ' ' << deg[par[a]] << '\n';
			assert(deg[par[a]]);
			deg[par[a]] --;
			if(par[a] && deg[par[a]] == 0) {
				pq.emplace(val[par[a]], par[a]);
			}

		}
		res[ind] = int(pq.size());
	}

	for(int i = 1; i <= N; i++) {
		cout << names[i] << ' ' << res[i] + 1 << '\n';
	}
}
