#include <bits/stdc++.h>

int eval(const std::vector<int>& P, const std::vector<int>& Q, int K) {
	int N = int(Q.size());
	std::vector<int> invQ(N);
	for (int i = 0; i < N; i++) invQ[Q[i]] = i;

	int ans = 0;
	for (int i = 0; i < N; i++) {
		if (i == K) continue;
		if (invQ[P[Q[i]]] > i) ans++;
	}
	return ans;
}

void test() {
	for (int N = 4; N <= 7; N++) {
		std::vector<std::vector<int>> Ps;
		std::vector<std::vector<int>> Qs;
		{
			std::vector<int> P(N);
			std::iota(P.begin(), P.end(), 0);
			do {
				Qs.push_back(P);
				bool is_derrangement = true;
				for (int i = 0; i < N; i++) { if (P[i] == i) is_derrangement = false; }
				if (is_derrangement) Ps.push_back(P);
			} while (std::next_permutation(P.begin(), P.end()));
		}
		for (int K = 0; K < N; K++) {
			std::map<std::vector<int>, std::vector<int>> mp;
			for (const auto& p : Ps) {
				std::vector<int> results; results.reserve(Qs.size());
				for (const auto& q : Qs) {
					results.push_back(eval(p, q, K));
				}
				mp.insert({results, p});
			}
			if (mp.size() < Ps.size()) {
				continue;
			}
			std::cerr << "good " << N << ' ' << K << '\n';
		}
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	//test();

	auto solve = [&](int N, int K, auto&& query) -> std::vector<int> {
		assert(N >= 4);
		assert(K >= 2);
		assert(K == N/2);
		std::vector<int> P(N, -1);
		for (int i = 0; i < N; i++) {
			std::vector<int> Q_base(N);
			std::iota(Q_base.begin(), Q_base.end(), 0);
			std::rotate(Q_base.begin(), Q_base.begin() + i, Q_base.end());
			assert(Q_base[0] == i);
			// it'll be +1 in a range of size K, and -1 in a range of size N-1-k;
			auto build_query = [&](int d) -> std::vector<int> {
				d %= (N-1);
				if (d < 0) d += N-1;
				auto Q = Q_base;
				std::rotate(Q.begin() + 1, Q.end() - d, Q.end());
				// rotate the last K elements to the front
				std::rotate(Q.begin(), Q.end() - K, Q.end());
				assert(Q[K] == i);
				return Q;
			};
			auto query_shift = [&](int d) -> int {
				return query(build_query(d));
			};

			const int cnt_big = K;
			const int cnt_small = N-1-K;
			assert(cnt_big >= cnt_small);
			assert(cnt_big - cnt_small <= 1);
			int mi, ma; // big -> small transition point
			int v_big, v_small;

			{
				int v_base = query_shift(0);
				int v_oppo = query_shift(cnt_small);
				if (v_base == v_oppo) {
					assert(cnt_big - cnt_small == 1);
					// this must be exactly 0..cnt_small are the big guys
					v_big = v_base;
					v_small = v_big - 1;
					mi = cnt_small;
					ma = cnt_small + 1;
					assert(query_shift(ma) == v_small);
				} else if (v_base > v_oppo) {
					assert(v_base - v_oppo == 1);
					v_big = v_base, v_small = v_oppo;
					mi = 0, ma = cnt_small;
				} else if (v_oppo > v_base) {
					assert(v_oppo - v_base == 1);
					v_big = v_oppo, v_small = v_base;
					mi = cnt_small, ma = N-1;
				} else assert(false);
			}
			assert(v_big - v_small == 1);
			while (ma - mi > 1) {
				int md = mi + (ma - mi) / 2;
				auto r = query_shift(md);
				assert(r == v_big || r == v_small);
				if (r == v_big) mi = md;
				else if (r == v_small) ma = md;
				else assert(false);
			}
			assert(ma - mi == 1);
			assert(build_query(mi)[K-1] == build_query(ma)[K+1]);
			P[build_query(mi)[K-1]] = i;
		}
		return P;
	};

	if (true) {
		int T = 1000;
		std::mt19937 mt(48);
		while (T--) {
			int N = std::uniform_int_distribution<int>(4, 10)(mt);

			std::vector<int> P(N);
			std::iota(P.begin(), P.end(), 0);
			while (true) {
				std::shuffle(P.begin(), P.end(), mt);
				bool is_derrangement = true;
				for (int i = 0; i < N; i++) { if (P[i] == i) is_derrangement = false; }
				if (is_derrangement) break;
			}
			cerr << "test " << N << '\n';
			for (auto p : P) { cerr << p << ' '; } cerr << '\n';

			int K = N/2;

			int num_queries = 0;
			auto query = [&](const std::vector<int>& Q) -> int {
				num_queries++;
				if (num_queries > 10 * N) assert(false);
				return eval(P, Q, K);
			};

			auto P_mine = solve(N, K, query);
			assert(P == P_mine);
		}
	}

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		if (N == -1) exit(0);

		int K = N / 2;
		cout << K+1 << '\n';

		int num_queries = 0;
		auto query = [&](const std::vector<int>& Q) -> int {
			cout << '?';
			for (auto q : Q) cout << ' ' << q+1;
			cout << '\n' << flush;
			num_queries++;
			if (num_queries > 15 * N) exit(0);
			int res; cin >> res;
			return res;
		};

		auto P = solve(N, K, query);
		cout << '!';
		for (auto p : P) cout << ' ' << p+1;
		cout << '\n' << flush;
	}

	return 0;
}
