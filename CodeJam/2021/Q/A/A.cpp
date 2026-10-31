#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		vector<int> L(N); for (auto& i : L) cin >> i;

		int ans = 0;
		for (auto it = L.begin(); it+1 != L.end(); it++) {
			auto jt = min_element(it, L.end()) + 1;
			ans += int(jt - it);
			std::reverse(it, jt);
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
