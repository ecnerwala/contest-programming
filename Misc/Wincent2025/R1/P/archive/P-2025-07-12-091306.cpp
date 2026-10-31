#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K, P; cin >> N >> K >> P;
		std::vector<int> S_freq(N);
		for (int i = 0; i < N; i++) {
			int s; cin >> s; S_freq[s]++;
		}

		assert(P > 0 && K >= 1);

		// probability that you convince k people out of a subset of size i
		std::vector<std::vector<double>> p_convince(N, std::vector<double>(K+1));
		for (int i = 1; i <= N-1; i++) {
			double cur_prob = 1;
			for (int k = std::min(K, i); k > 0; k--) {
				// what's the probability that you talk with exactly k of the i people
				if (k == K) {
					for (int a = 0; a < K; a++) {
						cur_prob *= i - a;
						cur_prob /= N-1-a;
					}
				} else if (k == i) {
					// pick all of these
					for (int a = 0; a < i; a++) {
						cur_prob *= K - a;
						cur_prob /= N-1-a;
					}
				} else {
					// go from 1 above downwards
					cur_prob *= k+1;
					cur_prob /= (i-k);

					cur_prob *= N-1-i - (K-k) + 1;
					cur_prob /= K-k;
				}

				auto slow_choose = [&](int n, int r) -> double {
					if (0 <= r && r <= n) {
						double res = 1;
						for (int a = 0; a < r; a++) {
							res *= n - a;
							res /= a + 1;
						}
						return res;
					} else {
						return 0;
					}
				};

				double slow_prob = slow_choose(i, k) * slow_choose(N - 1 - i, K - k) / slow_choose(N - 1, K);
				assert(std::abs(slow_prob - cur_prob) / slow_prob <= 1e-8);

				p_convince[i][0] += cur_prob;
				{
					// do one round
					for (int z = K-k; z >= 0; z--) {
						double a = p_convince[i][z];
						p_convince[i][z+1] += a * P / 100;
						p_convince[i][z] = a * (100 - P) / 100;
					}
				}
			}
		}

		// Solve the N, K, P system
		// P[win] is equal to freq / N, by some symmetry arguments
		// so we only care about E[(time if win else 0)]
		std::vector<std::vector<double>> eqns;
		for (int f = 0; f <= N; f++) {
		}
	}

	return 0;
}
