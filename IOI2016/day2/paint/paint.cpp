#include "paint.h"
#include <bits/stdc++.h>
using namespace std;

struct puzzle {
	static const int MAXN = 3e5;
	int N;
	string S;

	static const int MAXK = 200;
	int K;
	vector<int> C;

	int cnt_has[MAXN];

	bool dp[MAXN][MAXK];

	bool can_open[MAXN];
	int can_has[MAXN];

	string go() {
		N++; S += '_';

		cnt_has[0] = 0;
		for(int i = 0; i < N; i++) {
			cnt_has[i+1] = cnt_has[i] + bool(S[i] != '_');
		}

		memset(dp, 0, sizeof(dp));

		dp[0][0] = true;
		for(int i = 0; i < N; i++) {
			for(int k = 0; k <= K; k++) {
				if(!dp[i][k]) continue;
				if(S[i] != 'X') {
					dp[i+1][k] = true;
				}
				if(k < K && i+C[k] < N &&
						cnt_has[i+C[k]] - cnt_has[i] == C[k] &&
						S[i+C[k]] != 'X') {
					dp[i+C[k]+1][k+1] = true;
				}
			}
		}
		assert(dp[N][K]);

		for(int k = 0; k < K; k++) dp[N][k] = false;

		for(int i = N-1; i >= 0; i--) {
			for(int k = 0; k <= K; k++) {
				if(!dp[i][k]) continue;
				bool is_good = false;
				if(S[i] != 'X') {
					if(dp[i+1][k]) {
						is_good = true;
						can_open[i] = true;
					}
				}
				if(k < K && i+C[k] < N &&
						cnt_has[i+C[k]] - cnt_has[i] == C[k] &&
						S[i+C[k]] != 'X') {
					if(dp[i+C[k]+1][k+1]) {
						is_good = true;
						can_has[i] ++;
						can_has[i+C[k]] --;
						can_open[i+C[k]] = true;
					}
				}
				if(!is_good) dp[i][k] = false;
			}
		}

		for(int i = 0; i < N; i++) {
			can_has[i + 1] += can_has[i];
		}
		assert(can_has[N] == 0);

		string res(N, '0');
		for(int i = 0; i < N; i++) {
			if(can_open[i] && can_has[i]) {
				res[i] = '?';
			} else if(can_open[i]) {
				res[i] = '_';
			} else if(can_has[i]) {
				res[i] = 'X';
			} else assert(false);
		}
		res.pop_back();
		return res;
	}

	puzzle(string s, vector<int> c)
		: N(int(s.size())), S(s), K(int(c.size())), C(c) { }
};


std::string solve_puzzle(std::string s, std::vector<int> c) {
	return puzzle(s, c).go();
}
