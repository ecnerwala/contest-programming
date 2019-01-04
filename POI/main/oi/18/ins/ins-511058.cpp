#include<iostream>
#include<algorithm>
#include<vector>
#include<cassert>

using namespace std;

#define ITER(i, v) for(__typeof((v).begin()) i = (v).begin(); i != (v).end(); i++)

template <class T> void setmax(T &a, T b) { if(b > a) a = b; }
template <class T> void setmin(T &a, T b) { if(b < a) a = b; }

typedef long long ll;

const int MAXN = 2000000;

int N;
vector<int> adj[MAXN];

ll S[MAXN];

vector<int> split;

ll maxDist(int cur, int prv) {
	ll res = 0;
	ITER(it, adj[cur]) {
		int nxt = *it;
		if(nxt == prv) continue;

		setmax(res, maxDist(nxt, cur) + 1);
	}

	return res;
}

pair<ll, ll> totDist(int cur, int prv) {
	pair<ll, ll> ans (0, 1);
	ITER(it, adj[cur]) {
		int nxt = *it;
		if(nxt == prv) continue;

		pair<ll, ll> v = totDist(nxt, cur);
		ans.first += v.first + v.second;
		ans.second += v.second;
	}

	return ans;
}

ll numNodes(int cur, int prv) {
	ll totSz = 1;
	ll maxSz = 0;
	ITER(it, adj[cur]) {
		int nxt = *it;
		if(nxt == prv) continue;

		ll v = numNodes(nxt, cur);
		totSz += v;
		setmax(maxSz, v);
	}
	setmax(maxSz, N - totSz);
	
	if(maxSz <= N / 2) {
		split.push_back(cur);
	}

	return totSz;
}

void go() {
	for(int i = 0; i < N; i++) S[i] = -1;

	numNodes(0, -1);

	if(split.size() == 1) {
		int a = split[0];
		S[a] = 2 * totDist(a, -1).first - maxDist(a, -1);
	} else if(split.size() == 2) {
		int a = split[0], b = split[1];
		S[a] = 2 * totDist(a, -1).first - (1 + maxDist(b, a));
		S[b] = 2 * totDist(b, -1).first - (1 + maxDist(a, b));
	} else assert(false);
}

int main() {
	ios_base::sync_with_stdio(0);

	cin >> N;
	for(int i = 0; i < N - 1; i ++ ) {
		int a, b; cin >> a >> b; a--, b--;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}

	go();
	
	for(int i = 0; i < N; i++) {
		cout << S[i] << '\n';
	}
}

