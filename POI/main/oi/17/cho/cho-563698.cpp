#include<iostream>
#include<string>
#include<cstring>
#include<cassert>

using namespace std;

typedef long long ll;

const ll INF = 1e15;

const int MAXN = 230;
const int MAXS = 2e5;
const int MAXL = 32;

int N, M;

string S[MAXN];
int F[MAXS];

ll cost[MAXN][MAXN];
ll dp[MAXN][MAXN];
ll tmp[MAXN][MAXN];

void buildKMP(const string &w) {
	F[0] = -1;
	F[1] = 0;
	for(int pos = 2, cnd = 0; pos <= w.size(); ) {
		if(w[pos - 1] == w[cnd]) {
			F[pos] = cnd + 1;
			cnd++, pos++;
		} else if(cnd == 0) {
			F[pos] = 0;
			pos++;
		} else {
			cnd = F[cnd];
		}
	}
}

//returns the number of word characters left over
int search(const string &s, const string &w) {
	int i = 1;
	int j = 0;
	while(i < s.size()) {
		if(s[i] == w[j]) {
			i++, j++;
		} else if (j == 0) {
			i++;
		} else {
			j = F[j];
		}
	}
	return w.size() - j;
}


int main() {
	cin >> N >> M;
	for(int i = 0; i < N; i++) {
		cin >> S[i];
	}
	for(int i = 0; i < N; i++) {
		buildKMP(S[i]);
		for(int j = 0; j < N; j++) {
			cost[j][i] = search(S[j], S[i]);
		}
	}
	//for(int i = 0; i < N; i++) {
		//for(int j = 0; j < N; j++) {
			//cerr << cost[i][j] << ' ';
		//}
		//cerr << '\n';
	//}
	for(int i = 0; i < N; i++) for(int j = 0; j < N; j++) dp[i][j] = (i == j) ? 0 : INF;
	
	M--;
	while(M) {
		if(M % 2) {
			for(int i = 0; i < N; i++) {
				for(int j = 0; j < N; j++) {
					ll v = INF;
					for(int k = 0; k < N; k++) {
						ll cnd = cost[i][k] + dp[k][j];
						if(cnd < v) v = cnd;
					}
					tmp[i][j] = v;
				}
			}
			memcpy(dp, tmp, sizeof(tmp));
		}
		for(int i = 0; i <= N; i++) {
			for(int j = 0; j < N; j++) {
				ll v = INF;
				for(int k = 0; k < N; k++) {
					ll cnd = cost[i][k] + cost[k][j];
					if(cnd < v)  v = cnd;
				}
				tmp[i][j] = v;
			}
		}
		memcpy(cost, tmp, sizeof(tmp));
		M /= 2;
	}

	ll res = INF;
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			ll cnd = S[i].size() + dp[i][j];
			if(cnd < res) res = cnd;
		}
	}
	cout << res << '\n';
	return 0;
}
