#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 600;
const int MAXM = 600;

int N, M;
ll C[MAXN][MAXM];

int main() {
	int __T; cin >> __T;
	for(int T = 1; T <= __T; T++) {
		cout << "Case #" << T << ": ";

		ll res = 0;
		priority_queue<ll, vector<ll>, greater<ll>> pq;

		cin >> N >> M;
		assert(N < MAXN);
		assert(M < MAXM);

		for (int i = 0; i < N; i++) {
			for(int j = 0; j < M; j++) {
				cin >> C[i][j];
			}

			sort(C[i], C[i] + M);
			for(int j = 0; j < M; j++) {
				C[i][j] += (2 * j + 1);
				if (j) assert(C[i][j] > C[i][j-1]);
				pq.push(C[i][j]);
			}

			res += pq.top(); pq.pop();
		}
		cout << res << '\n';
	}

	return 0;
}
