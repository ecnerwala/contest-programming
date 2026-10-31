#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	enum struct EVT_TYPE {
		// rectangle delete is -0.1
		RECT_DELETE,
		// rectangle insert is 0.1
		RECT_INSERT,
		// cows are in 0.5
		COW,
		// flowers are in 0.8
		FLOWER,
	};

	vector<pair<pair<int, EVT_TYPE>, pair<pair<int, int>, int>>> evts;

	int NRECT; cin >> NRECT;
	for (int i = 0; i < NRECT; i++) {
		int xlo, ylo, xhi, yhi; cin >> xlo >> ylo >> xhi >> yhi;
		xhi++, yhi++;
		evts.push_back({{xlo, EVT_TYPE::RECT_DELETE}, {{ylo, yhi}, i}});
		evts.push_back({{xhi, EVT_TYPE::RECT_INSERT}, {{ylo, yhi}, i}});
	}

	int NFLOWER; cin >> NFLOWER;
	for (int i = 0; i < NFLOWER; i++) {
		int x, y; cin >> x >> y;
		evts.push_back({{x, EVT_TYPE::FLOWER}, {{y,y}, i}});
	}

	int NCOW; cin >> NCOW;
	vector<int> ans(NCOW);
	for (int i = 0; i < NCOW; i++) {
		int x, y; cin >> x >> y;
		evts.push_back({{x, EVT_TYPE::COW}, {{y,y}, i}});
	}

	const int S = 1 << 20;
	vector<int> seg(2*S);
	auto query = [&](int i) -> int {
		int res = 0;
		for (int a = S+i; a; a /= 2) {
			res += seg[a];
		}
		return res;
	};
	// inclusive range
	auto updateRange = [&](int l, int r, int v) -> void {
		for (int a = S+l, b = S+r+1; a < b; a /= 2, b /= 2) {
			if (a & 1) seg[a++] += v;
			if (b & 1) seg[--b] += v;
		}
	};

	map<int, int> eagerFlowers;
	map<int, int> lazyFlowers;

	// eagerFlowers always contains all right endpoints
	eagerFlowers[0] = 0;

	vector<int> rectCorrection(NRECT);

	sort(evts.begin(), evts.end());
	reverse(evts.begin(), evts.end());

	for (auto evt : evts) {
		EVT_TYPE type = evt.first.second;
		switch(type) {
			case EVT_TYPE::RECT_INSERT:
				{
					int ylo, yhi; tie(ylo, yhi) = evt.second.first;
					assert(ylo < yhi);
					int rnum = evt.second.second;

					// shove the lazy flowers out of the way
					{
						auto it = lazyFlowers.lower_bound(ylo);
						int cntLazy = 0;
						while (it != lazyFlowers.end() && it->first < yhi) {
							cntLazy += it->second;
							updateRange(ylo, it->first, -1 * it->second);
							it = lazyFlowers.erase(it);
						}
						lazyFlowers[ylo-1] += cntLazy;
					}

					// add a placeholder for any eager flowers
					eagerFlowers.emplace(ylo, 0);
					eagerFlowers.emplace(yhi, 0);

					rectCorrection[rnum] = query(ylo);
					updateRange(ylo, yhi-1, -rectCorrection[rnum]);
				}
				break;
			case EVT_TYPE::RECT_DELETE:
				{
					int ylo, yhi; tie(ylo, yhi) = evt.second.first;
					assert(ylo < yhi);
					int rnum = evt.second.second;

					// undo the correction
					updateRange(ylo, yhi-1, rectCorrection[rnum]);

					// remove all the lingering lazy flowers
					{
						auto it = lazyFlowers.lower_bound(ylo);
						while (it != lazyFlowers.end() && it->first < yhi) {
							updateRange(ylo, it->first, -1 * it->second);
							it = lazyFlowers.erase(it);
						}
					}

					// allow eager flowers to pass
					{
						auto it = eagerFlowers.find(yhi);
						assert(it != eagerFlowers.end());
						assert(it != eagerFlowers.begin());

						auto jt = it; --jt;

						// these guys are done!
						assert(jt->first == ylo);
						jt = eagerFlowers.erase(jt);

						assert(jt != eagerFlowers.begin());
						--jt;

						updateRange(jt->first, it->first-1, it->second);
						jt->second += it->second;
						eagerFlowers.erase(it);
					}
				}
				break;
			case EVT_TYPE::FLOWER:
				{
					int y = evt.second.first.first;

					{
						auto it = eagerFlowers.upper_bound(y);
						assert(it != eagerFlowers.begin());
						--it;
						updateRange(it->first, y, 1);
						it->second++;
					}

					lazyFlowers[y]++;
				}
				break;
			case EVT_TYPE::COW:
				{
					int y = evt.second.first.first;
					int cnum = evt.second.second;
					ans[cnum] = query(y);
				}
				break;
			default: assert(false);
		}
	}

	for (int i = 0; i < NCOW; i++) {
		cout << ans[i] << '\n';
	}

	return 0;
}
