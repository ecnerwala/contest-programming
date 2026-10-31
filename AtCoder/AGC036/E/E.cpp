#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e6;
int N;
char S[MAXN];

int cnt[3];

string go() {
	for (int i = 1; i <= N; i++) {
		cnt[S[i] - 'A'] ++;
	}
	vector<pair<int, int>> cnts({{cnt[0], 0}, {cnt[1], 1}, {cnt[2], 2}});
	sort(cnts.begin(), cnts.end());
	const int A = cnts[2].second;
	const int B = cnts[1].second;
	const int C = cnts[0].second;
	cerr << A << ' ' << B << ' ' << C << '\n';

	if (cnt[C] == 0) {
		return "";
	}
	vector<pair<bool, int>> regions; // isA and length
	regions.emplace_back(false, 0);
	for (int i = 1; i <= N; i++) {
		if (S[i] - 'A' == C) {
			regions.emplace_back(false, 0);
		} else {
			assert(S[i] - 'A' == A || S[i] - 'A' == B);
			if (regions.back().second == 0) {
				regions.back().first = (S[i] - 'A' == A);
			}
			regions.back().second++;
		}
	}
	int M = int(regions.size()) - 1;
	assert(M == cnt[C]);
	for (int i = 0; cnt[A] > cnt[B] && i <= M; i++) {
		if (regions[i].second == 0) continue;
		if (regions[i].second == 1 && (i != 0 && i != M)) continue;

		bool startA = regions[i].first;
		bool endA = regions[i].second % 2 == 0 ? (!startA) : startA;
		if (startA) {
			cnt[A] --;
			regions[i].first ^= 1;
			regions[i].second --;
			i--;
			continue;
		} else if (endA) {
			cnt[A] --;
			regions[i].second --;
			i--;
			continue;
		} else {
			// no a's to cut
		}
	}

	{
		// must mean we have a lot of singleton regions
		// just remove those
		vector<pair<bool, int>> nRegions;
		for (int i = 0; i <= M; i++) {
			if (regions[i].first == true && regions[i].second == 1) {
				if (cnt[A] > cnt[B]) {
					cnt[A] --;
					cnt[C] --;
					continue;
				}
			}
			// otherwise add it
			nRegions.push_back(regions[i]);
		}
		regions = nRegions;
		M = int(regions.size()) - 1;
		assert(M == cnt[C]);
		if (M == 0) return "";
		assert(M > 0);
	}

	assert(cnt[A] == cnt[B]);

	// now we just need to delete
	for (int i = 0; cnt[A] > cnt[C] && i <= M; i++) {
		if (regions[i].second <= 1) continue;
		if (regions[i].second == 2 && (i != 0 && i != M)) continue;
		{
			regions[i].second -= 2;
			cnt[A] --;
			cnt[B] --;
			i--;
			continue;
		}
	}
	assert(cnt[A] == cnt[B] && cnt[A] == cnt[C]);

	string res = "";
	for (int i = 0; i <= M; i++) {
		if (i) res += char('A' + C);
		for (int j = 0, d = regions[i].first; j < regions[i].second; j++, d ^= 1) {
			res += (d ? char('A' + A) : char('A' + B));
		}
	}
	return res;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	string _S; cin >> _S;
	S[0] = '^';
	for (char c : _S) {
		if (c != S[N]) {
			S[++N] = c;
		}
	}
	S[N+1] = '$';
	S[N+2] = 0;
	
	//cerr << S << '\n';

	cout << go() << '\n';

	return 0;
}
