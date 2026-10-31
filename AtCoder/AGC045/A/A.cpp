#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		vector<uint64_t> A(N); for (auto& a : A) cin >> a;
		string S; cin >> S;

		vector<uint64_t> basis; basis.reserve(64);
		bool ans = 0;
		for (int i = N-1; i >= 0; i--) {
			uint64_t v = A[i];
			for (uint64_t b : basis) {
				v = min(v, b ^ v);
			}
			if (v) {
				if (S[i] == '0') {
					basis.push_back(v);
				} else if (S[i] == '1') {
					ans = 1;
					break;
				} else assert(false);
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
