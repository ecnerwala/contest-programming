#include<bits/stdc++.h>
using namespace std;

template <typename T> void setmax(T& a, const T& b) { if (b > a) a = b; }
template <typename T> void setmin(T& a, const T& b) { if (b < a) a = b; }

const int MAXN = 4.1e4;
int N;

const int MAXK = 9;
int K;

int maxLeft[1<<9];

int adj[9];
int maxLeftLen[1<<9][9+1];

int ans[9+1];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> K;
	long long tot = 0;
	memset(maxLeft, 0, sizeof(maxLeft));
	for (int z = 1; z < N; z++) {

		for (int j = 0; j < K; j++) adj[j] = 0;

		for (int i = 0; i < K; i++) {
			string s; cin >> s;
			for (int j = 0; j < K; j++) {
				if (s[j] == '1') adj[j] |= (1<<i);
			}
		}

		memset(maxLeftLen, -1, sizeof(maxLeftLen));
		for (int m = 0; m < (1<<K); m++) {
			maxLeftLen[m][__builtin_popcount(m)] = maxLeft[m];
		}

		for (int i = 0; i < K; i++) {
			for (int m = 0; m < (1<<K); m++) {
				if (m & (1<<i)) continue;
				for (int l = 0; l <= K; l++) {
					setmax(maxLeftLen[m|(1<<i)][l], maxLeftLen[m][l]);
				}
			}
		}

		for (int m = 0; m < (1<<K); m++) {
			int m2 = 0;
			for (int j = 0; j < K; j++) {
				if (m & (1<<j)) m2 |= adj[j];
			}
			maxLeft[m] = maxLeftLen[m2][__builtin_popcount(m)] + 1;
		}

		for (int i = 0; i < K; i++) {
			for (int m = 0; m < (1<<K); m++) {
				if (m & (1<<i)) continue;
				setmin(maxLeft[m|(1<<i)], maxLeft[m]);
			}
		}
		memset(ans, 0, sizeof(ans));
		for (int m = 0; m < (1<<K); m++) {
			setmax(ans[__builtin_popcount(m)], maxLeft[m]);
		}
		for (int l = 1; l <= K; l++) {
			tot += ans[l];
		}
	}
	cout << tot << '\n';

	return 0;
}
