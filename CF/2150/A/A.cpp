#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::string S; cin >> S; assert(int(S.size()) == N);
		std::vector<int> A(M); for (auto& a : A) cin >> a;
		std::set<int> black(A.begin(), A.end());
		int cur = 1;
		for (char c : S) {
			if (c == 'A') {
				cur++;
				black.insert(cur);
			} else if (c == 'B') {
				cur++;
				while (black.count(cur)) cur++;
				black.insert(cur);
				while (black.count(cur)) cur++;
			} else assert(false);
		}
		cout << int(black.size()) << '\n';
		for (auto it = black.begin(); it != black.end(); ++it) {
			cout << *it << " \n"[std::next(it) == black.end()];
		}
	}

	return 0;
}
