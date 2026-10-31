#include<bits/stdc++.h>
using namespace std;

using ll = long long;
const int INF = int(1e9) + 200;

template <typename T> void setmin(T& a, const T& b) {
	if (b < a) a = b;
}
template <typename T> void setmax(T& a, const T& b) {
	if (b > a) a = b;
}

const int MAXN = 8100;
const int MAXM = 8100;
int N, M;
int dp[MAXN][MAXN];
int mx[MAXN][MAXN];

string go() {
	cin >> N >> M;
	vector<int> F(N);
	for (int i = 0; i < N; i++) {
		cin >> F[i];
	}
	vector<int> H(N-1);
	for (int i = 0; i < N-1; i++) {
		cin >> H[i];
	}
	vector<int> prefF(F.size()+1);
	for (int i = 0; i < N; i++) {
		prefF[i+1] = prefF[i] + F[i];
	}
	vector<int> prefH(H.size()+1);
	for (int i = 0; i < N-1; i++) {
		prefH[i+1] = prefH[i] + H[i];
	}

	memset(mx, -1, sizeof(mx));

	for (int i = 0; i < N-1; i++) {
		mx[i][i+1] = H[i];
	}
	for (int len = 3; len <= N; len++) {
		for (int i = 0, j = len-1; j < N; i++, j++) {
			mx[i][j] = max(mx[i+1][j], mx[i][j-1]);
		}
	}
	dp[0][N-1] = dp[N-1][0] = 0;
	for (int len = N-1; len >= 1; len--) {
		for (int i = 0, j = len-1; j < N; i++, j++) {
			dp[i][j] = dp[j][i] = INF;
			int curHave = prefF[j+1] - prefF[i];
			int curNeed = prefH[j] - prefH[i];
			if (i > 0) {
				// extend left by one
				setmin(dp[i][j], max(dp[i-1][j], curNeed + H[i-1] - curHave));
			}
			if (j+1 < N) {
				setmin(dp[j][i], max(dp[j+1][i], curNeed + H[j] - curHave));
			}
			if (len > 1) {
				assert(mx[i][j] != -1);
				setmin(dp[i][j], max(dp[j][i], curNeed - curHave + mx[i][j]));
				setmin(dp[j][i], max(dp[i][j], curNeed - curHave + mx[i][j]));
			}

			//cerr << i << ' ' << j << ' ' << dp[i][j] << '\n';
			//cerr << j << ' ' << i << ' ' << dp[j][i] << '\n';
		}
	}

	vector<int> minHeight(N);
	for (int i = 0; i < N; i++) {
		minHeight[i] = dp[i][i];
	}

	{
		string res;
		while (M--) {
			int X, Y; cin >> X >> Y; X--;
			res += (Y >= minHeight[X] ? 'Y' : 'N');
		}
		return res;
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		cerr << "Case #" << t << '\n';
		string ans = go();
		cout << "Case #" << t << ": " << ans << '\n';
	}

	return 0;
}
