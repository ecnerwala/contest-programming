#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class EllysThreePrimes {

public:

	vector <int> getPrimes(vector <int> sums) {
		static const int V = int(1e5);
		vector<bool> isPrime(V);
		for (int p = 2; p < V; p++) isPrime[p] = true;
		for (int p = 2; p < V; p++) {
			if (!isPrime[p]) continue;
			for (int j = p + p; j < V; j += p) isPrime[j] = false;
		}

		vector<int> primes;
		for (int p = V / 10; p < V; p++) {
			if (isPrime[p]) {
				primes.push_back(p);
			}
		}

		int sum = sums[0] + sums[1] * 10 + sums[2] * 100 + sums[3] * 1000 + sums[4] * 10000;

		for (int i = 0; i < int(primes.size()); i++) {
			int p = primes[i];
			for (int j = i+1; j < int(primes.size()); j++) {
				int q = primes[j];
				int v = sum - p - q;
				if (v > primes[j] && v < V && isPrime[v] &&
						(sums[0] == p % 10 + q % 10 + v % 10) &&
						(sums[1] == p / 10 % 10 + q / 10 % 10 + v / 10 % 10) &&
						(sums[2] == p / 100 % 10 + q / 100 % 10 + v / 100 % 10) &&
						(sums[3] == p / 1000 % 10 + q / 1000 % 10 + v / 1000 % 10) &&
						(sums[4] == p / 10000 % 10 + q / 10000 % 10 + v / 10000 % 10)
						) {
					return {p, q, v};
				}
			}
		}
		return {};
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::EllysThreePrimes;

// vim:ft=cpp
