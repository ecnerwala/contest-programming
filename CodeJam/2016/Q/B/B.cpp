#include<bits/stdc++.h>
using namespace std;

int main() {

	ios_base::sync_with_stdio(0);
	int T; cin >> T;
	for(int case_num = 1; case_num <= T; case_num++) {
		string S; cin >> S;
		S += '+';
		int res = 0;
		for(int i = 0; i + 1 < int(S.size()); i++) {
			res += (S[i] != S[i + 1]);
		}
		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
