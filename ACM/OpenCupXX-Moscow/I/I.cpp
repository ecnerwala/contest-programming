#include<bits/stdc++.h>
using namespace std;

namespace std {

template<class Fun>
	class y_combinator_result {
		Fun fun_;
	public:
		template<class T>
			explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

		template<class ...Args>
			decltype(auto) operator()(Args &&...args) {
				return fun_(std::ref(*this), std::forward<Args>(args)...);
			}
	};

template<class Fun>
	decltype(auto) y_combinator(Fun &&fun) {
		return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
	}

} // namespace std

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int N; cin >> N;
	vector<pair<int, int>> A(N);
	for (auto& it : A) cin >> it.first >> it.second;

	int M; cin >> M;
	vector<pair<int, int>> B(M);
	for (auto& it : B) cin >> it.first >> it.second;

	vector<bool> matched(M, false);

	vector<bool> Avis(N);
	vector<bool> Bvis(M);

	vector<int> Amatch(N, -1);

	vector<int> Bq; Bq.reserve(M);

	const int S = 1<<17;
	vector<vector<pair<int, pair<int, int>>>> seg(2*S);

	using iter = vector<int>::iterator;
	auto build_matching = y_combinator([&](auto self, iter Abegin, iter Aend, iter Bbegin, iter Bend, int x) -> void {
		assert(x >= 0);
		assert(std::distance(Bbegin, Bend) >= x);
		assert(std::distance(Bbegin, Bend-x) <= std::distance(Abegin, Aend));

		if (x == 0) {
			std::for_each(Bbegin, Bend, [&](int i) { matched[i] = true; });
			return;
		}
		if (std::distance(Bbegin, Bend-x) == std::distance(Abegin, Aend)) {
			nth_element(Bbegin, Bend-x, Bend);
			std::for_each(Bbegin, Bend-x, [&](int i) { matched[i] = true; });
			std::for_each(Bend-x, Bend, [&](int i) { matched[i] = false; });
			return;
		}

		nth_element(Bbegin, Bend-x, Bend);
		const int big_ind = *(Bend-x);

		iter Bfull = Bend;
		Bend -= x/2;
		nth_element(Bfull-x, Bend, Bfull);

		std::for_each(Abegin, Aend, [&](int i) { Amatch[i] = -1, Avis[i] = false; });
		std::for_each(Bbegin, Bend, [&](int i) { Bvis[i] = false; });
		Bq.clear();

		sort(Abegin, Aend, [&](int i, int j) { return A[i].first < A[j].first; });
		sort(Bbegin, Bend, [&](int i, int j) { return B[i].first < B[j].first; });

		{
			// Greedy matching
			set<pair<int, int>> st;

			for (iter Ait = Abegin, Bit = Bbegin; Bit != Bend; ++Bit) {
				while (Ait != Aend && A[*Ait].first <= B[*Bit].first) {
					st.emplace(A[*Ait].second, *Ait);
					++Ait;
				}
				auto it = st.upper_bound(pair<int, int>(B[*Bit].second, N));
				if (it == st.begin()) {
					Bvis[*Bit] = true;
					Bq.push_back(*Bit);
				} else {
					--it;
					Amatch[it->second] = *Bit;
					st.erase(it);
				}
			}
		}

		int num_extra = int(Bq.size());

		assert(distance(Bbegin, Bend) - num_extra >= distance(Bbegin, Bfull) - x);

		if (x/2 == 0) {
			assert(x == 1);
			assert(Bend == Bfull);
			// some fast-paths to avoid doing the real partition
			if (Bq.empty()) {
				self(Abegin, Aend, Bbegin, Bend, 0);
			} else {
				self(Abegin, Abegin + std::distance(Bbegin, Bend-1), Bbegin, Bend, 1);
			}
			return;
		}

		std::sort(Abegin, Aend, [&](int i, int j) { return A[i].second > A[j].second; });
		std::for_each(Abegin, Aend, [&](int i) {
			if (Amatch[i] == -1) return;
			for (int a = S+A[i].first; a; a /= 2) {
				seg[a].push_back({A[i].second, {i, Amatch[i]}});
			}
		});

		int num_big = 0;
		for (int z = 0; z < int(Bq.size()); z++) {
			int b_ind = Bq[z];
			num_big += (b_ind >= big_ind);
			for (int a = S, b = S+B[b_ind].first+1; a < b; a /= 2, b /= 2) {
				assert(!(a&1));
				if (b & 1) {
					auto& pq = seg[--b];
					while (!pq.empty() && pq.back().first <= B[b_ind].second) {
						int i, j; tie(i, j) = pq.back().second; pq.pop_back();
						if (!Avis[i]) {
							Avis[i] = true;
							Bvis[j] = true;
							Bq.push_back(j);
						}
					}
				}
			}
		}

		// clear out our segtree garbage
		std::for_each(Abegin, Aend, [&](int i) {
			if (Amatch[i] == -1) return;
			for (int a = S+A[i].first; a; a /= 2) {
				seg[a].clear();
			}
		});

		assert(num_big >= num_extra);

		// partition puts true first
		iter Amid = std::partition(Abegin, Aend, [&](int i) { return Avis[i]; });
		iter Bmid = std::partition(Bbegin, Bend, [&](int i) { return Bvis[i]; });

		assert(std::distance(Bbegin, Bmid) == int(Bq.size()));
		assert(std::distance(Abegin, Amid) == int(Bq.size()) - num_extra);

		self(Abegin, Amid, Bbegin, Bmid, num_big);
		self(Amid, Aend, Bmid, Bfull, x/2);
	});

	vector<int> Ainds(N); iota(Ainds.begin(), Ainds.end(), 0);
	vector<int> Binds(M); iota(Binds.begin(), Binds.end(), 0);
	build_matching(Ainds.begin(), Ainds.end(), Binds.begin(), Binds.end(), M);

	int ans = 0;
	for (int q = 0; q < M; q++) {
		ans += matched[q];
		cout << ans << '\n';
	}

	return 0;
}
