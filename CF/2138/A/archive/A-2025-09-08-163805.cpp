#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int K; int64_t X; cin >> K >> X;
		int64_t Y = (int64_t(1) << (K+1)) - X;
		std::vector<int> ops; ops.reserve(K+1);
		while (true) {
			if (X > Y) {
				ops.push_back(2);
				X -= Y;
				Y *= 2;
			} else if (Y > X) {
				ops.push_back(1);
				Y -= X;
				X *= 2;
			} else break;
		}
		std::ranges::reverse(ops);
		cout << ops.size() << '\n';
		for (int i = 0; i < int(ops.size()); i++) {
			cout << ops[i] << " \n"[i+1==int(ops.size())];
		}
	}

	return 0;
}
