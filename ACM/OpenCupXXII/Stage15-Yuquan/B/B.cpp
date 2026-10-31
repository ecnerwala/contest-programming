#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	/*
	ifstream fin("/home/andrew/Dropbox/programming/ProjectEuler/primes/primes.txt");
	std::vector<int> primes(5e7);
	for (auto& p : primes) fin >> p;
	cerr << primes.end()[-2] << '\n';
	cerr << primes.back() << '\n';
	const int64_t MX = 1e18;
	auto is_pow_2 = [&](int64_t a) {
		return (a & (a - 1)) == 0;
	};
	for (int i = 0; i < int(primes.size()); i++) {
		int64_t x = 1;
		for (int j = i; j < int(primes.size()); j++) {
			if ((x + 1) % primes[j] == 0 && is_pow_2((x + 1) / primes[j])) {
				cerr << i << ' ' << j << ' ' << primes[i] << ' ' << primes[j] << ' ' << x << '\n';
			}

			if (__int128(x) * primes[j] > MX) break;
			x *= primes[j];
		}
	}
	*/
	int64_t N; cin >> N;
	cout << (N >= 2431) << '\n';

	return 0;
}
