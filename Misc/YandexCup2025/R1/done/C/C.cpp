#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; int64_t K; cin >> N >> K;
	// we want to factorize K into fibonacci numbers
	std::vector<int64_t> fib;
	fib.push_back(0);
	fib.push_back(1);
	fib.push_back(1);
	while (fib.back() <= int64_t(1e18)) {
		fib.push_back(fib.end()[-2] + fib.end()[-1]);
	}
	std::vector<int> pieces;
	[&](this auto&& self, int64_t val, int cur_tot, int idx) -> bool {
		if (cur_tot > N+1) return false;
		if (idx == 0) {
			return val == 1;
		}
		if (val % fib[idx] == 0) {
			if (self(val / fib[idx], cur_tot + idx + 1, idx)) {
				return true;
			}
		}
		if (self(val, cur_tot, idx - 1)) {
			return true;
		}
		return false;
	}(K, 0, int(fib.size()) - 1);

	return 0;
}
