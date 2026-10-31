#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	// In order to maximize the concatenation, we first want the length to be as long as possible.
	// To maximize length, you can check the configuration where all lengths differ by at most one
	std::string N; cin >> N;
	int K; cin >> K;
	if (K == 1) {
		cout << N << '\n';
		exit(0);
	}
	auto length_good = [&](int l) -> bool {
		assert(l > K);
		int p10 = l / K;
		assert(p10 >= 1);
		int extra = l % K;
		int v = extra * 10 + (K - extra) * (p10 == 1 ? 0 : 1);
		assert(v > 0);
		std::string cnd(p10-1, '0');
		for (int cur = v; cur > 0; cur /= 10) cnd += char('0' + cur % 10);
		std::reverse(cnd.begin(), cnd.end());
		if (cnd.size() != N.size()) return cnd.size() > N.size();
		else return cnd <= N;
	};
	int mi = K, ma = int(N.size()) * K + 1;
	while (ma - mi > 1) {
		int md = (mi + ma) / 2;
		if (length_good(md)) {
			mi = md;
		} else {
			ma = md;
		}
	}
	int l = mi;
	cerr << "using length " << l << '\n';
	std::string best;
	std::vector<std::string> ans;
	for (int l0 = (mi + K-1) / K + 1; l0 >= 1 && l0 >= mi / K - 1; l0--) {
		cerr << "trying " << l0 << '\n';
		int l_rem = l - l0;
		if (l_rem < K-1) continue;
		int p10 = l_rem / (K-1);
		int extra = l_rem % (K-1);
		int v = extra * 10 + (K-1 - extra) * (p10 == 1 ? 0 : 1);
		std::string cnd(p10-1, '0');
		for (int cur = v; cur > 0; cur /= 10) cnd += char('0' + cur % 10);
		std::reverse(cnd.begin(), cnd.end());
		if (cnd.size() > N.size()) continue;
		cnd.resize(N.size(), '0');
		if (cnd > N) continue;
		std::string v0(N.size(), '?');
		int carry = 0;
		for (int i = int(N.size())-1; i >= 0; i--) {
			int d = (N[i] - '0') - (cnd[i] - '0') - carry;
			carry = d < 0;
			d += carry * 10;
			assert(0 <= d && d < 10);
			v0[i] = char('0' + d);
		}
		assert(!carry);
		std::reverse(v0.begin(), v0.end());
		while (v0.size() > 1 && v0.back() == '0') v0.pop_back();
		std::reverse(v0.begin(), v0.end());
		cerr << "got " << v0 << '\n';
		assert(int(v0.size()) <= l0);
		if (int(v0.size()) < l0) continue;
		if (v0 == "0") continue;
		cerr << "hi " << '\n';
		std::vector<std::string> n_ans; n_ans.reserve(K);
		n_ans.push_back(v0);
		if (p10 != 1) {
			std::string tmp(p10, '0');
			tmp[0] = '1';
			for (int z = 0; z < K-1-extra; z++) n_ans.push_back(tmp);
		}
		{
			std::string tmp(p10 + 1, '0');
			tmp[0] = '1';
			for (int z = 0; z < extra; z++) n_ans.push_back(tmp);
		}
		if (p10 == 1) {
			std::string tmp(p10, '0');
			for (int z = 0; z < K-1-extra; z++) n_ans.push_back(tmp);
		}
		assert(int(n_ans.size()) == K);
		std::string n_best; n_best.reserve(l);
		for (const std::string& s : n_ans) n_best += s;
		assert(int(n_best.size()) == l);
		if (ans.empty() || n_best > best) {
			best = n_best;
			n_ans = ans;
		}
	}
	assert(!ans.empty());
	for (int i = 0; i < K; i++) {
		cout << ans[i] << " \n"[i+1==K];
	}

	return 0;
}
