#include<bits/stdc++.h>
using namespace std;

const int MAXN = 501;
using bs = bitset<MAXN>;
int N;
bs cnt[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		string s; cin >> s;
		for (int j = 0; j < N; j++) {
			cnt[i][j] = (s[j] - '0') & 1;
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j <= i; j++) cout << 0;
		for (int j = i+1; j < N; j++) {
			cout << int(cnt[i][j]);
			if (cnt[i][j]) {
				cnt[i] ^= cnt[j];
			}
		}
		cout << '\n';
	}

	return 0;
}
