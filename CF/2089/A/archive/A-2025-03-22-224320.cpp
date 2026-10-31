#include <bits/stdc++.h>

std::vector<int> solve(int N) {
	using std::cout;
	using std::cerr;
	cerr << "solve " <<  N << '\n';
	std::vector<int> primes; primes.reserve(N);
	std::vector<int> pfac(N+1);
	for (int i = 2; i <= N; i++) {
		if (!pfac[i]) {
			primes.push_back(i);
			pfac[i] = i;
		}
		for (int p : primes) {
			if (int64_t(p) * int64_t(i) > N) break;
			pfac[i * p] = p;
			if (p == pfac[i]) break;
		}
	}

	int p = N/2 + 1;
	while (pfac[p] != p) p++;
	assert(p <= N);

	std::vector<int> res; res.reserve(N);
	res.push_back(p);
	for (int i = p+1; i <= N; i++) {
		assert(2 * p - i >= 1);
		res.push_back(2 * p - i);
		res.push_back(i);
	}
	for (int i = 2 * p - N - 1; i >= 1; i--) {
		res.push_back(i);
	}
	assert(int(res.size()) == N);
	int64_t s = 0;
	int cnt = 0;
	for (int i = 1; i <= N; i++) {
		s += res[i-1];
		int a = int((s + i-1) / i);
		if (pfac[a] == a) {
			cnt++;
		}
	}
	cerr << cnt << ' ' << N/3-1 << '\n';
	assert(cnt >= N/3-1);
	return res;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	for (int N = 2; N <= 100; N++) {
		solve(N);
	}

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		auto res = solve(N);
		for (int i = 0; i < N; i++) {
			cout << res[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
