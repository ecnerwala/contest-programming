#include <bits/stdc++.h>

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
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::string> G(N);
	for (auto& s : G) cin >> s;
	auto get_score = [&]() -> int {
		int ans = 0;
		auto check_pair = [&](char a, char b) -> void {
			ans += (a == 'O' && b == 'O') || (a == 'M' && b == 'P') || (a == 'P' && b == 'M');
		};
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if ('A' <= G[i][j] && G[i][j] <= 'Z') ans++;
				if (i+1 < N) check_pair(G[i][j], G[i+1][j]);
				if (j+1 < N) check_pair(G[i][j], G[i][j+1]);
				if (i+1 < N && j+1 < N) check_pair(G[i][j], G[i+1][j+1]);
				if (i+1 < N && j+1 < N) check_pair(G[i+1][j], G[i][j+1]);
			}
		}
		return ans;
	};

	int orig = get_score();
	for (int x0 = 0; x0 < N; x0++) {
		for (int y0 = 0; y0 < N; y0++) {
			if ('A' <= G[x0][y0] && G[x0][y0] <= 'Z') {
				std::y_combinator([&](auto self, int x, int y) -> void {
					char a = G[x][y];
					assert('A' <= a && a <= 'Z');
					for (int nx = std::max(0, x-1); nx <= std::min(N-1, x+1); nx++) {
						for (int ny = std::max(0, y-1); ny <= std::min(N-1, y+1); ny++) {
							char b = G[nx][ny];
							if ((a == 'O' && b == 'o') || (a == 'M' && b == 'p') || (a == 'P' && b == 'm')) {
								G[nx][ny] += 'A' - 'a';
								self(nx, ny);
							}
						}
					}
				})(x0, y0);
			}
		}
	}
	int final = get_score();
	cout << orig << ' ' << final << '\n';
	for (const auto& s : G) cout << s << '\n';


	return 0;
}
