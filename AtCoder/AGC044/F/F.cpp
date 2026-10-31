#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	// takes N!
	auto check_clubs = [](int N, vector<int> clubs) -> bool {
		assert(is_sorted(clubs.begin(), clubs.end()));

		vector<int> perm(N); iota(perm.begin(), perm.end(), 0);
		vector<int> nclubs(clubs.size());
		// skip the identity
		while (next_permutation(perm.begin(), perm.end())) {
			for (int z = 0; z < int(clubs.size()); z++) {
				nclubs[z] = 0;
				for (int i = 0; i < N; i++) {
					if (clubs[z] & (1 << i)) {
						nclubs[z] |= 1 << perm[i];
					}
				}
			}
			sort(nclubs.begin(), nclubs.end());
			if (clubs == nclubs) {
				return false;
			}
		}
		return true;
	};

	auto count_name_preserving = [&](int N, int K, bool mustDistinct) -> int {
		//cerr << "count_name_preserving " << N << ' ' << K << ' ' << mustDistinct << '\n';
		assert(K <= N && N <= (1 << K));

		int ans = 0;
		int ans_den = 1; for (int i = 1; i <= K; i++) ans_den *= i;
		vector<int> perm(1 << K); iota(perm.begin(), perm.end(), 0);
		reverse(perm.begin() + N, perm.end());

		auto next_choose = [&]() -> bool {
			if (N == 0 || perm[0] == (1<<K)-N) return false;

			int i = N-1, j = N;
			while (perm[i] >= perm[j]) --i;
			while (j+1 < (1 << K) && perm[j+1] >= perm[i]) j++;
			reverse(perm.begin()+i+1, perm.begin()+N);
			reverse(perm.begin()+i, perm.begin()+j+1);
			reverse(perm.begin()+N, perm.begin()+j);

			assert(is_sorted(perm.begin(), perm.begin()+N));
			assert(is_sorted(perm.begin()+N, perm.end(), std::greater()));
			return true;
		};

		vector<int> clubs(K);
		do {
			fill(clubs.begin(), clubs.end(), 0);
			for (int i = 0; i < N; i++) {
				for (int k = 0; k < K; k++) {
					if (perm[i] & (1 << k)) clubs[k] |= 1 << i;
				}
			}

			/*
			cerr << "try" << '\n';
			for (int k = 0; k < K; k++) {
				cerr << bitset<10>(clubs[k]) << '\n';
			}
			*/

			sort(clubs.begin(), clubs.end());
			if (mustDistinct && adjacent_find(clubs.begin(), clubs.end()) != clubs.end()) {
				continue;
			}

			if (check_clubs(N, clubs)) {
				//cerr << "good!" << '\n';
				int weight;
				if (mustDistinct) {
					weight = 1;
				} else {
					weight = 1;
					int cur = 1;
					for (int i = 1; i < K; i++) {
						if (clubs[i] != clubs[i-1]) cur = 0;
						weight *= ++cur;
					}
					//cerr << "weight " << weight << '\n';
				}
				ans += weight;
				if (ans > 1000 * ans_den) return 1001;
			}
		} while (next_choose());
		assert(ans % ans_den == 0);
		return ans / ans_den;
	};

	auto precompute_table = [&]() {
		auto print_row = [&](int N, int K, bool mustDistinct) {
			cerr << std::boolalpha << "precomputed[{" << N << "," << K << "," << mustDistinct << "}] = " << count_name_preserving(N, K, mustDistinct) << ";" << '\n';
		};
		print_row(4, 3, false);
		print_row(7, 4, false);
		print_row(8, 4, false);
		for (int K = 0; K <= 5; K++) {
			for (int N = K; N <= (1 << K) / 2; N++) {
				print_row(N, K, true);
			}
		}
	};
	if (false) precompute_table();

	map<tuple<int, int, bool>, int> precomputed;
	precomputed[{4,3,false}] = 7;
	precomputed[{7,4,false}] = 336;
	precomputed[{8,4,false}] = 384;
	precomputed[{0,0,true}] = 1;
	precomputed[{1,1,true}] = 2;
	precomputed[{2,2,true}] = 2;
	precomputed[{3,3,true}] = 4;
	precomputed[{4,3,true}] = 6;
	precomputed[{4,4,true}] = 36;
	precomputed[{5,4,true}] = 108;
	precomputed[{6,4,true}] = 220;
	precomputed[{7,4,true}] = 334;
	precomputed[{8,4,true}] = 384;
	precomputed[{5,5,true}] = 976;

	auto count_name_preserving_cached = [&](int N, int K, bool mustDistinct) {
		assert(precomputed.count({N, K, mustDistinct}));
		return precomputed[{N, K, mustDistinct}];
	};

	auto go = [&](int64_t N) -> int {
		vector<int64_t> limits({1});
		for (int i = 1; limits.back() < N; i++) {
			int j = 0; while (i > limits[j]) j++;
			assert(j < i);
			limits.push_back((int64_t(1) << i) - j);
		}

		//for (int i = 0; i < int(limits.size()); i++) { cerr << i << ' ' << limits[i] << '\n'; }

		int64_t K = int(limits.size())-1;

		if (N == 4 || N == 7 || N == 8) {
			// These can have non-unique ones, so we'll brute force them
			return count_name_preserving_cached(int(N), int(K), false);
		}

		while (true) {
			assert(K < int(limits.size()));
			assert(N <= limits[K]);
			N = min(N, (int64_t(1) << K) - N);
			if (N < K) {
				swap(N, K);
				continue;
			}
			assert(K <= N);
			assert(N <= (int64_t(1) << K) / 2);

			if (K >= 6 || (K == 5 && N >= 6)) return 1001;
			return count_name_preserving_cached(int(N), int(K), true);
		}
	};

	int64_t N; cin >> N;
	int ans = go(N);

	if (ans > 1000) cout << -1 << '\n';
	else cout << ans << '\n';

	return 0;
}
