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
	for (int64_t v : fib) cerr << v << '\n';

	return 0;
}
