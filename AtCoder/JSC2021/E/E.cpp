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

std::optional<std::pair<int, std::string>> solve(std::vector<std::array<int, 26>> freq, int K) {
	int L = int(freq.size());
	if (K == 0) {
		if (!L) return {{0, ""}};
		if (L == 1) return std::nullopt;
		// gotta make sure it's not a palindrome
		int cost = 0;
		std::string res(L, '\0');
		for (int i = 0; i < L; i++) {
			int c = int(std::max_element(freq[i].begin(), freq[i].end()) - freq[i].begin());
			cost += std::accumulate(freq[i].begin(), freq[i].end(), 0) - freq[i][c];
			res[i] = char(c + 'a');
		}
		if (res == std::string(res.rbegin(), res.rend())) {
			// gotta flip one
			int best_flip = -1;
			int best_flip_v = -1;
			int best_flip_cost = 0;
			for (int i = 0; i < L; i++) {
				if (i == L-1-i) continue;

				int c = res[i] - 'a';
				for (int v = 0; v < 26; v++) {
					if (c == v) continue;
					int cnd = freq[i][c] - freq[i][v];
					if (best_flip == -1 || cnd < best_flip_cost) {
						best_flip = i;
						best_flip_v = v;
						best_flip_cost = cnd;
					}
				}
			}
			cost += best_flip_cost;
			res[best_flip] = char(best_flip_v + 'a');
		}
		return {{cost, res}};
	}

	if (!L) return std::nullopt;
	std::vector<std::array<int, 26>> nfreq(freq.begin(), freq.begin() + L/2);
	for (int i = 0; i < L/2; i++) {
		for (int v = 0; v < 26; v++) {
			nfreq[i][v] += freq[L-1-i][v];
		}
	}
	auto nres = solve(nfreq, K-1);
	if (!nres) return std::nullopt;

	if (L & 1) {
		// midpoint character
		int md = L / 2;
		int c = int(std::max_element(freq[md].begin(), freq[md].end()) - freq[md].begin());
		nres->first += std::accumulate(freq[md].begin(), freq[md].end(), 0) - freq[md][c];
		nres->second += char(c + 'a') + std::string(nres->second.rbegin(), nres->second.rend());
	} else {
		nres->second += std::string(nres->second.rbegin(), nres->second.rend());
	}
	return nres;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int K; cin >> K;
	string S; cin >> S;
	vector<array<int, 26>> freq(S.size());
	for (int i = 0; i < int(S.size()); i++) {
		freq[i].fill(0);
		freq[i][S[i]-'a']++;
	}
	auto res = solve(freq, K);
	if (res) {
		cout << res->first << '\n';
		//cout << res->second << '\n';
	} else {
		cout << "impossible" << '\n';
	}

	return 0;
}
