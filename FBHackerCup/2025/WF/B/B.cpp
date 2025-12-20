#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::string BASELINE;
	{
		std::ifstream base("ProblemText.txt");
		std::getline(base, BASELINE);
	}
	cerr << '"' << BASELINE.substr(BASELINE.size() - 10) << '"' << '\n';

	int T; cin >> T;
	std::vector<std::string> Ss(T);
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		cin.ignore(1, '\n');
		std::string S; std::getline(cin, S);
		cerr << "Case #" << case_num << " ";
		cerr << "S = \"" << S << '"' << '\n';
		assert(int(S.size()) == N);

		Ss[case_num-1] = S;
	}

	for (auto S : Ss) {
	}

	std::vector<std::string> answers(T);
	int num_skip = 0;
	for (int case_num = 1; case_num <= T; case_num ++) {
		auto ans = answers[case_num-1];
		if (ans.empty()) {
			ans = "SKIP";
			num_skip++;
		} else {
			assert(int(ans.size()) == 2025);
		}
		cout << "Case #" << case_num << ": " << ans << '\n';
	}
	cerr << "tot skip " << num_skip << '\n';

	return 0;
}
