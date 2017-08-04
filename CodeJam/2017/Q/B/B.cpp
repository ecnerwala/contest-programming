#include<bits/stdc++.h>
using namespace std;

bool isGood(string s) {
	if (s == "0") return true;
	if (s == "") return false;
	if(s[0] == '0') return false;
	for(int i = 1; i < int(s.size()); i++) {
		if (s[i-1] > s[i]) return false;
	}
	return true;
}

string go(string S) {
	for (int i = 1; i < int(S.size()); i++) {
		if (S[i - 1] > S[i]) {
			while (i > 1 && S[i-2] == S[i-1]) i--;
			assert(S[i-1] > '0');
			S[i-1] --;
			for (int j = i; j < int(S.size()); j++) {
				S[j] = '9';
			}
			break;
		}
	}
	while (S.size() >= 2 && S[0] == '0') {
		S = S.substr(1);
	}
	assert(isGood(S));
	return S;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		string S; cin >> S;
		cout << "Case #" << case_num << ": ";
		cout << go(S) << '\n';
	}

	return 0;
}
