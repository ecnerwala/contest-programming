#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXV = 1.2e6;
const int V = 1.1e6;
bool isPrime[MAXV];
int primeDist[MAXV];

void precomp() {
	for (int v = 2; v <= V; v++) isPrime[v] = true;
	for (int p = 2; p <= V; p++) {
		if (!isPrime[p]) continue;
		for (int j = p + p; j <= V; j += p) isPrime[j] = false;
	}
	primeDist[0] = 3;
	primeDist[1] = 2;
	primeDist[2] = 1;
	for (int v = 3; v <= V; v++) {
		primeDist[v] = isPrime[v] ? 0 : (primeDist[v-1] + 1);
	}
	for (int v = V-1; v >= 3; v--) {
		primeDist[v] = min(primeDist[v], isPrime[v] ? 0 : (primeDist[v+1] + 1));
	}
}

const int MAXN = 2.1e5;
int N;
ll M;
int A[MAXN];
ll dp[MAXN][2];
bool prv[MAXN][2];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	precomp();

	cin >> N >> M;
	for (int i = 0; i < N; i++) {
		cin >> A[i];
	}

	dp[0][0] = 0;
	dp[0][1] = M+1;
	for (int i = 0; i < N; i++) {
		dp[i+1][0] = dp[i+1][1] = M+1;
		prv[i+1][0] = prv[i+1][1] = -1;
		for (int z = 0; z < 2; z++) {
			for (int v = 0; v < 2; v++) {
				ll dist = dp[i][z] + (v ? primeDist[A[i]] : abs(A[i] - 2));
				int nz = z ^ v ^ (A[i] & 1);
				if (dist < dp[i+1][nz]) {
					dp[i+1][nz] = dist;
					prv[i+1][nz] = v;
				}
			}
		}
	}
	vector<pair<ll, ll>> res(N);
	int z = M&1;
	if (dp[N][M&1] <= M) {
		// take it
		for (int i = N-1; i >= 0; i--) {
			int v = prv[i+1][z];
			assert(v != -1);
			if (v) {
				if (isPrime[A[i] + primeDist[A[i]]]) {
					res[i].second = primeDist[A[i]];
				} else if (isPrime[A[i] - primeDist[A[i]]]){
					res[i].first = primeDist[A[i]];
				} else assert(false);
			} else {
				if (A[i] >= 2) {
					res[i].first = A[i] - 2;
				} else {
					res[i].second = 2 - A[i];
				}
			}
			z ^= v ^ (A[i] & 1);
		}
		for (int i = 0; i < N; i++) {
			M -= res[i].first;
			M -= res[i].second;
		}
		assert(M >= 0);
		assert(M % 2 == 0);
		res[0].first += M / 2;
		res[0].second += M / 2;

		cout << "YES" << '\n';
		for (int i = 0; i < N; i++) {
			cout << res[i].first << ' ' << res[i].second << '\n';
		}
		exit(0);
	} else {
		cout << "NO" << '\n';
		exit(0);
	}

	return 0;
}
