#include<bits/stdc++.h>
using namespace std;

const int INF = 2.1e9;

const int MAXM = 2e6;
int M;

const int MAXN = 3e3;
int N;

int L[MAXN];

typedef pair<int, int> pii;
#define A first
#define B second

// length, value
vector<pii> dp[MAXN];

int main() {
	scanf("%d %d", &M, &N);
	for(int i = 1; i <= N; i++) {
		scanf("%d", &L[i]);
		L[i] += L[i - 1];
	}

	for(int i = 1; i <= N; i++) {
		for(int j = i - 1; j >= 0; j--) {
			int len = L[i] - L[j] + (i - j - 1);
			if(len > M) break;
			int val = INF;
			if(j) {
				vector<pii>::iterator it = lower_bound(dp[j].begin(), dp[j].end(), pii(len, -1));
				if(it != dp[j].end()) {
					assert(it->first >= len);
					val = min(val, it->second + (it->first - len));
				}
				if(it != dp[j].begin()) {
					it--;
					assert(it->first <= len);
					val = min(val, it->second + (len - it->first));
				}
			} else {
				val = 0;
			}
			bool good = true;
			//cerr << i << ' ' << j << ' ' << len << ' ' << val << '\n';
			while(!dp[i].empty()) {
				assert(dp[i].back().first < len);
				if((len - dp[i].back().first) + dp[i].back().second <= val) {
					good = false;
					break;
				} else if((len - dp[i].back().first) + val <= dp[i].back().second) {
					dp[i].pop_back();
				} else {
					break;
				}
			}
			if(good) dp[i].push_back(pii(len, val));
		}
	}
	int res = INF;
	for(vector<pii>::iterator it = dp[N].begin(); it != dp[N].end(); it++) {
		res = min(res, it->second);
	}
	printf("%d\n", res);
	return 0;
}
