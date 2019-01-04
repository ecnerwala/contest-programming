#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e5;
int N;
string S[MAXN];
int cnt[MAXN][30];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> S[i];
		reverse(S[i].begin(), S[i].end());
		for (char c : S[i]) {
			cnt[i][c-'a']++;
		}
	}
	string res;
	for (char c = 'z'; c >= 'a'; c--) {
		int n = cnt[0][c-'a'];
		for (int i = 1; i < N; i++) {
			n = min(n, cnt[i][c-'a']);
		}
		for (int k = 0; k < n; k++) {
			res += c;
		}
		for (int i = 0; i < N; i++) {
			for (int k = n; k; ) {
				cnt[i][S[i].back()-'a']--;
				k-=(S[i].back()==c);
				S[i].pop_back();
			}
		}
	}
	if (res.empty()) res = "-1";
	cout << res << '\n';

	return 0;
}
