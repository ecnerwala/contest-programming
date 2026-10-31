#include <bits/stdc++.h>

std::optional<std::array<int, 2>> solve(int x, int y, int A, int B) {
	if (x == y) {
		return {{2, 2}};
	}
	assert(x != y);
	int diff = std::abs(x - y);
	for (int t = 1; t * (t+1) <= std::min(x, y); t++) {
		if (diff % t != 0) continue;
		int bx = std::min({x / t, A, B - (y - x) / t});
		int by = std::min({y / t, B, A - (x - y) / t});
		assert((bx - by) * t == x - y);
		if (bx < 2 || by < 2) continue;
		int u = x - t * bx;
		if (0 <= std::min(t, u) && std::max(t, u) < std::min(bx, by)) {
			return {{bx, by}};
		}
	}
	auto in_base = [&](int n, int b, std::vector<int>& res) -> void {
		res.clear();
		while (n > 0) {
			res.push_back(n % b);
			n /= b;
		}
		std::reverse(res.begin(), res.end());
	};
	int bx = 2;
	std::vector<int> vx;
	in_base(x, 2, vx);
	int by = 2;
	std::vector<int> vy;
	in_base(y, 2, vy);

	while (bx <= A && by <= B) {
		if (vx.size() <= 2 && vy.size() <= 2) {
			// TODO: Special stuff
			break;
		}
		if (vx.size() > vy.size()) {
			bx++;
			in_base(x, bx, vx);
		} else if (vy.size() > vx.size()) {
			by++;
			in_base(y, by, vy);
		} else {
			if (vx > vy) {
				bx++;
				in_base(x, bx, vx);
			} else if (vy > vx) {
				by++;
				in_base(y, by, vy);
			} else {
				return std::array<int, 2>{bx, by};
			}
		}
	}
	return std::nullopt;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int x, y, A, B; cin >> x >> y >> A >> B;
		auto ans = solve(x, y, A, B);
		if (ans) {
			cout << "YES" << '\n';
			cout << (*ans)[0] << ' ' << (*ans)[1] << '\n';
		} else {
			cout << "NO" << '\n';
		}
	}

	return 0;
}
