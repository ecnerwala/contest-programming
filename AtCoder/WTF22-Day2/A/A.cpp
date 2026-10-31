#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::string S; cin >> S;
		std::string R(N, '\0');
		int cnt = 0;
		std::set<int> cuts;
		cuts.insert(0);
		std::vector<int> stk; stk.reserve(N+1);
		stk.push_back(0);
		std::string cur_string; cur_string.reserve(N+1);
		for (int i = N-1; i >= 0; i--) {
			if (S[i] == 'R') cnt++;
			else if (S[i] == 'B') cnt--;
			else assert(false);
			if (cuts.count(cnt)) {
				R[i] = '0';
				while (stk.back() != cnt) {
					cuts.erase(stk.back());
					stk.pop_back();
				}
				cur_string.clear();
			} else {
				R[i] = '1';
				if (!cur_string.empty() && cur_string.back() != S[i]) {
					stk.push_back(cnt);
					cuts.insert(cnt);
					cur_string.clear();
				} else {
					cur_string += S[i];
				}
			}
		}
		cout << R << '\n';
	}

	return 0;
}
