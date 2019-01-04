#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e5;
int N, M;
int A[MAXN];
int deg[MAXN];
int cnt[2];
int cntCon[2];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	for (int i = 1; i <= N; i++) cin >> A[i];
	for (int i = 0; i < M; i++) {
		int u, v; cin >> u >> v;
		if (A[u] != A[v]) {
			deg[u] ++;
			deg[v] ++;
		}
	}
	for (int i = 1; i <= N; i++) {
		cnt[A[i]] ++;
		if (deg[i]) cntCon[A[i]] ++;
	}

	for (int i = 1; i <= N; i++) {
		int res = cnt[A[i]] - 1;
		if (deg[i]) {
			res += 2 * cnt[!A[i]] - deg[i];
		} else {
			res += 3 * cnt[!A[i]] - cntCon[!A[i]];
		}
		cout << res << " \n"[i==N];
	}


	return 0;
}
