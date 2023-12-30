#include <bits/stdc++.h>

std::vector<std::string> solve(int N) {
	if (N == 1) return {"1"};
	assert(N >= 3);
	std::vector<std::string> res; res.reserve(N);
	res.push_back("196" + std::string(N - 3, '0'));
	for (int i = 0; i <= (N-3)/2; i++) {
		res.push_back("1" + std::string(i, '0') + "6" + std::string(i, '0') + "9" + std::string(N - 3 - 2*i, '0'));
		res.push_back("9" + std::string(i, '0') + "6" + std::string(i, '0') + "1" + std::string(N - 3 - 2*i, '0'));
	}
	return res;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		auto res = solve(N);
		for (const auto& s : res) cout << s << '\n';
	}

	return 0;
}
