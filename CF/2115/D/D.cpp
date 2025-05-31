#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int64_t X = 0;
		std::vector<int64_t> A(N);
		for (auto& a : A) {
			cin >> a;
			X ^= a;
		}
		for (auto& a : A) {
			int64_t b; cin >> b;
			a ^= b;
		}
		std::string C; cin >> C;

		cerr << "X = " << X << '\n';

		std::vector<int64_t> basis(60);
		std::vector<char> value(60, '?');
		for (int i = N-1; i >= 0; i--) {
			int64_t v = A[i];
			cerr << v << ' ' << C[i] << '\n';
			for (int b = 59; b >= 0; b--) v = std::min(v, v ^ basis[b]);
			if (v) {
				int b = 63 - __builtin_clzll(v);
				assert(!basis[b]);
				basis[b] = v;
				value[b] = C[i];
				for (auto& o : basis) o = std::min(o, o ^ v);
			}
		}
		for (int b = 0; b < 60; b++) {
			cerr << b << ' ' << basis[b] << ' ' << value[b] << '\n';
			if (basis[b]) {
				assert(value[b] != '?');
				if (value[b] == '0') {
					X = std::min(X, X ^ basis[b]);
				} else if (value[b] == '1') {
					X = std::max(X, X ^ basis[b]);
				} else assert(false);
			}
		}
		cout << X << '\n';
	}

	return 0;
}
