#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	auto solve = [&](int X, int Y, int Z) -> std::optional<std::string> {
		assert(X+Y+Z >= 1);
		if (X + Y + Z == 1) {
			std::string s;
			for (int z = 0; z < X; z++) s += 'M';
			for (int z = 0; z < Y; z++) s += 'I';
			for (int z = 0; z < Z; z++) s += 'T';
			return s;
		} else if ((X == 0) + (Y == 0) + (Z == 0) == 2) {
			return std::nullopt;
		} else if ((X == 0) + (Y == 0) + (Z == 0) == 1) {
			std::vector<std::pair<int, char>> vals;
			vals.push_back({X, 'M'});
			vals.push_back({Y, 'I'});
			vals.push_back({Z, 'T'});
			std::sort(vals.begin(), vals.end());
			std::reverse(vals.begin(), vals.end());
			assert(vals.back().first == 0);
			vals.pop_back();
			assert(vals[0].first - vals[1].first >= 0);
			if (vals[0].first - vals[1].first > 1) {
				return nullopt;
			} else {
				std::string s; s.reserve(X+Y+Z);
				for (int z = 0; z < vals[0].first + vals[1].first; z++) {
					s += vals[z&1].second;
				}
				return s;
			}
		} else {
			// all nonzero
			// can start/end with I
			// need to kill some number of M's/T's
			std::vector<std::pair<int, char>> vs;
			vs.push_back({X, 'M'});
			vs.push_back({Z, 'T'});
			std::sort(vs.begin(), vs.end());
			while (Y >= 2 || vs[0].first - vs[1].first > 1) {
				if (Y == 0) {
					return std::nullopt;
				}
				if (vs[0].first <= 1) return std::nullopt;
				vs[0].first--;
				Y--;
				std::sort(vs.begin(), vs.end());
			}
			std::string s; cin >> s;
			if (Y > 0) {
				s += 'I';
				Y--;
			}
			for (int z = 0; z < vs[0].first + vs[1].first; z++) {
				int& v = (vs[z&1].second == 'M' ? X : Z);
				s += vs[z&1].second;
				while (v > vs[z&1].first) {
					s += 'I';
					s += vs[z&1].second;
					v--;
				}
			}
			if (Y > 0) {
				s += 'I';
				Y--;
			}
			assert(Y == 0);
			return s;
		}
	};

	int T; cin >> T;
	while (T--) {
		int X, Y, Z; cin >> X >> Y >> Z;
		auto res = solve(X, Y, Z);
		if (res) { cout << "YES" << '\n' << *res << '\n'; }
		else cout << "NO" << '\n';
	}

	return 0;
}
