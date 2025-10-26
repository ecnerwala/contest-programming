#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int L; cin >> L;
		std::vector<int> A(L);
		for (auto& a : A) cin >> a;
		std::vector<int> B(L);
		for (auto& b : B) cin >> b;
		auto res = [&]() -> std::optional<std::vector<std::array<int, 4>>> {
			{
				int s = std::accumulate(A.begin(), A.end(), 0);
				if (s > L-s) {
					for (auto& a : A) a ^= 1;
					for (auto& b : B) b ^= 1;
				}
			}
			int M = std::accumulate(A.begin(), A.end(), 0);
			assert(2 * M <= L);
			if (std::accumulate(B.begin(), B.end(), 0) != M) {
				return std::nullopt;
			}
			std::vector<int> vA; vA.reserve(M);
			{
				int cnt_0 = 0;
				for (auto a : A) {
					if (a == 0) cnt_0++;
					else if (a == 1) vA.push_back(cnt_0);
					else assert(false);
				}
			}
			assert(int(vA.size()) == M);
			std::vector<int> vB; vB.reserve(M);
			{
				int cnt_0 = 0;
				for (auto b : B) {
					if (b == 0) cnt_0++;
					else if (b == 1) vB.push_back(cnt_0);
					else assert(false);
				}
			}
			assert(int(vB.size()) == M);
			if (std::accumulate(vA.begin(), vA.end(), int64_t(0)) != std::accumulate(vB.begin(), vB.end(), int64_t(0))) {
				return std::nullopt;
			}
			std::vector<std::array<int, 4>> ops; ops.reserve(M);
			std::vector<int> decrs;
			std::vector<int> incrs;
			for (int i = 0; i < M; i++) {
				if (vB[i] > vA[i]) {
					incrs.push_back(i);
				} else if (vB[i] < vA[i]) {
					decrs.push_back(i);
				}
			}
			std::reverse(decrs.begin(), decrs.end());
			while (true) {
				assert(incrs.empty() == decrs.empty());
				if (incrs.empty()) break;
				int in = incrs.back();
				int dn = decrs.back();
				assert(vB[in] > vA[in]);
				assert(vB[dn] < vA[dn]);
				int delta = std::min(vB[in] - vA[in], vA[dn] - vB[dn]);

				assert(in+1 == M || vA[in] + delta <= vA[in+1]);
				assert(dn == 0 || vA[dn] - delta >= vA[dn-1]);

				std::array<int, 2> ipos{in + vA[in], in + vA[in] + delta + 1};
				std::array<int, 2> dpos{dn + vA[dn] - delta, dn + vA[dn] + 1};
				if (ipos > dpos) std::swap(ipos, dpos);
				ops.push_back({ipos[0], ipos[1], dpos[0], dpos[1]});

				vA[in] += delta;
				vA[dn] -= delta;
				if (vA[in] == vB[in]) incrs.pop_back();
				if (vA[dn] == vB[dn]) decrs.pop_back();
			}
			return ops;
		}();
		if (res) {
			cout << "Yes" << '\n';
			cout << res->size() << '\n';
			for (auto v : *res) {
				cout << v[0]+1 << ' ' << v[1] << ' ' << v[2]+1 << ' ' << v[3] << '\n';
			}
		} else {
			cout << "No" << '\n';
		}
	}

	return 0;
}
