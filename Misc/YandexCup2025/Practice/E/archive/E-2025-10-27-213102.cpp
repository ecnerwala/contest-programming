#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	auto query = [&](int64_t v) -> bool {
		if (v >= int64_t(1e18)) return true;
		cout << v << '\n' << flush;
		std::string res; cin >> res;
		if (res == "fail") exit(0);
		else if (res == "ok") return true;
		else if (res == "wet") return false;
		else assert(false);
	};
	int64_t n_min = 0;
	int64_t n_max;
	// M >= 2 * n_min + 1
	while (true) {
		int64_t q = 2 * n_min + 1;
		if (query(q)) {
			n_max = q;
			break;
		} else {
			n_min = q;
			continue;
		}
	}
	while (n_max - n_min > 1) {
		int64_t q = n_min + (n_max - n_min) / 2;
		if (query(q)) n_max = q;
		else n_min = q;
	}
	cout << '!' << ' ' << n_max << '\n' << flush;

	return 0;
}
