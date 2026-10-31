#include <bits/stdc++.h>

std::optional<std::string> solve(std::string S) {
	int num_hyphen = 0;
	int num_digit = 0;
	int sum = 0;
	for (char c : S) {
		if (c == 'X') {
			if (num_digit != 9) return std::nullopt;
			sum += (10 - num_digit) * 10;
			sum %= 11;
			num_digit++;
		} else if ('0' <= c && c <= '9') {
			sum += (10 - num_digit) * (c - '0');
			sum %= 11;
			num_digit++;
		} else if (c == '-') {
			num_hyphen++;
		} else {
			return std::nullopt;
		}
	}
	if (num_digit != 10) return std::nullopt;
	if (num_hyphen > 3) return std::nullopt;
	if (S.front() == '-') return std::nullopt;
	if (S.back() == '-') return std::nullopt;
	for (int i = 0; i+1 < int(S.size()); i++) {
		if (S[i] == '-' && S[i+1] == '-') {
			return std::nullopt;
		}
	}
	if (num_hyphen == 3 && S[S.size() - 2] != '-') {
		return std::nullopt;
	}
	if (sum != 0) return std::nullopt;

	std::string res = "978-" + S;
	res.pop_back();
	int new_sum = 0;
	int cur_w = 1;
	for (char c : res) {
		if (c == '-') continue;
		assert('0' <= c && c <= '9');
		new_sum += (c - '0') * cur_w;
		new_sum %= 10;
		cur_w ^= 2;
	}
	assert(cur_w == 1);
	res += char('0' + (new_sum * 9 % 10));
	return res;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		std::string S; cin >> S;
		auto ans = solve(S);
		if (ans) cout << *ans << '\n';
		else cout << "invalid" << '\n';
	}

	return 0;
}
