#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	vector<int> ans(26, 100);
	for (int i = 0; i < N; i++) {
		string s; cin >> s;
		vector<int> cnts(26);
		for (char c : s) cnts[c-'a']++;
		for (int v = 0; v < 26; v++) ans[v] = min(ans[v], cnts[v]);
	}
	for (int v = 0; v < 26; v++) {
		while (ans[v] --> 0) cout << char(v+'a');
	}
	cout << '\n';

	return 0;
}
