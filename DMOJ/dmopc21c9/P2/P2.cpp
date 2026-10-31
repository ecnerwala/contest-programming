#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		string A, B; cin >> A >> B;
		cout << ([&]() -> bool {
			int j = 0;
			std::vector<char> stk; stk.reserve(A.size());
			for (int i = 0; i < int(A.size()); i++) {
				char c = A[i];
				while (!stk.empty() && stk.back() == c) {
					stk.pop_back();
					if (c == 'z') return false;
					c++;
				}
				if (stk.empty() && j < int(B.size()) && c == B[j]) {
					j++;
				} else {
					stk.push_back(c);
				}
			}
			return stk.empty() && j == int(B.size());
		}() ? "YES" : "NO") << '\n';
	}
	return 0;
}
