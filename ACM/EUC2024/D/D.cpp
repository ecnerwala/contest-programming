#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::string> A(N);
	for (auto& a : A) cin >> a;

	int cF = 0;
	int cS = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (A[i][j] == 'F') cF++;
			if (A[i][j] == 'S') cS++;
		}
	}
	char cross_color = (cF <= cS) ? 'F' : 'S';

	std::vector<int> par(N, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};
	auto merge = [&](int a, int b) -> bool {
		a = get_par(a), b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) std::swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	};
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (A[i][j] == 'F' || A[i][j] == 'S') {
				merge(i, j);
			}
		}
	}

	std::vector<std::pair<int, int>> groups;
	for (int i = 0; i < N; i++) {
		if (par[i] < 0) {
			groups.push_back({-par[i], i});
		}
	}
	std::sort(groups.begin(), groups.end(), std::greater<>());

	std::vector<int> side(N, 0);

	int lo = (N / 4) - 1, hi = (N / 4);
	int cur_sz = 0;
	for (auto [s, i] : groups) {
		if (cur_sz + s <= hi) {
			cur_sz += s;
			side[i] = 1;
		}
	}
	assert(cur_sz <= hi);
	assert(cur_sz >= lo);

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (A[i][j] == '?') {
				if (side[get_par(i)] == 0 && side[get_par(j)] == 0) {
					// both big
					A[i][j] = 'F' ^ 'S' ^ cross_color;
				} else {
					A[i][j] = cross_color;
				}
			}
		}
	}
	for (int i = 0; i < N; i++) {
		cout << A[i] << '\n';
	}

	return 0;
}
