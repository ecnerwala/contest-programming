#include<bits/stdc++.h>
using namespace std;

int mp[300];
int main() {
	ios_base::sync_with_stdio(0);
	int N; cin >> N;
	string S;
	getline(cin, S);
	memset(mp, 0, sizeof(mp));
	for (int i = 0, nlines = 0; i < N; i++) {
		if (nlines < N) {
			getline(cin, S);
			nlines ++;
			while (S == "" && nlines < N) {
				getline(cin, S);
				nlines ++;
			}
		}
		for (int j = 0; j < int(S.size()); j++) {
			char c = S[j];
			if (c == ' ') continue;
			assert(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'));
			mp[c] ++;
		}
	}
	for (char c = 'a'; c <= 'z'; c++) {
		if (mp[c]) cout << c << ' ' << mp[c] << '\n';
	}
	for (char c = 'A'; c <= 'Z'; c++) {
		if (mp[c]) cout << c << ' ' << mp[c] << '\n';
	}

	return 0;
}
