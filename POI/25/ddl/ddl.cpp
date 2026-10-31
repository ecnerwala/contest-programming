#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }
template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int INF = int(1e9) + 200;
	struct rng_t {
		int mi, ma;
	};
	map<int, array<rng_t, 2>> mp_;
	mp_[0] = {rng_t{0, 0}, {0, 0}};
	int L = INF;
	for (int z = 0; z < 2; z++) {
		int N; cin >> N;
		vector<int> V(N);
		{
			int curL = 0;
			for (auto& v : V) {
				char c; int l; cin >> c >> l;
				if (c == 'T') v = +l;
				else if (c == 'W') v = -l;
				else assert(false);
				curL += l;
			}
			assert(curL < INF);
			setmin(L, curL);
		}

		for (int i = 0; i < N; i++) {
			int l = 0, v = 0;
			for (int j = i; j < N; j++) {
				l += abs(V[j]);
				v += V[j];
				auto& s = mp_.insert({l, {rng_t{INF, -INF}, {INF, -INF}}}).first->second;
				setmin(s[z].mi, v);
				setmax(s[z].ma, v);
			}
		}
	}

	vector<pair<int, array<rng_t, 2>>> mp(mp_.begin(), mp_.end());

	for (auto it = mp.begin(), jt = next(it); jt != mp.end(); it = jt, ++jt) {
		int d = jt->first - it->first;
		assert(d > 0);
		for (int z = 0; z < 2; z++) {
			setmin(jt->second[z].mi, it->second[z].mi + d);
			setmax(jt->second[z].ma, it->second[z].ma - d);
		}
	}
	for (auto it = mp.rbegin(), jt = next(it); jt != mp.rend(); it = jt, ++jt) {
		int d = it->first - jt->first;
		assert(d > 0);
		for (int z = 0; z < 2; z++) {
			setmin(jt->second[z].mi, it->second[z].mi + d);
			setmax(jt->second[z].ma, it->second[z].ma - d);
		}
	}

	/*
	for (auto it : mp) {
		cerr << it.first << ' ' << it.second[0].mi << ".." << it.second[0].ma << ' ' << it.second[1].mi << ".." << it.second[1].ma << '\n';
	}
	*/

	while (mp.back().first > L) mp.pop_back();

	const auto& bk = mp.back();
	if (bk.second[0].mi > bk.second[1].ma) {
		for (auto it = mp.rbegin(); it != mp.rend(); ++it) {
			if (it->second[0].mi <= it->second[1].ma) {
				cout << it->first + (it->second[1].ma - it->second[0].mi) / 2 << '\n';
				exit(0);
			}
		}
	} else if (bk.second[1].mi > bk.second[0].ma) {
		for (auto it = mp.rbegin(); it != mp.rend(); ++it) {
			if (it->second[1].mi <= it->second[0].ma) {
				cout << it->first + (it->second[0].ma - it->second[1].mi) / 2 << '\n';
				exit(0);
			}
		}
	} else {
		cout << L << '\n';
		exit(0);
	}


	return 0;
}
