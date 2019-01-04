#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e6;
const int MAXK = 2e6;

int N, K;
int C[MAXN];

pair<int, int> ranges[MAXK];

const int MAXL = 2e6;

int L, L2;
int X[MAXL];
int P[MAXK]; // maps from X to P;
int dp[MAXL];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> K;
	for(int i = 0; i < N; i++) {
		cin >> C[i], C[i] --;
	}

	cin >> L >> L2;

	for(int i = 0; i < K; i++) {
		ranges[i] = make_pair(N, -1);
	}

	for(int t = 0; t < 2; t++) {

		for(int i = 0; i < K; i++) {
			P[i] = -1;
		}

		for(int i = 0; i < L; i++) {
			dp[i] = N;
		}

		for(int i = 0; i < L; i++) {
			cin >> X[i]; X[i] --;
			P[X[i]] = i;
		}


		for(int i = N - 1; i >= 0; i--) {
			int c = C[i];

			if(t == 0) ranges[c].first = dp[0];
			else ranges[c].second = N - 1 - dp[0];

			int v = P[c];

			if(v == L - 1) {
				dp[v] = i;
			} else {
				dp[v] = dp[v + 1];
			}
		}

		reverse(C, C + N);
		L = L2;
	}

	sort(ranges, ranges + K);

	vector<int> res;

	for(int i = 0, j = 0; i < N && j < K; ) {
		if(ranges[j].second < i) {
			j++;
		} else {
			if(ranges[j].first <= i && C[i] == X[L - 1]) {
				res.push_back(i);
			}
			i++;
		}
	}

	cout << res.size() << '\n';
	if(res.size()) cout << res[0] + 1;
	for(int i = 1; i < res.size(); i++) {
		cout << ' ' << res[i] + 1;
	}
	cout << '\n';

	return 0;
}
