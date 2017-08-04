#include<bits/stdc++.h>
using namespace std;

string go(string s) {
	if (s.length() % 2 == 1) {
		return "AMBIGUOUS";
	}
	int N = int(s.size());
	string r(N, ' ');
	r[1] = s[0];
	for (int i = 2; i < int(s.length()); i+=2) {
		r[i+1] = 'A' + ((s[i] - 'A') - (r[i-1] - 'A') + 26) % 26;
	}
	r[N-2] = s[N-1];
	for (int i = N-3; i >= 0; i-=2) {
		r[i-1] = 'A' + ((s[i] - 'A') - (r[i+1] - 'A') + 26) % 26;
	}
	return r;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		string s; cin >> s;
		string r = go(s);
		cout << "Case #" << case_num << ": " << r << '\n';
	}

	return 0;
}
