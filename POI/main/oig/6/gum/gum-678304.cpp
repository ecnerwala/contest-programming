#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e4;
int N;
vector<char> S[MAXN];
int cnt[MAXN][30];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for(int i = 0; i < N; i++) {
		string s;
		cin >> s;
		S[i] = vector<char>(s.rbegin(), s.rend());
		for(int j = 0; j < int(S[i].size()); j++) {
			cnt[i][S[i][j] - 'a'] ++;
		}
	}
	bool good = false;
	for(int v = 25; v >= 0; v--) {
		if(!good && v == 2) {
			cout << "bitek\n";
			return 0;
		}
		int c = cnt[0][v];
		for(int j = 1; j < N; j++) {
			c = min(c, cnt[j][v]);
		}
		if(c) {
			good = true;
			for(int j = 0; j < c; j++) {
				cout << char('a' + v);
			}
			for(int j = 0; j < N; j++) {
				for(int k = c; k; ) {
					int w = S[j].back() - 'a';
					S[j].pop_back();
					cnt[j][w] --;
					if(w == v) k--;
				}
			}
		}
	}
	return 0;
}
