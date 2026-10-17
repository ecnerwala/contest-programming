#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	auto flip_subtrees = [&](int H, std::string S) -> std::string {
		std::string R((1 << H) - 1, '?');
		[&](this auto&& self, int i, int j) -> void {
			R[i-1] = S[j-1];
			if (i >= (1 << (H-1))) return;
			bool f = R[i-1] - '0';
			self(2 * i + 0, 2 * j + (0 ^ f));
			self(2 * i + 1, 2 * j + (1 ^ f));
		}(1, 1);
		return R;
	};

	auto encode = [&](int N, int H, std::string B) -> std::string {
		assert(int(B.size()) == N);
		assert(N <= (1 << H) - 1 - H);
		std::string S((1 << H) - 1, '?');
		{
			auto it = B.begin();
			for (int i = int(S.size()) - 1; i >= 0; i--) {
				if (!i || (i & (i-1))) {
					S[i] = it != B.end() ? *(it++) : '0';
				} else {
					bool b = false;
					for (int j = i+1; j < int(S.size()); j++) {
						if (j & i) b ^= S[j] - '0';
					}
					S[i] = b + '0';
				}
			}
			assert(it == B.end());
		}
		return flip_subtrees(H, std::move(S));
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
	auto decode = [&](int N, int H, std::string S) -> std::string {
		assert(int(S.size()) == (1 << H) - 1);
		assert(N <= (1 << H) - 1 - H);
		S = flip_subtrees(H, std::move(S));
		{
			int f = 0;
			for (int i = 1; i < int(S.size()); i *= 2) {
				bool b = false;
				for (int j = 0; j < int(S.size()); j++) {
					if (j & i) b ^= S[j] - '0';
				}
				if (b) f += i;
			}
			S[f] ^= 1;
		}
		std::string B(N, '?');
		{
			auto it = B.begin();
			for (int i = int(S.size()) - 1; i >= 0; i--) {
				if (!i || (i & (i-1))) {
					if (it != B.end()) *(it++) = S[i];
				}
			}
			assert(it == B.end());
		}
		return B;
	};

	for (int H = 2; H <= 8; H++) {
		int N = (1 << H) - 1 - H;
		for (int msk = 0; msk < (1 << N); msk++) {
			std::string B(N, '?');
			for (int i = 0; i < N; i++) B[i] = '0' + ((msk >> i) & 1);
			std::cerr << "test " << N << ' ' << H << ' ' << B << '\n';
			std::string S = encode(N, H, B);
			//std::cerr << "got " << S << '\n';
			for (int i = 1; i < (1 << H); i++) {
				std::string T = do_flip(N, H, S, i);
				//std::cerr << " flip " << i << ' ' << T << '\n';
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
