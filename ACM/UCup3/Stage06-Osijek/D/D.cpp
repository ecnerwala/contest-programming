#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, Q; cin >> N >> M >> Q;
	N += 2, M += 2;

	auto v = [&](int a, int b) -> int {
		assert(0 <= a && a < N && 0 <= b && b < M);
		return a * M + b;
	};

	const int V = N*M;
	std::vector<bool> filled(V);
	std::vector<int> par(V, -1);
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
	auto op = [&](int x, int y) -> bool {
		cerr << "trying " << x << ' ' << y << '\n';
		// check for 3x3
		filled[v(x, y)] = true;
		for (int dx = -1; dx <= 1; dx++) {
			for (int dy = -1; dy <= 1; dy++) {
				if (filled[v(x + dx, y + dy)]) {
					int cnt = 0;
					for (int nx = -1; nx <= 1; nx++) {
						for (int ny = -1; ny <= 1; ny++) {
							cnt += filled[v(x + dx + nx, y + dy + ny)];
						}
					}
					if (cnt == 9) {
						cerr << "bad 3x3 at " << x + dx << ' ' << y + dy << '\n';
						filled[v(x, y)] = false;
						return false;
					}
				}
			}
		}
		filled[v(x, y)] = false;
		std::array<std::pair<int, int>, 8> neighbors = {
			std::pair<int, int>{x-1, y  },
			std::pair<int, int>{x-1, y-1},
			std::pair<int, int>{x  , y-1},
			std::pair<int, int>{x+1, y-1},
			std::pair<int, int>{x+1, y  },
			std::pair<int, int>{x+1, y+1},
			std::pair<int, int>{x  , y+1},
			std::pair<int, int>{x-1, y+1},
		};
		std::array<int, 8> comps;
		for (int i = 0; i < 8; i ++) {
			auto [a, b] = neighbors[i];
			int p = v(a, b);
			comps[i] = filled[p] ? get_par(p) : -1;
			cerr << i << ' ' << a << ',' << b << ' ' << comps[i] << '\n';
		}
		for (int i = 0; i < 8; i += 2) {
			if (comps[i] == -1) continue;
			if (comps[i] == comps[(i+2)%8]) {
				if (comps[(i+1)%8] == comps[i]) {
					// we're okay
				} else {
					assert(comps[(i+1)%8] == -1);
					int num_same = 0;
					for (int c : comps) num_same += (c == comps[i]);
					if (num_same == 7) {
						// we're okay
						goto good;
					} else {
						cerr << "bad cycle at " << x << ' ' << y << '\n';
						return false;
					}
				}
			}

			if (comps[i] == comps[(i+4)%8]) {
				if (comps[(i+2)%8] == comps[i] || comps[(i+6)%8] == comps[i]) {
					// we're okay, and the above checks are enough
				} else {
					cerr << "bad oppo at " << x << ' ' << y << '\n';
					return false;
				}
			}
		}
good:
		filled[v(x, y)] = true;
		for (int i = 0; i < 8; i += 2) {
			auto [a, b] = neighbors[i];
			int p = v(a, b);
			if (filled[p]) {
				merge(p, v(x, y));
			}
		}
		return true;
	};

	std::string ans(Q, '?');
	for (int q = 0; q < Q; q++) {
		int x, y; cin >> x >> y;
		ans[q] = '0' + op(x, y);
	}
	cout << ans << '\n';

	return 0;
}
