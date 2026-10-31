#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		std::vector<std::string> S(N);
		for (auto& s : S) { assert(!s.empty()); cin >> s; }

		cout << "Case #" << case_num << ": ";
	}

	return 0;
}
