#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> ans(2*N);
		std::vector<int> cur; cur.reserve(N+1);
		std::vector<int> oth; oth.reserve(N+1);
		for (int i = 0; i < 2*N; i++) {
			cur.push_back(i);
			cout << '?' << ' ' << cur.size();
			for (int v : cur) cout << ' ' << v+1;
			cout << '\n' << flush;
			int res; cin >> res;
			if (res != 0) {
				ans[i] = res;
				cur.pop_back();
				oth.push_back(i);
			}
		}
		assert(int(cur.size()) == N);
		for (int i : cur) {
			oth.push_back(i);
			cout << '?' << ' ' << oth.size();
			for (int v : oth) cout << ' ' << v+1;
			cout << '\n' << flush;
			int res; cin >> res;
			ans[i] = res;
			assert(res != 0);
			oth.pop_back();
		}
		cout << '!';
		for (int i = 0; i < 2*N; i++) cout << ' ' << ans[i];
		cout << '\n' << flush;
	}

	return 0;
}
