#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int N, D; std::cin >> N >> D;
	std::vector<int> A(N); for (auto& a : A) { std::cin >> a; a--; }
	std::vector<int> B(N); for (auto& b : B) { std::cin >> b; b--; }
	std::vector<int> invA(N); for (int i = 0; i < N; i++) invA[A[i]] = i;

	std::vector<int> day_to_idx(N);
	std::vector<int> idx_to_day(N);
	for (int i = 0; i < N; i++) {
		day_to_idx[i] = invA[B[i]];
		idx_to_day[invA[B[i]]] = i;
	}

	auto get_cost = [&](int i) -> int {
		if (i == N) return 0;
		int p = i == 0 ? N-1 : day_to_idx[i-1];
		int n = day_to_idx[i];
		if (n <= p) n += N;
		return n - p - 1;
	};
	int64_t tot_cost = 0;
	for (int i = 0; i < N; i++) {
		tot_cost += get_cost(i);
	}

	std::cout << tot_cost << '\n';
	std::vector<int> to_upd; to_upd.reserve(4);
	for (int q = 1; q < D; q++) {
		int op; std::cin >> op;
		int i, j; std::cin >> i >> j; i--, j--;
		if (op == 1) {
			i = idx_to_day[i];
			j = idx_to_day[j];
		} else if (op == 2) {
		} else assert(false);

		to_upd.clear();
		to_upd.push_back(i);
		to_upd.push_back(i+1);
		to_upd.push_back(j);
		to_upd.push_back(j+1);
		std::sort(to_upd.begin(), to_upd.end());
		to_upd.resize(std::unique(to_upd.begin(), to_upd.end()) - to_upd.begin());
		for (auto t : to_upd) tot_cost -= get_cost(t);

		std::swap(day_to_idx[i], day_to_idx[j]);
		idx_to_day[day_to_idx[i]] = i;
		idx_to_day[day_to_idx[j]] = j;

		for (auto t : to_upd) tot_cost += get_cost(t);

		std::cout << tot_cost << '\n';
	}

	return 0;
}
