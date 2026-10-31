#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 2.1e5;
int N, L;
int A[MAXN];

const int S = 1 << 18;
int seg[2 * S];
unordered_map<int, vector<int>> vals;

void precomp() {
	memset(seg, -1, sizeof(seg));
	for (int i = 0; i < N; i++) {
		seg[S + i] = A[i];
		vals[A[i]].push_back(i);
	}
	for (int i = S-1; i > 0; i--) {
		seg[i] = max(seg[2 * i], seg[2 * i + 1]);
	}
}

int query_max(int a, int b) {
	int res = -1;
	for (int l = a+S, r = b+S; l < r; l /= 2, r /= 2) {
		if (l & 1) {
			res = max(res, seg[l]);
			++l;
		}
		if (r & 1) {
			--r;
			res = max(res, seg[r]);
		}
	}
	return res;
}

ll ans = 0;

struct rangeData {
	int k = -1;
	int wholeValue = -1;
	vector<int> lPts = vector<int>({1}); // number of pts of each size
	vector<int> rPts = vector<int>({1}); // number of pts of each size
};

vector<int> upCast(vector<int> v) {
	assert(!v.empty());
	assert(v[0] == 1);
	vector<int> res((v.size() - 1) / L + 1);
	res[0] = v[0];
	for (int i = L; i < int(v.size()); i++) {
		res[i / L] += v[i];
	}
	return res;
}

rangeData upCast(rangeData r, int v) {
	assert(r.k < v);
	while (r.k < v) {
		if (r.lPts.size() == 1 && r.rPts.size() == 1) {
			assert(r.wholeValue == 0 || r.wholeValue == -1);
			// no more shrinking is possible;
			r.k = v;
			break;
		}
		assert(r.wholeValue != 0);
		if (r.wholeValue >= L) {
			r.wholeValue /= L;
		} else {
			r.wholeValue = -1;
		}
		r.lPts = upCast(std::move(r.lPts));
		r.rPts = upCast(std::move(r.rPts));
		r.k ++;
	}
	return r;
}

rangeData dfs(int l, int r) {
	if (l == r) {
		// wholeValue = 0 means it's empty
		return rangeData {-1, 0, vector<int>({1}), vector<int>({1})};
	}

	assert(l < r);
	int k = query_max(l, r);
	vector<int> locs;
	locs.push_back(l-1);
	{
		const vector<int>& v = vals[k];
		locs.insert(locs.end(), lower_bound(v.begin(), v.end(), l), lower_bound(v.begin(), v.end(), r));
	}
	locs.push_back(r);
	int M = int(locs.size()) - 1;
	vector<rangeData> ranges;
	for (int i = 0; i < M; i++) {
		ranges.emplace_back(upCast(dfs(locs[i]+1, locs[i+1]), k));
	}

	vector<int> rCnt({0});
	for (int i = 0; i < M; i++) {
		const rangeData& d = ranges[i];
		assert(d.k == k);
		for (int z = 0; z < int(d.lPts.size()); z++) {
			//cerr << z << ' ' << d.lPts[z] << ' ' << getNumLeft(offset - (L - min(z+1, L))) << '\n';
			ans += 1ll * d.lPts[z] * rCnt[max(0, int(rCnt.size()) - 1 - max(0, L - (z+1)))];
		}
		if (d.wholeValue == -1) {
			rCnt = vector<int>({0});
		}
		for (int z = int(d.rPts.size()) - 1; z >= 0; z--) {
			rCnt.push_back(rCnt.back() + d.rPts[z]);
		}
	}

	rangeData res{k, M-1, vector<int>({}), vector<int>({})};
	for (int i = 0; i < M; i++) {
		if (ranges[i].wholeValue == -1) {
			res.wholeValue = -1;
			break;
		} else {
			res.wholeValue += ranges[i].wholeValue;
		}
	}
	for (int i = 0; i < M; i++) {
		const rangeData& d = ranges[i];
		res.lPts.insert(res.lPts.end(), d.lPts.begin(), d.lPts.end());
		if (d.wholeValue == -1) {
			break;
		}
	}
	for (int i = M-1; i >= 0; i--) {
		const rangeData& d = ranges[i];
		res.rPts.insert(res.rPts.end(), d.rPts.begin(), d.rPts.end());
		if (d.wholeValue == -1) {
			break;
		}
	}
	/*
	cerr << "return " << l << ' ' << r << '\n';
	cerr << res.k << ' ' << res.wholeValue << '\n';
	for (auto c : res.lPts) { cerr << c << ' '; } cerr << '\n';
	for (auto c : res.rPts) { cerr << c << ' '; } cerr << '\n';
	for (int i = 0; i < M; i++) {
		cerr << "range" << '\n';
		cerr << ranges[i].k << ' ' << ranges[i].wholeValue << '\n';
		for (auto c : ranges[i].lPts) { cerr << c << ' '; } cerr << '\n';
		for (auto c : ranges[i].rPts) { cerr << c << ' '; } cerr << '\n';
	}
	*/
	return res;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> L;
	for (int i = 0; i < N; i++) {
		cin >> A[i];
	}
	precomp();
	dfs(0, N);
	ans += N;
	cout << ans << '\n';

	return 0;
}
