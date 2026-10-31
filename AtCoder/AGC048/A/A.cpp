#include<bits/stdc++.h>

int solve(std::string S) {
	using namespace std;
	assert(!S.empty());
	if ("atcoder" < S) {
		return 0;
	}
	assert("atcoder" >= S);
	assert(S[0] == 'a');
	for (int i = 1; i < int(S.size()); i++) {
		if (S[i] != 'a') {
			if (i == 1) return 1;
			else {
				assert(S[1] == 'a');
				if (S[i] > 't') {
					return i-1;
				} else {
					return i;
				}
			}
		}
	}
	return -1;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		string S; cin >> S;
		cout << solve(S) << '\n';
	}

	return 0;
}
