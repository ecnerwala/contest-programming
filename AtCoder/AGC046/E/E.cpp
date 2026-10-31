#include<bits/stdc++.h>

template <typename T> void setmax(T& a, const T& b) { if (b > a) a = b; }
template <typename T> void setmin(T& a, const T& b) { if (b < a) a = b; }

struct interval {
	int lo, hi;
	interval() : lo(0), hi(0) {}
	interval(int lo_, int hi_) : lo(lo_), hi(hi_) {}
	interval& operator &= (const interval& o) {
		setmax(lo, o.lo);
		setmin(hi, o.hi);
		return *this;
	}
	friend interval operator & (const interval& a, const interval& b) {
		return interval(std::max(a.lo, b.lo), std::min(a.hi, b.hi));
	}
	explicit operator bool () const {
		return lo <= hi;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int K; cin >> K;
	vector<int> A(K);
	for (int i = 0; i < K; i++) cin >> A[i];
	if (*min_element(A.begin(), A.end()) * 2 < *max_element(A.begin(), A.end())) {
		cout << -1 << '\n';
		exit(0);
	}

	int L = accumulate(A.begin(), A.end(), 0);
	vector<int> res; res.reserve(L);
	auto is_good = [&K, &res, &A]() -> bool {
		vector<bool> seen(K);
		int N = int(res.size());
		int st = N;
		while (st > 0 && !seen[res[st-1]]) {
			--st;
			seen[res[st]] = true;
		}

		int en;
		if (N - st == K) {
			en = N;
		} else if (st == 0) {
			en = 0;
		} else {
			int l = st, r = N;
			while (r - l < K) {
				if (l == 0) {
					return false;
				} else if (seen[res[l-1]]) {
					seen[res[--r]] = false;
					if (r < st) return false;
				} else {
					seen[res[--l]] = true;
				}
			}
			en = r;
		}

		seen.assign(K, 0);
		for (int i = st; i < N; i++) seen[res[i]] = true;

		auto can_make = [&](interval base, vector<pair<interval, interval>> opts) {
			vector<interval> pref(opts.size()+1);
			pref[0] = base;
			for (int i = 0; i < int(opts.size()); i++) {
				pref[i+1] = pref[i] & opts[i].first;
			}
			interval suff = base;
			if (pref.back()) return true;
			for (int i = int(opts.size())-1; i >= 0; i--) {
				suff &= opts[i].second;
				if (pref[i] & suff) {
					return true;
				}
			}
			return false;
		};

		{
			interval fixed(0, int(1e9));
			for (int v = 0; v < K; v++) {
				if (!seen[v]) {
					fixed &= interval(A[v], 2*A[v]);
				}
			}
			for (int i = en; i < N; i++) {
				int v = res[i];
				fixed &= interval(A[v]+1, 2*A[v]+1);
			}
			vector<pair<interval, interval>> opts; opts.reserve(en-st);
			for (int i = st; i < en; i++) {
				int v = res[i];
				opts.push_back({{A[v],2*A[v]}, {A[v]+1, 2*A[v]+1}});
			}
			if (can_make(fixed, opts)) return true;
		}

		{
			interval fixed(0, int(1e9));
			for (int v = 0; v < K; v++) {
				if (!seen[v]) {
					fixed &= interval(A[v]+1, 2*A[v]);
				}
			}
			vector<pair<interval, interval>> opts; opts.reserve(N-st);
			for (int i = st; i < N; i++) {
				int v = res[i];
				opts.push_back({{A[v]+1, 2*A[v]+1}, {A[v]+2, 2*A[v]+2}});
			}
			if (can_make(fixed, opts)) return true;
		}

		return false;
	};

	for (int curL = 1; curL <= L; curL++) {
		for (int i = 0; i < K; i++) {
			if (A[i]) {
				A[i]--;
				res.push_back(i);
				if (is_good()) {
					break;
				}
				res.pop_back();
				A[i]++;
			}
		}
		assert(int(res.size()) == curL);
	}

	for (int z = 0; z < L; z++) {
		cout << res[z]+1 << " \n"[z+1==L];
	}

	return 0;
}
