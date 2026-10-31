#include <bits/stdc++.h>

std::optional<std::string> solve(std::string S) {
	int N = int(S.size());
	assert(N >= 3);
	std::ostringstream str;
	auto print_expr = [&](int lo, int hi) -> void {
		assert((hi - lo) & 1);
		str << S[lo];
		for (int i = lo+1; i < hi; i += 2) {
			str << '?' << S[i] << ':' << S[i+1];
		}
	};
	if (N >= 5 && S.back() == '1' && S[0] == '1' && S[1] == '0') {
		str << "(" << S[0] << "?" << S[1] << ":" << S[2] << ")";
		str << "?";
		str << "(";
		print_expr(3, N-1);
		str << ")";
		str << ':' << S[N-1];
		return str.str();
	} else if (S.back() == '1' && S[0] == '0') {
		str << S[0];
		str << '?';
		str << '(';
		print_expr(1, N-1);
		str << ')';
		str << ':';
		str << S[N-1];
		return str.str();
	} else {
		int i = 0;
		while (i < N && S[i] != '1') i += 2;
		if (i >= N) return std::nullopt;
		assert(i != N-1); // should've been caught
		int j = i+1;
		while (j < N && S[j] != '1') j += 2;
		if (j >= N) return std::nullopt;
		if (i > 0) {
			assert(S[0] == '0');
			str << S[0];
			str << '?';
			str << '(';
			print_expr(1, i);
			str << ')';
			str << ':';
		}
		str << S[i];
		str << '?';
		if (j > i+1) {
			str << '(';
			assert(S[i+1] == '0');
			str << S[i+1];
			str << '?';
			str << '(';
			print_expr(i+2, j);
			str << ')';
			str << ':';
			str << S[j];
			str << ')';
		} else {
			str << S[j];
		}
		str << ':';
		print_expr(j+1, N);
		return str.str();
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		N = 2 * N + 1;
		assert(N >= 3);
		std::string S; cin >> S;
		assert(int(S.size()) == N);
		auto res = solve(S);
		if (res) cout << "Yes" << '\n' << *res << '\n';
		else cout << "No" << '\n';
	}

	return 0;
}
