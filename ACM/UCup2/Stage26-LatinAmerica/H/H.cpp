#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::string S; cin >> S;
	int N = int(S.size());

	std::vector<int> fail(N+1);
	fail[0] = -1;
	for (int i = 0; i < N; i++) {
		int cnd = fail[i];
		while (cnd != -1 && S[i] != S[cnd]) {
			cnd = fail[cnd];
		}
		fail[i+1] = cnd+1;
	}

	{
		int cur = fail[N];
		int new_len = -1;
		for (int i = 0; i < N; i++) {
			while (cur != -1 && S[i] != S[cur]) {
				cur = fail[cur];
			}
			cur++;
			if (cur == N) {
				new_len = i+1;
				break;
			}
		}
		cerr << "new_len " << new_len << '\n';
		assert(new_len != -1);
		assert(N % new_len == 0);
		for (int i = 0; i < N; i++) {
			assert(S[(i + new_len) % N] == S[i]);
		}
		N = new_len;
		S.resize(N);
		fail.resize(N+1);
	}

	int loop_pos = -1;
	{
		int cur = 0;
		for (int i = 0; i < 2*N; i++) {
			char c = S[(N-1-(i%N))];
			while (cur != -1 && c != S[cur]) {
				cur = fail[cur];
			}
			cur++;
			if (cur == N) {
				assert(i >= N);
				loop_pos = ((N-1-(i%N)) + (N-1)) % N;
				break;
			}
		}

		cerr << "loop_pos " << loop_pos << '\n';
		if (loop_pos != -1) {
			for (int i = 0; i < N; i++) {
				assert(S[(N+loop_pos - i) % N] == S[i]);
			}
		}
	}

	struct op_t {
		int v = 0;
		int s = +1;
	};
	// first do a, then do b
	auto merge = [&](op_t a, op_t b) -> op_t {
		return op_t{(b.v + a.v * b.s + N) % N, a.s * b.s};
	};
	int K; cin >> K;
	std::vector<std::array<int, 2>> cnt(N);
	op_t pref{0, +1};
	cnt[pref.v][pref.s == +1] ++;
	int64_t ans = 0;
	for (int i = 0; i < N; i++) {
		char op; cin >> op;
		op_t cur;
		if (op == 'I') {
			cur.v = N-1;
			cur.s = -1;
		} else if (op == 'R') {
			int D; cin >> D;
			cur.v = D % N;
			cur.s = +1;
		} else if (op == 'L') {
			int D; cin >> D;
			cur.v = (N - D % N) % N;
			cur.s = +1;
		} else assert(false);
		pref = merge(pref, cur);

		ans += cnt[pref.v][pref.s == +1];
		if (loop_pos != -1) {
			op_t opref = merge(pref, op_t{loop_pos, -1});
			ans += cnt[opref.v][opref.s == +1];
		}
		cnt[pref.v][pref.s == +1] ++;
	}
	cout << ans << '\n';

	return 0;
}
