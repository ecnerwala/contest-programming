#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e5;
int N;
int par[MAXN];
int getPar(int a) {
	return (par[a] == -1) ? a : (par[a] = getPar(par[a]));
}
bool merge(int a, int b) {
	a = getPar(a), b = getPar(b);
	if (a == b) return false;
	par[b] = a;
	return true;
}
pair<int, int> X[MAXN];
pair<int, int> Y[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	memset(par, -1, sizeof(par));
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> X[i].first >> Y[i].first;
		X[i].second = Y[i].second = i;
	}
	sort(X,X+N);
	sort(Y,Y+N);
	vector<pair<int, pair<int, int>>> edges;
	for (int i = 0; i+1 < N; i++) {
		edges.push_back({X[i+1].first-X[i].first, {X[i].second, X[i+1].second}});
		edges.push_back({Y[i+1].first-Y[i].first, {Y[i].second, Y[i+1].second}});
	}
	sort(edges.begin(), edges.end());
	long long ans = 0;
	for (auto it : edges) {
		if (merge(it.second.first, it.second.second)) {
			ans += it.first;
		}
	}
	cout << ans << '\n';

	return 0;
}
