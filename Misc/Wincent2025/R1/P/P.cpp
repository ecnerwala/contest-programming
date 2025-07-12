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

		cerr << "Running case " << N << ' ' << K << ' ' << P << '\n';

		assert(P > 0 && K >= 1);

		// probability that you convince k people out of a subset of size i
		std::vector<std::vector<double>> p_convince(N, std::vector<double>(K+1));
		for (int i = 1; i <= N-1; i++) {
			double cur_prob = 1;
			for (int k = std::min(K, i); k >= 0; k--) {
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

				p_convince[i][0] += cur_prob;
				if (k > 0) {
					// do one round
					for (int z = K-k; z >= 0; z--) {
						double a = p_convince[i][z];
						p_convince[i][z+1] += a * P / 100;
						p_convince[i][z] = a * (100 - P) / 100;
					}
				}
			}
			//for (int k = 0; k <= K; k++) { cerr << p_convince[i][k] << ' '; } cerr << '\n';
		}

		// Solve the N, K, P system
		// P[win] is equal to freq / N, by some symmetry arguments
		// so we only care about E[(time if win else 0)]
		// TODO: Make this sparse
		std::vector<std::vector<double>> eqn_early(N+1, std::vector<double>(K+1));
		std::vector<std::vector<double>> eqn_near(N+1, std::vector<double>(2*K+1));
		auto eqns = [&](int i, int j) -> double& {
			if (j <= K) return eqn_early[i][j];
			else if (i-K <= j && j <= i+K) return eqn_near[i][j-(i-K)];
			else assert(false);
		};
		std::vector<double> b(N+1);
		for (int f = 0; f <= N; f++) {
			eqns(f, f) = 1;
			if (f == 0 || f == N) continue;

			double pf = double(f) / N;
			// += p_win
			b[f] += pf;

			for (int a = 0; a <= std::min(N-f, K); a++) {
				eqns(f, f+a) -= p_convince[N-f][a] * pf;
			}
			for (int a = 0; a <= std::min(f, K); a++) {
				eqns(f, f-a) -= p_convince[f][a] * (1-pf);
			}
		}
		for (int z = 0; z < N+1; z++) {
			//for (int i = 0; i < N+1; i++) { cerr << eqns[z][i] << ' '; } cerr << ": " << b[z]; cerr << '\n';
		}
		// now just gaussian eliminate
		std::vector<double> vals(N+1);
		for (int a = K+1; a <= N; a++) {
			int f = a-K;
			double v = eqn_early[f][2*K];
			assert(v < 0);
			for (int j = a+1; j <= )
		}

		double ans = 0;
		for (int f : S_freq) {
			ans += vals[f];
		}
		cout << fixed << setprecision(20) << ans << '\n';
	}

	return 0;
}
