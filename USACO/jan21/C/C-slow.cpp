#include <bits/stdc++.h>
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
    ios_base::sync_with_stdio(false), cin.tie(nullptr);
    int N, M, Q; cin >> N >> M >> Q;
    vector<string> G(N);
    for (auto& v : G) { cin >> v; assert(int(v.size()) == M); }
    for (int q = 0; q < Q; q++) {
        int xlo, xhi, ylo, yhi; cin >> xlo >> ylo >> xhi >> yhi; xlo--, ylo--, xhi--, yhi--;
        int ans = 0;
        vector<string> tmp(G);
        const int dx[4] = {-1, 0, 1, 0};
        const int dy[4] = {0, 1, 0, -1};
        for (int x = xlo; x <= xhi; x++) {
            for (int y = ylo; y <= yhi; y++) {
                if (tmp[x][y]) {
                    ans++;
                    std::y_combinator([&](auto self, int x, int y, char c) -> void {
                        tmp[x][y] = 0;
                        for (int d = 0; d < 4; d++) {
                            int nx = x + dx[d], ny = y + dy[d];
                            if (nx < xlo || nx > xhi || ny < ylo || ny > yhi) continue;
                            if (tmp[nx][ny] != c) continue;
                            self(nx, ny, c);
                        }
                    })(x,y,tmp[x][y]);
                }
            }
        }
        cout << ans << '\n';
    }
}
