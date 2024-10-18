#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	auto query = [&](int64_t h) -> bool {
		cout << h << '\n' << flush;
		std::string resp; cin >> resp;
		if (resp == "wet") return false;
		else if (resp == "ok") return true;
		else if (resp == "fail") exit(0);
		else assert(false);
	};

	int64_t lo = 0;
	int64_t hi = 1;
	while (!query(hi)) {
		lo = hi;
		hi *= 2;
	}
	while (hi - lo > 1) {
		int64_t md = lo + (hi - lo) / 2;
		if (query(md)) hi = md;
		else lo = md;
	}
	cout << hi << '\n';

	return 0;
}
