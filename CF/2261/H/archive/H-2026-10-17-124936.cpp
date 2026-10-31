#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	auto encode = [&](int N, int H, std::string B) -> std::string {
		assert(int(B.size()) == N);
		assert((N == 1 && H == 3) || (N == 2030 && H == 11));
		assert(N == 2030 && H == 11);
		std::string S((1 << H) - 1, '?');
		{
			auto it = B.begin();
			for (int i = 1; i < (1 << H) - 1; i++) {
				if (i & (i-1)) {
					S[i-1] = it != B.end() ? *(it++) : '0';
				}
			}
			assert(it == B.end());
		}
		// Now, let's fill in the extras in S
		{
			// S[1-1] is the sum of all right children
			bool b = false;
			for (int i = 2; i < (1 << H); i++) {
				if (i & 1) b ^= S[i-1] - '0';
			}
			S[1-1] = '0' + b;
		}
		for (int i = 1; i < H; i++) {
			// S[(1<<i) - 1] is the sum of everything in its row, as well as the entire subtrees of its odd positions
			bool b = false;
			for (int k = (1 << i) + 1; k < (2 << i); k++) b ^= S[k-1] - '0';
			for (int j = i+1; j < H; j++) {
				for (int k = (1 << j); k < (2 << j); k++) {
					if ((k >> (j-i)) & 1) b ^= S[k-1] - '0';
				}
			}
			S[(1 << i) - 1] = '0' + b;
		}
		return S;
	};
	auto decode = [&](int N, int H, std::string S) -> std::string {
		assert(int(S.size()) == (1 << H) - 1);
		assert((N == 1 && H == 3) || (N == 2030 && H == 11));
		assert(N == 2030 && H == 11);
		{
			std::string B(N, '?');
			auto it = B.begin();
			for (int i = 1; i < (1 << H) - 1; i++) {
				if (i & (i-1)) {
					if (it != B.end()) *(it++) = S[i-1];
				}
			}
			assert(it == B.end());
			return B;
		}
	};

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
