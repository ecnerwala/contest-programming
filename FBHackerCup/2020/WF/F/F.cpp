#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int S; cin >> S;
		int N; cin >> N;

		cerr << "Case #" << case_num << ":" << '\n';
		cerr << "N = " << N << '\n';

		auto scan_pt = [&]() {
			int x, y; cin >> x >> y;
			if (y == 0) return x;
			else if (x == S) return S + y;
			else if (y == S) return S + S + (S-x);
			else if (x == 0) return S + S + S + (S-y);
			else assert(false);
		};

		int tot_val = 0;
		vector<pair<int, array<int, 2>>> intervals; intervals.reserve(2*N);
		for (int i = 0; i < N; i++) {
			int val; cin >> val;
			tot_val += val;

			int a = scan_pt();
			int b = scan_pt();
			int c = scan_pt();
			int d = scan_pt();

			assert(a != b && c != d);

			if (a > b) swap(a,b);
			if (c > d) swap(c,d);
			assert(a <= b && c <= d);
			if (a > c) { swap(a,c), swap(b,d); }
			assert(a <= b && c <= d);
			assert(a <= c);
			if (c >= b) {
				assert(a <= b && b <= c && c <= d);
				// a..b is cut and c..d is cut
				intervals.push_back({val, {b,a}});
				intervals.push_back({val, {d,c}});
			} else {
				assert(a <= c && c <= d && d <= b);
				intervals.push_back({val, {a,b}});
				intervals.push_back({val, {d,c}});
			}
		}

		assert(int(intervals.size()) == 2 * N);
		N *= 2;
		S *= 4;

		// Now we have our intervals, so solve the circular arc max clique problem, and then subtract the total
		int ans = 0;
		for (int st = 0; st < N; st++) {
			// First, we pick a reference interval which is contained in the set
			// We can assume it's minimal, i.e. there are no interval strictly inside it
			// Thus, every other interval contains either its start or its end.

			// If another interval contains both, it's auto good, if it contains neither, auto bad.

			// Otherwise, you can have left intervals and right intervals which don't intersect, which would be :(
			// To count those, we take everything a subtract a weighted max matching/flow

			int cur_val = 0;
			int lo = intervals[st].second[0];
			int hi = intervals[st].second[1];
			while (hi <= lo) hi += S;

			// we'll sort along lo->hi, and store a segtree on hi->lo+S
			vector<pair<int, pair<int, int>>> inserts; inserts.reserve(N);
			vector<pair<int, pair<int, int>>> deletes; deletes.reserve(N);

			for (int i = 0; i < N; i++) {
				int val = intervals[i].first;
				auto [a, b] = intervals[i].second;
				a %= S;
				while (a <= lo) a += S;
				assert(a <= lo + S);
				b %= S;
				while (b <= a) b += S;
				assert(lo < a && a < b);
				if (a <= lo + S && hi + S <= b) {
					// interval includes the whole thing
					cur_val += val;
				} else if (a <= hi && lo + S <= b) {
					// interval includes both endpoints
					cur_val += val;
				} else if (lo < a && b < hi) {
					// strictly inside is bad
					continue;
				} else if (hi < a && b < lo + S) {
					// strictly outside is bad
					continue;
				} else if (a <= lo + S && lo + S <= b) {
					assert(hi < a && b < hi + S);
					// this is a low interval
					// TODO: put this into a set
					cur_val += val;
					inserts.push_back({b - S, {a, val}});
				} else if (a <= hi && hi <= b) {
					assert(lo < a && b < lo + S);
					// this is a high interval
					// TODO: put this into a set
					cur_val += val;
					deletes.push_back({a, {b, val}});
				} else assert(false);
			}

			sort(inserts.begin(), inserts.end());
			sort(deletes.begin(), deletes.end());
			{
				map<int, int> mp;
				auto it = inserts.begin();
				for (auto evt : deletes) {
					for (; it != inserts.end() && it->first < evt.first; ++it) {
						mp[it->second.first] += it->second.second;
					}
					// if something is higher than b, then it's bad
					int v = evt.second.second;
					for (auto pt = mp.upper_bound(evt.second.first); v > 0 && pt != mp.end(); ) {
						if (v >= pt->second) {
							cur_val -= pt->second;
							v -= pt->second;
							pt = mp.erase(pt);
							continue;
						} else {
							cur_val -= v;
							pt->second -= v;
							v = 0;
							break;
						}
					}
				}
			}
			ans = max(ans, cur_val);
		}

		ans -= tot_val;
		cout << "Case #" << case_num << ": " << ans << '\n';;
	}

	return 0;
}
