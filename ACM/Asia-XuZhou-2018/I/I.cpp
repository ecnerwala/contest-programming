#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXK = 20;

int N, K;
ll P;
int U[MAXK][2];

ll res = 0;
int perm[60];
void dfs(int ind) {
	if (ind == -1) {
		res++;
		return;
	}
	if (perm[U[ind][0]] < perm[U[ind][1]]) {
		dfs(ind-1);
		swap(perm[U[ind][0]], perm[U[ind][1]]);
		dfs(ind-1);
		swap(perm[U[ind][0]], perm[U[ind][1]]);
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	while (T--) {
		cin >> N >> K >> P;
		for (int i = 0; i < K; i++) {
			cin >> U[i][0] >> U[i][1];
			U[i][0] --;
			U[i][1] --;
		}
		res = 0;
		// case 1: no change
		for (int i = 0; i < N; i++) {
			perm[i] = i;
		}
		dfs(K-1);
		for (int i = 0; i < N-1; i++) {
			swap(perm[i], perm[i+1]);
			dfs(K-1);
			swap(perm[i], perm[i+1]);
		}
		for (int v = 0; v < N; v++) {
			for (int p = 0; p < N; p++) {
				if (abs(v-p) <= 1) continue;
				for (int i = 0; i < min(v, p); i++) {
					perm[i] = i;
				}
				if (v < p) {
					for (int i = v; i < p; i++) {
						perm[i] = i+1;
					}
					perm[p] = v;
				} else {
					for (int i = p; i < v; i++) {
						perm[i+1] = i;
					}
					perm[p] = v;
				}
				for (int i = max(v, p)+1; i < N; i++) {
					perm[i] = i;
				}
				dfs(K-1);
			}
		}

		res %= P;
		if (res < 0) res += P;
		cout << res << '\n';
	}

	return 0;
}
