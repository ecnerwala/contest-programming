#include<bits/stdc++.h>
using namespace std;

int cnt[200];

int main() {
	if(fopen("concatenation.in", "r")) {
		freopen("concatenation.in", "r", stdin);
		freopen("concatenation.out", "w", stdout);
	}
	ios_base::sync_with_stdio(0);

	string s, t; cin >> s >> t;
	long long res = s.size();
	res *= t.size();
	for(int i = 1; i < int(s.size()); i++) {
		cnt[s[i]] ++;
	}
	for(int i = 0; i + 1 < int(t.size()); i++) {
		res -= cnt[t[i]];
	}
	cout << res << '\n';
}
