#include<bits/stdc++.h>
using namespace std;
using ll = long long;

const int MAXN = 1.1e5;
int N;
ll M;

int par[MAXN];
int sz[MAXN];
int getpar(int a) {
	return par[a] == -1 ? a : (par[a] = getpar(par[a]));
}
bool merge(int a, int b) {
	a = getpar(a), b = getpar(b);
	if (a == b) return false;
	if (sz[a] < sz[b]) swap(a,b);
	par[b] = a;
	sz[a] += sz[b];
	return true;
}

const int MAXQ = 1.1e5;
int Q;
vector<pair<int, int>> paths[2];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M >> Q;
	for (int i = 0; i < N; i++) {
		par[i] = -1;
		sz[i] = 1;
	}

	ll numE = 0;
	for (int i = 0; i < Q; i++) {
		int a, b, c; cin >> a >> b >> c;
		paths[c].emplace_back(a,b);
		if (c == 0) {
			if (merge(a,b)) {
				numE++;
			}
		}
	}
	ll numCC = 0;
	for (int i = 0; i < N; i++) {
		if (getpar(i) == i) {
			numCC ++;
		}
	}
	assert(numCC + numE == N);
	for (auto e : paths[1]) {
		int a = e.first, b = e.second;
		if (getpar(a) == getpar(b)) {
			cout << "No\n";
			exit(0);
		}
	}
	if (numE + (numCC - paths[1].empty()) <= M && M <= numE + numCC * (numCC - 1) / 2) {
		cout << "Yes\n";
		exit(0);
	} else {
		cout << "No\n";
		exit(0);
	}

	return 0;
}
