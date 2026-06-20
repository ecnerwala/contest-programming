#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	const int V = int(1e6);
	std::vector<int> primes; primes.reserve(V);
	std::vector<int> pfac(V+1);
	for (int i = 2; i <= V; i++) {
		if (!pfac[i]) {
			primes.push_back(i);
			pfac[i] = i;
		}
		for (int p : primes) {
			if (int64_t(i) * p > V) break;
			pfac[i*p] = p;
			if (p == pfac[i]) break;
		}
	}
	const int NUM_P = 150;
	const int MAX_P = primes[NUM_P-1];
	std::vector<int> smooth; smooth.reserve(V);
	smooth.push_back(1);
	for (int z = 0; z < int(smooth.size()); z++) {
		int i = smooth[z];
		for (int p : primes) {
			if (p > MAX_P) break;
			if (int64_t(i) * p > V) break;
			smooth.push_back(i*p);
			if (p == pfac[i]) break;
		}
	}
	smooth.resize(1 << 18);
	std::vector<int> smooth_idx(V+1, -1);
	for (int i = 0; i < int(smooth.size()); i++) smooth_idx[smooth[i]] = i;

	std::vector<int> p_pows(NUM_P);
	for (int i = 0; i < NUM_P; i++) {
		int p = primes[i];
		int q = p;
		while (q <= V / p) q *= p;
		p_pows[i] = q;
	}

	auto to_bits = [&](std::vector<int> A, int W) -> std::vector<bool> {
		std::vector<bool> B; B.reserve(int(A.size()) * W);
		for (int i = 0; i < int(A.size()); i++) {
			for (int b = 0; b < W; b++) {
				B.push_back((A[i] >> b) & 1);
			}
		}
		return B;
	};
	auto from_bits = [&](std::vector<bool> B, int W, int N = -1) -> std::vector<int> {
		if (N == -1) N = (int(B.size()) + W - 1) / W;
		std::vector<int> A(N);
		B.resize(W * N, false);
		for (int i = 0; i < int(A.size()); i++) {
			for (int b = 0; b < W; b++) {
				A[i] |= (B[i*W + b]) << b;
			}
		}
		return A;
	};

	std::string run; std::cin >> run;
	int T; std::cin >> T;
	while (T--) {
		if (run == "first") {
			int N; std::cin >> N;
			std::vector<int> A(N); for (auto& a : A) std::cin >> a;
			std::vector<int> B = from_bits(to_bits(A, 20), 18);
			for (auto& v : B) v = smooth[v];
			B.insert(B.end(), p_pows.begin(), p_pows.end());
			std::cout << int(B.size()) << '\n';
			for (int i = 0; i < int(B.size()); i++) {
				std::cout << B[i] << " \n"[i+1==int(B.size())];
			}

			{
				int K = int(B.size());
				K -= NUM_P;
				std::vector<int> B_test(K);
				for (int i = 0; i < K; i++) {
					int b = 1;
					for (int z = 0; z < NUM_P; z++) {
						int g = std::gcd(B[i], B[K+z]);
						if (g == -1) exit(0);
						b *= g;
					}
					B_test[i] = smooth_idx[b];
				}
				std::vector<int> A_test = from_bits(to_bits(B_test, 18), 20, N);
				assert(A_test == A);
			}
		} else if (run == "second") {
			int N, K; std::cin >> N >> K;
			K -= NUM_P;
			std::vector<int> B(K);
			for (int i = 0; i < K; i++) {
				int b = 1;
				for (int z = 0; z < NUM_P; z++) {
					std::cout << '?' << ' ' << i+1 << ' ' << K+z+1 << '\n' << std::flush;
					int g; std::cin >> g;
					if (g == -1) exit(0);
					b *= g;
				}
				B[i] = b;
			}
			std::vector<int> A = from_bits(to_bits(B, 18), 20, N);
			std::cout << '!';
			for (int i = 0; i < int(A.size()); i++) { std::cout << ' ' << A[i]; }
			std::cout << '\n' << std::flush;
		} else assert(false);
	}

	return 0;
}
