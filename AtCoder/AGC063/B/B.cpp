#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;
	std::vector<int> stk; stk.reserve(N);
	int64_t ans = 0;
	for (int a : A) {
		if (a == 1) {
			stk.push_back(1);
		} else {
			while (!stk.empty() && stk.back() != a-1) {
				stk.pop_back();
			}
			if (!stk.empty()) {
				stk.back() = a;
			}
		}
		ans += int(stk.size());
	}
	cout << ans << '\n';

	return 0;
}
