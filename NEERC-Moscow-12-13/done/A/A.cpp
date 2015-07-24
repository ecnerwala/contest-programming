#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e4;
const int MAXK = 10;

const int MAXM = 2e5;

int K;

int N;
pair<int, int> S[MAXN];

int M;
pair<pair<int, int>, int> Q[MAXM];

int res[MAXM];

int dp[1 << (MAXK * 2)];

// apply the mask
int apply(int traits, int mask) {
	return (mask << K) | (traits & mask);
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> K;
	for(int i = 0; i < N; i++) {
		cin >> S[i].first;
		int Y; cin >> Y;
		for(int j = 0; j < Y; j++) {
			int f; cin >> f; f--;
			S[i].second |= (1 << f);
		}
	}

	cin >> M;
	for(int i = 0; i < M; i++) {
		int a; cin >> a; a--;
		int T; cin >> T;
		int mask = 0;
		for(int j = 0; j < T; j++) {
			int h; cin >> h; h--;
			mask |= (1 << h);
		}

		Q[i].first = make_pair(S[a].first, apply(S[a].second, mask));
		Q[i].second = i;
	}

	sort(S, S + N);
	reverse(S, S + N);
	sort(Q, Q + M);
	reverse(Q, Q + M);

	for(int i = 0, f = 0; i < M; i++) {
		while(S[f].first > Q[i].first.first) {
			for(int m = 0; m < (1 << K); m++) {
				dp[apply(S[f].second, m)] ++;
			}
			f++;
		}
		res[Q[i].second] = dp[Q[i].first.second] + 1;
	}

	for(int i = 0; i < M; i++) {
		cout << res[i] << '\n';
	}

	return 0;
}
