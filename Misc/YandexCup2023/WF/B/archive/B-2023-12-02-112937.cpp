#include <bits/stdc++.h>

using bs = std::bitset<1 << 18>;

std::vector<bs> base_terms;

bs parse_or(std::string::iterator& it);
bs parse_term(std::string::iterator& it) {
	if (*it == '(') {
		++it;
		auto res = parse_or(it);
		assert(*it == ')');
		++it;
		return res;
	} else if ('a' <= *it && *it <= 'z') {
		return base_terms[*it - 'a'];
	} else assert(false);
}
bs parse_not(std::string::iterator& it) {
	if (*it == '!') {
		++it;
		return ~parse_not(it);
	} else {
		return parse_term(it);
	}
}
bs parse_and(std::string::iterator& it) {
	auto lhs = parse_not(it);
	while (true) {
		if (*it == ')' || *it == '|') {
			return lhs;
		} else if (*it == '&') {
			++it;
			lhs &= parse_not(it);
		} else assert(false);
	}
}
bs parse_or(std::string::iterator& it) {
	auto lhs = parse_and(it);
	while (true) {
		if (*it == ')') {
			return lhs;
		} else if (*it == '|') {
			++it;
			lhs |= parse_and(it);
		} else assert(false);
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	using bs = bitset<1 << 18>;
	std::string S; cin >> S;
	S += ')';

	int Q; cin >> Q;
	while (Q--) {
	}

	return 0;
}
