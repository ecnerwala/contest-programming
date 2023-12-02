#include <bits/stdc++.h>

using bs = std::bitset<1 << 18>;

std::vector<bs> base_terms;
int N;
std::string S;

bs parse_or(std::string::iterator& it);
bs parse_term(std::string::iterator& it) {
	if (*it == '(') {
		++it;
		auto res = parse_or(it);
		assert(*it == ')');
		++it;
		return res;
	} else if ('a' <= *it && *it <= 'z') {
		return base_terms[N - 1 - (*(it++) - 'a')];
	} else assert(false);
}
bs parse_not(std::string::iterator& it) {
	bool flip = false;
	while (*it == '!') {
		++it;
		flip = !flip;
	}
	if (flip) return ~parse_term(it);
	else return parse_term(it);
}
bs parse_and(std::string::iterator& it) {
	std::cerr << "parse_and " << std::string(it, S.end()) << '\n';
	auto lhs = parse_not(it);
	std::cerr << "parse_and " << std::string(it, S.end()) << '\n';
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

	cin >> N;
	cin >> S;
	S += ')';
	base_terms.resize(N);
	for (int i = 0; i < N; i++) {
		for (int msk = 0; msk < (1 << N); msk++) {
			base_terms[i][msk] = (msk >> i) & 1;
		}
	}

	auto it = S.begin();
	bs values = parse_or(it);
	for (int m = 0; m < (1 << N); m++) {
		for (int i = N-1; i >= 0; i--) {
			cerr << ((m >> i) & 1);
		}
		cerr << ' ' << values[m] << '\n';
	}
	assert(it + 1 == S.end());
	for (int i = 0; i < N; i++) {
		bs zeros = base_terms[i] >> (1 << i);
		zeros &= values;
		bs ones = base_terms[i];
		ones &= values;
		values = (zeros & (ones >> (1 << i))) | (ones | (zeros << (1 << i)));
	}
	std::vector<int> goods;
	goods.reserve(1 << N);
	for (int i = 0; i < N; i++) {
		if (values[i]) {
			goods.push_back(i);
		}
	}

	int Q; cin >> Q;
	while (Q--) {
		int k; cin >> k; k--;
		for (int i = N-1; i >= 0; i--) {
			cout << ((goods[k] & (1 << i)) ? 'E' : 'A') << char('a' + (N-1-i));
		}
		cout << '\n';
	}

	return 0;
}
