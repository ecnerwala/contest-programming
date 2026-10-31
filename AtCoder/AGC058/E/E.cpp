#include <bits/stdc++.h>

int dist(std::vector<int> inv_a, std::vector<int> b) {
	int N = int(b.size());
	int ans = 0;
	for (int i = 0; i < N; i++) {
		for (int j = i+1; j < N; j++) {
			if (inv_a[b[i]] > inv_a[b[j]]) ans++;
		}
	}
	return ans;
}

std::vector<int> f(std::vector<int> x) {
	int N = int(x.size());
	std::vector<int> inv_x(N);
	for (int i = 0; i < N; i++) inv_x[x[i]] = i;

	std::vector<int> inv_y(N);
	std::iota(inv_y.begin(), inv_y.end(), 0);

	std::vector<int> z(N);
	std::iota(z.begin(), z.end(), 0);
	std::reverse(z.begin(), z.end());
	for (int i = 0; i < N; i++) {
		std::rotate(z.begin() + i, z.end() - 1, z.end());
		for (int j = N-1; true; std::swap(z[i], z[j]), j--) {
			assert(j >= i);
			if (dist(inv_x, z) <= dist(inv_y, z)) {
				break;
			}
		}
	}

	return z;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	for (int N = 3; N <= 7; N++) {
		std::vector<int> x(N);
		std::iota(x.begin(), x.end(), 0);
		std::map<std::vector<int>, std::vector<std::vector<int>>> seen;
		do {
			auto z = f(x);
			seen[z].push_back(x);
		} while (next_permutation(x.begin(), x.end()));
		std::vector<int> id(N);
		std::iota(id.begin(), id.end(), 0);
		for (auto [z, xs] : seen) {
			for (int v : z) { cerr << v << ' '; } cerr << "   " << dist(id, z) << "    ";
			for (int v : xs[0]) { cerr << v << ' '; } cerr << '\n';
		}
	}

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> Z(N);
		for (auto& a : Z) { cin >> a; a--; }
	}

	return 0;
}
