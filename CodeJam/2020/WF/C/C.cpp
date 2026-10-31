#include<bits/stdc++.h>

int hex_to_int(char c) {
	if ('0' <= c && c <= '9') return c - '0';
	else if ('A' <= c && c <= 'F') return (c - 'A') + 10;
	else assert(false);
}
char int_to_hex(int a) {
	if (0 <= a && a <= 9) return char('0' + a);
	else if (10 <= a && a <= 15) return char('A' + (a - 10));
	else assert(false);
}

std::array<int64_t, 17> fact;
int64_t dfs(int s, int e, std::array<int, 16> coeffs, int msk) {
	using namespace std;
	if (e < 0) return 0;
	if (s < 0) s = 0;
	sort(coeffs.begin(), coeffs.end(), std::greater<>());
	if (coeffs[0] == 0) {
		return s == 0 ? fact[16 - __builtin_popcount(msk)] : 0;
	}
	int min_val = 0;
	int max_val = 0;
	for (int m = (1 << 16) - 1 - msk, z = 0, y = 16 - 1 - __builtin_popcount(msk); m; m -= m & (-m), z++, y--) {
		min_val += __builtin_ctz(m) * coeffs[z];
		max_val += __builtin_ctz(m) * coeffs[y];
	}
	if (max_val < s || min_val > e) {
		return 0;
	}
	s = max(s, min_val);
	e = min(e, max_val);
	if (s == min_val && max_val == e) {
		return fact[16 - __builtin_popcount(msk)];
	}
	static map<tuple<int, int, array<int, 16>, int>, int64_t> mem;
	tuple<int, int, array<int, 16>, int> key(s, e, coeffs, msk);
	{ auto it = mem.find(key); if (it != mem.end()) return it->second; }

	int64_t ans = [&s, &e, &coeffs, &msk]() -> int64_t {
		int64_t res = 0;
		int c = coeffs[0];
		coeffs[0] = 0;
		for (int m = (1 << 16) - 1 - msk; m; m -= m & (-m)) {
			int v = __builtin_ctz(m);
			res += dfs(s - v * c, e - v * c, coeffs, msk | (1 << v));
		}
		return res;
	}();

	return mem[key] = ans;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	fact[0] = 1;
	for (int i = 1; i < int(fact.size()); i++) fact[i] = fact[i-1] * int64_t(i);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N, D; cin >> N >> D;
		using str = array<int, 6>;
		auto read_hex = [&]() -> str {
			str res; res.fill(0);
			string s; cin >> s;
			assert(int(s.size()) == D);
			for (int i = 0; i < D; i++) {
				res[D-1-i] = hex_to_int(s[i]);
			}
			return res;
		};
		auto str_to_val = [&](const str& s) -> int {
			int ans = 0;
			for (int i = 0; i < D; i++) {
				ans |= s[i] << (4 * i);
			}
			return ans;
		};

		int S = str_to_val(read_hex());
		int E = str_to_val(read_hex());
		
		vector<str> L(N);
		for (int i = 0; i < N; i++) {
			L[i] = read_hex();
		}

		int64_t num = 0;
		int64_t den = N * (N-1) / 2 * fact[16];
		for (int i = 0; i < N; i++) {
			for (int j = i+1; j < N; j++) {
				array<int, 16> coeffs; coeffs.fill(0);
				for (int z = 0; z < D; z++) {
					coeffs[L[i][z]] += (1 << (4 * z));
					coeffs[L[j][z]] += (1 << (4 * z));
				}
				num += dfs(S, E, coeffs, 0);
				num += dfs(S + (1 << (4 * D)), E + (1 << (4 * D)), coeffs, 0);
			}
		}

		int64_t g = __gcd(num, den);
		num /= g, den /= g;
		cout << "Case #" << case_num << ":" << ' ' << num << ' ' << den << '\n';
	}

	return 0;
}
