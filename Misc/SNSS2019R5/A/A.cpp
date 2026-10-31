#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e4;

const int MAXV = MAXN * 2;

[[noreturn]] void NO_SAD() {
	cout << -1 << '\n';
	exit(0);
}

int N, V;
map<string, int> mp;
pair<int, int> A[MAXN];

int readString() {
	string s; cin >> s;
	if (mp.count(s)) return mp[s];
	return mp[s] = V++;
}

int par[MAXV * 2];
int getpar(int a) {
	return par[a] == -1 ? a : getpar(par[a]);
}
bool merge(int a, int b) {
	a = getpar(a), b = getpar(b);
	if (a == b) return false;
	par[b] = a;
	return true;
}

int cost[MAXV * 2];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		A[i].first = readString();
		A[i].second = readString();
	}

	memset(par, -1, sizeof(par));
	for (int i = 0; i < N; i++) {
		int u = A[i].first, v = A[i].second;
		if (u == v) {
			NO_SAD();
		}
		merge(2 * u, 2 * v + 1);
		merge(2 * u + 1, 2 * v);
	}
	for (int i = 0; i < V; i++) {
		if (getpar(2 * i) == getpar(2 * i + 1)) {
			NO_SAD();
		}
	}

	for (int i = 0; i < N; i++) {
		int u = A[i].first, v = A[i].second;
		assert(getpar(2 * u) == getpar(2 * v + 1));
		assert(getpar(2 * u + 1) == getpar(2 * v));

		assert((getpar(2 * u) ^ getpar(2 * u + 1)) == 1);

		int a = getpar(2 * u + 1);
		cost[a] ++;
	}

	int ans = 0;
	for (int i = 0; i < V; i++) {
		ans += min(cost[2 * i], cost[2 * i + 1]);
	}
	cout << ans << '\n';

	return 0;
}
