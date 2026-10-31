#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	string winner;
	int best_val;
	cin >> winner >> best_val;
	N--;
	while (N--) {
		string cur; cin >> cur;
		int val; cin >> val;
		if (val > best_val) {
			winner = std::move(cur);
			best_val = val;
		}
	}
	cout << winner << '\n';

	return 0;
}
