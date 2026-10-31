#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e6;
const int MAXM = 1.1e6;
int N;
int M;

int A[MAXN];
int par[MAXN];

int getpar(int a) {
	return par[a] == -1 ? a : (par[a] = getpar(par[a]));
}
bool merge(int a, int b) {
	a = getpar(a), b = getpar(b);
	if (a == b) return false;
	par[a] = b;
	return true;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	A[0] = 0;
	for (int i = 1; i <= N; i++) cin >> A[i];

	vector<pair<int, int>> edges;
	for (int e = 0; e < M; e++) {
		int x, y; cin >> x >> y;
		edges.emplace_back(x, y);
	}
	for (int i = 1; i <= N; i++) edges.emplace_back(0, i);

	memset(par, -1, sizeof(par));
	sort(edges.begin(), edges.end(), [](pair<int, int> i, pair<int, int> j) -> bool {
		return make_pair(A[i.first] + A[i.second], i) > make_pair(A[j.first] + A[j.second], j);
	});
	long long ans = 0;
	for (auto it : edges) {
		if (merge(it.first, it.second)) {
			ans += A[it.first] + A[it.second];
		}
	}

	for (int i = 1; i <= N; i++) ans -= A[i];
	cout << ans << '\n';

	return 0;
}

// each tree is always rooted at the max

// if you get a cycle, you replace each edge into the cycle with one of cost 0
