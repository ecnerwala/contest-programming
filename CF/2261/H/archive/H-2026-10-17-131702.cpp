#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	auto get_checksum = [&](int N, int H, const std::string& S, int i) -> bool {
		assert(i >= 0 && i < H);
		bool b = false;
		if (i == 0) {
			for (int k = 1; k < (1 << (H-1)); k++) {
				// Pick exactly 1 child of each guy to include, which is stable under flip
				b ^= S[2*k + !(k == 1 ? 0 : S[k-1] - '0')] - '0';
			}
		} else {
			// S[(1<<i) - 1] is the sum of everything in its row, as well as the entire subtrees of its odd positions
			for (int k = (1 << i) + 1; k < (2 << i); k++) b ^= S[k-1] - '0';
			for (int j = i+1; j < H; j++) {
				for (int k = (1 << j); k < (2 << j); k++) {
					if ((k >> (j-i)) & 1) b ^= S[k-1] - '0';
				}
			}
		}
		return b;
	};

	auto encode = [&](int N, int H, std::string B) -> std::string {
		assert(int(B.size()) == N);
		assert(N <= (1 << H) - 1 - H);
		std::string S((1 << H) - 1, '?');
		{
			auto it = B.begin();
			for (int i = 1; i < (1 << H); i++) {
				if (i & (i-1)) {
					S[i-1] = it != B.end() ? *(it++) : '0';
				}
			}
			assert(it == B.end());
		}
		// Now, let's fill in the extras in S
		for (int i = H-1; i >= 0; i--) {
			S[(1 << i) - 1] = '0' + get_checksum(N, H, S, i);
		}
		return S;
	};
	auto do_flip = [&](int N, int H, std::string S, int p) -> std::string {
		assert(1 <= p && p < (1 << H));
		S[p-1] ^= 1;
		for (int a = 2 * p + 0, b = 2 * p + 1, l = 1; a < (1 << H); a <<= 1, b <<= 1, l <<= 1) {
			for (int k = 0; k < l; k++) {
				std::swap(S[a+k-1], S[b+k-1]);
			}
		}
		return S;
	};
	auto get_flip_pos = [&](int N, int H, const std::string& S) -> int {
		for (int i = H-1; true; i--) {
			if (i == 0) {
				return 1;
			}
			assert(i > 0);

			if (S[(1 << i) - 1] != '0' + get_checksum(N, H, S, i)) {
				// figure out where the flip is
				int pos = (1 << i);
				for (int j = i-1; j > 0; j--) {
					if (S[(1 << j) - 1] != '0' + get_checksum(N, H, S, j)) {
						pos += 1 << (i - j);
					}
				}
				assert(!(pos & 1));
				pos ^= (pos/2 == 1 ? 0 : S[pos/2-1] - '0');
				if (S[1 - 1] != '0' + get_checksum(N, H, S, 0)) {
					// do nothing
					pos ^= 1;
				} else {
				}
				assert((1 << i) <= pos && pos < (2 << i));
				return pos;
			}
		}
	};
	auto decode = [&](int N, int H, std::string S) -> std::string {
		assert(int(S.size()) == (1 << H) - 1);
		assert(N <= (1 << H) - 1 - H);
		int pos = get_flip_pos(N, H, S);
		S = do_flip(N, H, S, pos);
		{
			std::string B(N, '?');
			auto it = B.begin();
			for (int i = 1; i < (1 << H); i++) {
				if (i & (i-1)) {
					if (it != B.end()) *(it++) = S[i-1];
				}
			}
			assert(it == B.end());
			return B;
		}
	};

	for (int H = 2; H <= 10; H++) {
		int N = (1 << H) - 1 - H;
		for (int msk = 0; msk < (1 << N); msk++) {
			std::string B(N, '?');
			for (int i = 0; i < N; i++) B[i] = '0' + ((msk >> i) & 1);
			std::cerr << "test " << N << ' ' << H << ' ' << B << '\n';
			std::string S = encode(N, H, B);
			std::cerr << "got " << S << '\n';
			for (int i = 1; i < (1 << H); i++) {
				std::string T = do_flip(N, H, S, i);
				std::cerr << " flip " << i << ' ' << T << '\n';
				assert(int(T.size()) == int(S.size()));
				int r = get_flip_pos(N, H, T);
				std::cerr << "recovered " << r << '\n';
				assert(r == i);
				assert(decode(N, H, T) == B);
			}
		}
	}

	std::string op; std::cin >> op;
	assert(op == "first" || op == "second");
	int T; std::cin >> T;
	while (T--) {
		int N, H; std::cin >> N >> H;
		if (op == "first") {
			std::string B; B.reserve(N); std::cin >> B;
			std::cout << encode(N, H, B) << '\n';
		} else if (op == "second") {
			std::string S; S.reserve((1 << H) - 1); std::cin >> S;
			std::cout << decode(N, H, S) << '\n';
		} else assert(false);
	}

	return 0;
}
