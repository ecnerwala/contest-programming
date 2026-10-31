#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		vector<string> S(N);
		for (int i = 0; i < N; i++) {
			cin >> S[i];
		}

		auto go = [&]() -> string {
			string pref;
			string suff;
			string mid;
			for (int i = 0; i < N; i++) {
				int first = int(S[i].find_first_of('*'));
				assert(first != int(string::npos));
				int last = int(S[i].find_last_of('*'));
				assert(last != int(string::npos));
				for (int z = 0; z < int(pref.size()) && z < first; z++) {
					if (pref[z] != S[i][z]) {
						return "*";
					}
				}
				for (int z = int(pref.size()); z < first; z++) {
					pref += S[i][z];
				}
				for (int z = 0; z < int(suff.size()) && int(S[i].size())-1-z > last; z++) {
					if (suff[z] != S[i][int(S[i].size())-1-z]) {
						return "*";
					}
				}
				for (int z = int(suff.size()); int(S[i].size())-1-z > last; z++) {
					suff += S[i][int(S[i].size())-1-z];
				}
				for (int z = first; z <= last; z++) {
					if (S[i][z] != '*') mid += S[i][z];
				}
			}
			reverse(suff.begin(), suff.end());
			return pref + mid + suff;
		};
		string ans = go();

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
