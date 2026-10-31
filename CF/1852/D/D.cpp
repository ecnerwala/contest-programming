#include <bits/stdc++.h>

using std::cerr;
std::optional<std::string> solve(std::string S, int K) {
	int N = int(S.size());
	//std::cerr << "solve " << S << ' ' << K << '\n';
	/*
	if (2 * K > N + (N-1) * 2) {
		std::string S2 = 0;
		for (int i = 0; i < N; i += 2) {
			S2[i] ^= 1;
		}
		auto res = solve(S2, N + (N-1) * 2 - K);
		if (res) {
			for (int i = 1; i < N; i += 2) {
				(*res)[i] ^= 1;
			}
		}
		return res;
	}
	assert(K <= N-1 + (N / 2));
	if (K == 0) {
		for (int i = 0; i+1 < N; i++) {
			if (S[i] != S[i+1]) return std::nullopt;
		}
		return S;
	}
	if (K == 1) {
		assert(N >= 2);
		return std::nullopt;
	}
	*/

	std::vector<std::array<std::vector<std::array<int, 2>>, 2>> can_make(N);
	auto normalize = [&](std::vector<std::array<int, 2>>& v) {
		std::sort(v.begin(), v.end());
		int L = 0;
		for (int i = 0; i < int(v.size()); i++) {
			if (L == 0 || v[i][0] > v[L-1][1] + 1) {
				v[L++] = v[i];
			} else {
				v[L-1][1] = std::max(v[L-1][1], v[i][1]);
			}
		}
		v.resize(L);
	};
	can_make[N-1][0].push_back({S[N-1] != 'A', S[N-1] != 'A'});
	can_make[N-1][1].push_back({S[N-1] != 'B', S[N-1] != 'B'});
	for (int i = N-2; i >= 0; i--) {
		for (int vi = 0; vi < 2; vi++) {
			can_make[i][vi].reserve(can_make[i+1][0].size() + can_make[i+1][1].size());
			for (int vj = 0; vj < 2; vj++) {
				int delta = (S[i] != S[i+1]) + (vi != vj) + (S[i] - 'A' != vi);
				for (auto [l, r] : can_make[i+1][vj]) {
					can_make[i][vi].push_back({l + delta, r + delta});
				}
			}
			normalize(can_make[i][vi]);
			//cerr << "dp " << i << ' ' << vi << ' ';
			//for (auto [l, r] : can_make[i][vi]) { cerr << l << '-' << r << ' '; }
			//cerr << '\n';
		}
	}
	int v0;
	for (v0 = 0; v0 < 2; v0++) {
		bool found = false;
		for (auto [l, r] : can_make[0][v0]) {
			if (l <= K && K <= r) {
				found = true;
				break;
			}
		}
		if (found) break;
	}
	if (v0 == 2) return std::nullopt;
	std::string res(N, '\0');
	res[0] = char('A' + v0);
	int vi = v0;
	int cur_val = 0;
	for (int i = 0; i < N-1; i++) {
		int vj;
		//std::cerr << i << ' ' << vi << '\n';
		for (vj = 0; vj < 2; vj++) {
			int delta = (S[i] != S[i+1]) + (vi != vj) + (S[i] - 'A' != vi);
			for (auto [l, r] : can_make[i+1][vj]) {
				if (l + cur_val + delta <= K && K <= r + cur_val + delta) {
					cur_val += delta;
					goto found;
				}
			}
		}
		assert(false);
found:
		vi = vj;
		res[i+1] = char('A' + vj);
	}
	return res;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::string S; cin >> S;
		auto res = solve(S, K);
		if (!res) {
			cout << "NO" << '\n';
		} else {
			cout << "YES" << '\n';
			cout << *res << '\n';
		}

		// start from the min, increment one at a time
	}

	return 0;
}
