#include <bits/stdc++.h>
using namespace std;

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

using ll = long long;
const ll INF = 1e18;

const int MAXN = 5100;
int N;
ll X, Y, Z;
int S[MAXN];
int maxMatch[MAXN][MAXN];

ll dp[MAXN];
ll dpCopy[MAXN][MAXN];

pair<int, int> dq[MAXN];

ll go() {
	cin >> N >> X >> Y >> Z;
	for (int i = 0; i < N; i++) {
		cin >> S[i];
	}

	for (int i = 0; i <= N; i++) {
		maxMatch[i][N] = maxMatch[N][i] = 0;
	}
	for (int i = N-1; i >= 0; i--) {
		for (int j = N-1; j >= 0; j--) {
			maxMatch[i][j] = (S[i] == S[j]) ? (1 + maxMatch[i+1][j+1]) : 0;
		}
	}

	for (int i = 0; i <= N; i++) {
		dp[i] = X * i;
	}
	for (int i = 0; i <= N; i++) {
		for (int j = i+1; j <= N; j++) {
			dpCopy[i][j] = X * j + Y; // fake copy, but it's a fine default
		}
	}

	for (int i = 0; i <= N; i++) {
		setmin(dp[i], dp[i-1] + X);

		int maxCopy = 0;
		for (int j = 0; j < i; j++) {
			maxCopy = max(maxCopy, min(i-j, maxMatch[i][j]));
		}
		for (int j = i+1; j <= i+maxCopy; j++) {
			setmin(dpCopy[i][j], dp[i] + Y + Z);
		}

		int head = N, tail = N-1;
		int curSucc = N+1;
		for (int j = N; j > i; j--) {
			int insertTime = min(j-i, maxMatch[i][j]) + i;
			while (head <= tail && dq[head].second <= insertTime) {
				head++;
			}
			dq[--head] = {j, insertTime};
			while (head <= tail && j <= dq[tail].second) {
				curSucc = dq[tail--].first;
			}
			if (curSucc <= N) {
				setmin(dpCopy[curSucc][curSucc+(j-i)], dpCopy[i][j] + X * (curSucc - j) + Z);
			}
		}
		for (int j = i+1; j <= N; j++) {
			setmin(dp[j], dpCopy[i][j]);
		}
	}
	return dp[N];
}

int main(){
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		ll ans = go();
		cout << "Case #" << t << ": " << ans << '\n';
	}
}
