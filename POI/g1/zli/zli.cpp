#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0);
	int N; cin >> N;
	string S;
	getline(cin, S);
	map<char, int> mp;
	for (int i = 0; i < N; i++) {
		getline(cin, S);
		for (int j = 0; j < int(S.size()); j++) {
			char c = S[j];
			if (c == ' ') continue;
			mp[c ^ 32] ++;
		}
	}
	for (map<char, int>::iterator it = mp.begin(); it != mp.end(); it++) {
		cout << char(it->first ^ 32) << ' ' << it->second << '\n';
	}

	return 0;
}
