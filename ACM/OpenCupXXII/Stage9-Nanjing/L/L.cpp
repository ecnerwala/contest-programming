#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> S(N);
		{
			string S_; S_.reserve(N); cin >> S_;
			for (int i = 0; i < N; i++) {
				S[i] = S_[i] - '0';
			}
		}

		auto mod = [&](int a) -> int {
			a %= N;
			if (a < 0) a += N;
			return a;
		};

		auto solve = [&]() {
			std::vector<int> ops; ops.reserve(2*N);

			std::vector<int> F(N);
			for (int st = 0; st < (N % 3 == 0 ? 3 : 1); st++) {
				F[st] = 0;
				for (int cur = st; true; ) {
					int a = (cur + 1) % N;
					int b = (cur + 2) % N;
					int nxt = (cur + 3) % N;
					int vnxt = F[cur] ^ S[a] ^ S[b];
					if (nxt == st) {
						if (vnxt != F[nxt]) {
							assert(N % 3 == 0);
							cout << 0 << '\n';
							return;
						}
						break;
					}
					F[nxt] = vnxt;
					cur = nxt;
				}
			}

			if ((F[0] ^ F[1] ^ F[2]) != !S[1]) {
				// flip everything
				for (int cur = 0; true; ) {
					F[cur] ^= 1;
					cur += 3;
					cur %= N;
					if (cur == 0) break;
				}
			}

			for (int i = 0; i < N; i++) {
				if ((F[i] ^ F[(i+1)%N] ^ F[(i+2)%N]) != !S[(i+1)%N]) {
					assert(false);
				}
			}

			auto do_op = [&](int a) -> void {
				assert(S[a] == 0);
				ops.push_back(a);

				S[a] ^= 1;
				S[mod(a-1)] ^= 1;
				S[mod(a+1)] ^= 1;

				F[a] ^= 1;
			};

			for (int z = 0; z < 3; z++) {
				for (int i = 0; i < N; i++) {
					if (F[(i+1)%N] == 1 && F[i] == F[(i+2)%N]) {
						do_op((i+1)%N);
					}
				}
			}

			// now, it must be of the form 011001100110

			for (int z = 0; z < 3; z++) {
				for (int i = 0; i < N; i++) {
					if (F[i] == 0 && F[mod(i+1)] == 0 && F[mod(i+2)] == 1) {
						assert(F[mod(i+3)] == 1);
						assert(F[mod(i+4)] == 0);
						do_op(mod(i+1));
						do_op(mod(i+2));
						do_op(mod(i+1));
						do_op(mod(i+3));
					}
				}
			}

			cout << ops.size() << '\n';
			for (int i = 0; i < int(ops.size()); i++) {
				cout << ops[i]+1 << " \n"[i+1==int(ops.size())];
			}
		};

		solve();
	}

	return 0;
}
