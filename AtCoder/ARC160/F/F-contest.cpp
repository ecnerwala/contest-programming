#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;

	using bs = bitset<1 << 15>;
	std::vector<bs> vals(N);
	for (int b = 0; b < N; b++) {
		for (int i = 0; i < (1 << N); i++) {
			vals[b][i] = bool(i & (1 << b));
		}
	}

	std::vector<int> cur_res(1<<N);
	std::iota(cur_res.begin(), cur_res.end(), 0);

	std::vector<int64_t> dp(1<<N);
	std::vector<bool> alive(1<<N);
	for (int i = (1<<N)-1; i >= 0; i--) {
		if (!(cur_res[i] & (cur_res[i]+1))) {
			alive[i] = true;
			//cerr << "marking ";
			//for (int z = 0; z < N; z++) { cerr << ((i >> z) & 1); }
			//cerr << " alive" << '\n';
			int64_t res = 0;
			for (int v = (1 << N) - 1 - i; v > 0; v &= v-1) {
				int off = v & -v;
				res += dp[i + off];
			}
			if (i == (1 << N) - 1) res = 1;
			dp[i] = res;
		}
	}

	auto mark_done = [&](int a) -> void {
		assert(!alive[a]);
		alive[a] = true;
		//cerr << "marking ";
		//for (int i = 0; i < N; i++) { cerr << ((a >> i) & 1); }
		//cerr << " alive" << '\n';
		for (int i = a; true; i = (i-1) & a) {
			if (!alive[i]) continue;
			int64_t res = 0;
			for (int v = (1 << N) - 1 - i; v > 0; v &= v-1) {
				int off = v & -v;
				res += dp[i + off];
			}
			dp[i] = res;
			if (i == 0) break;
		}
	};

	auto do_query = [&](int a, int b) -> int64_t {
		assert(0 <= a && a < b && b < N);
		bs to_flip = ~vals[a] & vals[b];
		vals[a] ^= to_flip;
		vals[b] ^= to_flip;
		for (int i = int(to_flip._Find_first()); i < (1 << N); i = int(to_flip._Find_next(i))) {
			cur_res[i] ^= (1 << a) ^ (1 << b);
			if (!(cur_res[i] & (cur_res[i]+1))) {
				mark_done(i);
			}
		}
		return dp[0];
	};

	int64_t prvS = 1;

	for (int q = 0; q < Q; q++) {
		int a, b;
		{
			cin >> a >> b;
			a = int((a + prvS) % N);
			b = int((b + prvS * 2) % N);
			assert(a != b);
			if (a > b) std::swap(a, b);
			assert(0 <= a && a < b && b < N);
		}

		//cerr << "query " << a << ' ' << b << '\n';
		prvS = do_query(a, b);
		//cerr << prvS << '\n';
		cout << prvS << '\n';
	}

	return 0;
}
