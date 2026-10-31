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

using bs = std::bitset<1024>;

std::pair<bs, int> parse(std::string S) {
	std::array<bs, 10> base_var;
	for (int i = 0; i < 10; i++) {
		for (int m = 0; m < 1024; m++) {
			base_var[i][m] = (m >> i) & 1;
		}
	}
	int cur_idx = 0;
	int has_var = 0;
	bs v = std::y_combinator([&](auto self) -> bs {
		assert(cur_idx < int(S.size()));
		if (S[cur_idx] == '(') {
			cur_idx++;
			assert(cur_idx < int(S.size()));
			if (S[cur_idx] == 'm') {
				cur_idx++;
				assert(cur_idx < int(S.size()));

				bool is_min = S[cur_idx] == 'i';
				if (is_min) {
					assert(S[cur_idx] == 'i');
					cur_idx++;
					assert(cur_idx < int(S.size()));

					assert(S[cur_idx] == 'n');
					cur_idx++;
					assert(cur_idx < int(S.size()));
				} else {
					assert(S[cur_idx] == 'a');
					cur_idx++;
					assert(cur_idx < int(S.size()));

					assert(S[cur_idx] == 'x');
					cur_idx++;
					assert(cur_idx < int(S.size()));
				}

				assert(S[cur_idx] == ' ');
				cur_idx++;
				assert(cur_idx < int(S.size()));

				bs l = self();
				assert(cur_idx < int(S.size()));

				assert(S[cur_idx] == ' ');
				cur_idx++;
				assert(cur_idx < int(S.size()));

				bs r = self();
				assert(cur_idx < int(S.size()));

				assert(S[cur_idx] == ')');
				cur_idx++;

				if (is_min) {
					return l & r;
				} else {
					return l | r;
				}
			} else {
				bs l = self();
				assert(cur_idx < int(S.size()));

				assert(S[cur_idx] == ' ');
				cur_idx++;
				assert(cur_idx < int(S.size()));

				assert(S[cur_idx] == '<');
				cur_idx++;
				assert(cur_idx < int(S.size()));

				bool allow_eq = false;
				if (S[cur_idx] == '=') {
					allow_eq = true;
					cur_idx++;
					assert(cur_idx < int(S.size()));
				}

				assert(S[cur_idx] == ' ');
				cur_idx++;
				assert(cur_idx < int(S.size()));

				bs r = self();
				assert(cur_idx < int(S.size()));

				assert(S[cur_idx] == ')');
				cur_idx++;

				if (allow_eq) {
					return (~l) | r;
				} else {
					return (~l) & r;
				}
			}
		} else {
			char c = S[cur_idx];
			cur_idx++;
			assert('a' <= c && c <= 'j');
			has_var |= 1 << (c-'a');
			return base_var[c - 'a'];
		}
	})();
	assert(cur_idx == int(S.size()));
	return {v, has_var};
}

std::optional<std::string> build(std::pair<bs, int> inp) {
	auto [v, full_msk] = inp;
	if (!v[full_msk]) return std::nullopt;

	std::string res;

	std::y_combinator([&](auto self, int unused, int cur_msk, int cur_neg, bool flip) -> void {
		if (!unused) {
			bool cur_val = v[cur_msk] ^ flip;
			if (cur_val) {
				// return true
				res += "(";
				res += char('a' + cur_neg);
				res += " <= ";
				res += char('a' + cur_neg);
				res += ")";
			} else {
				// return false
				res += char('a' + cur_neg);
			}
			return;
		}

		int b = __builtin_ctz(unused);
		unused -= (1 << b);
		res += "((";
		self(unused, cur_msk + (0 << b), b, !flip);
		res += " <= ";
		res += char('a' + b);
		res += ") <= (min ";
		res += char('a' + b);
		res += " ";
		self(unused, cur_msk + (1 << b), cur_neg, flip);
		res += "))";
	})(full_msk, 0, __builtin_ctz(full_msk), false);

	return std::nullopt;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::string S; std::getline(cin, S);
	auto res = parse(S);
	auto my_res = build(res);
	if (my_res) {
		cout << "YES" << '\n';
		cout << *my_res << '\n';
		assert(parse(*my_res) == res);
	} else {
		cout << "NO" << '\n';
	}

	return 0;
}
