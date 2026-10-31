#include <bits/stdc++.h>

std::vector<int> normalize(int N, std::string S) {
	std::vector<int> res; res.reserve(2*N+1);
	res.push_back(0);
	int prv = 0;
	for (int i = 0; i <= N; i++) {
		int cur = i < N ? (int(S[i] - 'A') - i % 3 + 3) % 3 : 0;
		for (; prv != cur; prv++, prv %= 3) {
			res.push_back(res.back() + (i - res.back()) % 3);
			if (res.size() >= 4 && res.end()[-3] == res.end()[-1]) {
				res.pop_back();
				res.pop_back();
				res.pop_back();
			}
		}
	}
	return res;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N;
	cin >> N;
	string S, T; cin >> S >> T;
	cout << (normalize(N, S) == normalize(N, T) ? "YES" : "NO") << '\n';

	return 0;
}
