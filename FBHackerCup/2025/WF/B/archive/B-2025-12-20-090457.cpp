#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		cin.ignore(1, '\n');
		std::string S; std::getline(cin, S);
		cerr << "Case #" << case_num << " ";
		cerr << "S = \"" << S << '"' << '\n';
		assert(int(S.size()) == N);

		std::string ans = "SKIP";
		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
