#include <bits/stdc++.h>

std::string S;
int idx;

using res_t = std::tuple<int64_t, int64_t, bool>;

res_t parse_expr();

int64_t parse_number() {
	assert('1' <= S[idx] && S[idx] <= '9');
	int64_t res = S[idx] - '0';
	idx++;
	while ('0' <= S[idx] && S[idx] <= '9') {
		res *= 10;
		res += S[idx] - '0';
		idx++;
	}
	return res;
}

res_t parse_factor() {
	res_t cur;
	if (S[idx] == 'N') {
		idx++;
		cur = {1, 0, 0};
	} else if (S[idx] == 'l') {
		idx++;
		assert(S[idx] == 'o');
		idx++;
		assert(S[idx] == 'g');
		idx++;
		assert(S[idx] == '(');
		idx++;
		auto [a, b, e] = parse_expr();
		if (a) {
			cur = {0, 1, 0};
		} else if (b) {
			cur = {0, 0, 1};
		} else {
			cur = {0, 0, 1};
		}
		assert(S[idx] == ')');
		idx++;
	} else if (S[idx] == '(') {
		idx++;
		cur = parse_expr();
		assert(S[idx] == ')');
		idx++;

		assert(S[idx] != '^');
	} else assert(false);

	if (S[idx] == '^') {
		idx++;
		int64_t v = parse_number();
		std::get<0>(cur) *= v;
		std::get<1>(cur) *= v;
	}
	return cur;
}

res_t parse_term() {
	auto res = parse_factor();
	while (S[idx] == '*') {
		idx++;
		auto o = parse_factor();
		std::get<0>(res) += std::get<0>(o);
		std::get<1>(res) += std::get<1>(o);
		std::get<2>(res) += std::get<2>(o);
	}
	return res;
}

res_t parse_expr() {
	auto res = parse_term();
	while (S[idx] == '+') {
		idx++;
		auto o = parse_term();
		res = std::max(res, o);
	}
	return res;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	cin >> S;
	S += ')';
	idx = 0;
	auto [a, b, eps] = parse_expr();
	cout << a << ' ' << b << '\n';

	return 0;
}
