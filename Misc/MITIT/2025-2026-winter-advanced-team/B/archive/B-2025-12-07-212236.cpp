#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	auto solve = [&](int X, int Y, int Z) -> std::optional<std::string> {
		if (X+Y+Z == 0) return "";
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
			std::reverse(vs.begin(), vs.end());
			while (Y >= 2 || vs[0].first - vs[1].first > 1) {
				if (Y == 0) {
					return std::nullopt;
				}
				if (vs[0].first <= 1) return std::nullopt;
				vs[0].first--;
				Y--;
				std::sort(vs.begin(), vs.end());
				std::reverse(vs.begin(), vs.end());
			}
			std::string s; s.reserve(X+Y+Z);
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

	auto is_good = [&](std::string s) -> bool {
		for (int i = 0; i+1 < int(s.size()); i++) {
			if (s[i] == s[i+1]) return false;
		}
		for (int i = 0; i+2 < int(s.size()); i++) {
			if (s[i+1] == 'I' && s[i] != s[i+2]) return false;
		}
		return true;
	};
	for (int x = 0; x <= 5; x++) {
		for (int y = 0; y <= 5; y++) {
			for (int z = 0; z <= 5; z++) {
				auto r = solve(x, y, z);
				if (r) {
					auto s = *r;
					cerr << x << ' ' << y << ' ' << z << '\n';
					cerr << s << '\n';
					int a = 0, b = 0, c = 0;
					for (char v : s) {
						if (v == 'M') a++;
						else if (v == 'I') b++;
						else if (v == 'T') c++;
						else assert(false);
					}
					assert(a == x && b == y && c == z);
					assert(is_good(s));
				} else {
					std::string s;
					s.insert(s.end(), x, 'M');
					s.insert(s.end(), y, 'I');
					s.insert(s.end(), z, 'T');
					std::sort(s.begin(), s.end());
					do {
						assert(!is_good(s));
					} while (std::next_permutation(s.begin(), s.end()));
				}
			}
		}
	}

	int T; cin >> T;
	while (T--) {
		int X, Y, Z; cin >> X >> Y >> Z;
		auto res = solve(X, Y, Z);
		if (res) { cout << "YES" << '\n' << *res << '\n'; }
		else cout << "NO" << '\n';
	}

	return 0;
}
