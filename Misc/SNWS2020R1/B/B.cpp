#include<bits/stdc++.h>
using namespace std;

using ll = long long;

template <typename T> void setmin(T& a, const T& b) { if (b < a) a = b; }
template <typename T> void setmax(T& a, const T& b) { if (b > a) a = b; }

const int MAXN = 510;
const int MAXM = 510;
const int MAXK = 510;

int N, M, K;
char G[MAXN][MAXM];
int closest[MAXN][MAXM][26];
string S;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M >> K;
	for (int i = 0; i < N; i++) {
		cin >> G[i];
	}
	cin >> S;
	for (int j = 0; j < M; j++) {
		for (int i = 0; i < N; i++) {
			for (int v = 0; v < 26; v++) {
				closest[i][j][v] = N+1;
			}
			closest[i][j][G[i][j] - 'a'] = 0;
		}
		for (int z = 0; z < 2; z++) {
			for (int i = 0; i < N; i++) {
				for (int v = 0; v < 26; v++) {
					setmin(closest[(i+1)%N][j][v], closest[i][j][v] + 1);
				}
			}
		}
	}

	ll ans = -1;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j + K <= M; j++) {
			ll cnd = 0;
			for (int a = 0; a < K; a++) {
				int v = closest[i][j+a][S[a] - 'a'];
				if (v > N) goto isBad;
				cnd += v;
			}
			if (ans == -1 || cnd < ans) {
				ans = cnd;
			}
isBad:;
		}
	}

	cout << ans << '\n';
	return 0;
}
