#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	string S; cin >> S;
	int N = int(S.size());

	deque<int> los;
	deque<int> his;
	los.push_back(0);
	his.push_back(0);
	int shift = 0;

	for (int i = 0; i < N; i++) {
		char c = S[i];
		if (c == '0') {
			shift += -1;
		} else if (c == '1') {
			shift += +1;
		} else if (c == '?') {
			{
				auto tmp = los.front(); los.pop_front();
				if (!los.empty() && los.front() + shift >= -2) {
					auto tmp2 = his.front(); his.pop_front();
					auto nhi = his.front()+2;
					auto nlo = los.front()+2;
					if (nhi > tmp2) {
						his.push_front(tmp2);
						los.push_front(tmp);
					}
					his.push_front(nhi);
					los.push_front(nlo);
				} else {
					los.push_front(tmp);
					los.push_front(los.front()+2);
					his.push_front(his.front()+2);
				}
			}

			shift += -1;
		} else assert(false);
		{
			auto tmp = his.back(); his.pop_back();
			tmp = max(tmp + shift, 0) - shift;
			if (!his.empty() && his.back() == tmp) {
				los.pop_back();
			} else {
				his.push_back(tmp);
			}
		}
		{
			auto tmp = los.front(); los.pop_front();
			tmp = min(tmp + shift, 0) - shift;
			if (!los.empty() && los.front() == tmp) {
				his.pop_front();
			} else {
				los.push_front(tmp);
			}
		}

		//cerr << c << ' ' << shift << '\n';
		//for (auto it : los) { cerr << it+shift << ' '; } cerr << '\n';
		//for (auto it : his) { cerr << it+shift << ' '; } cerr << '\n';
		//cerr << '\n';
	}

	int ans = 2*N+200; // INF
	while (!his.empty()) {
		ans = min(ans, his.back() - los.back());
		his.pop_back();
		los.pop_back();
	}
	cout << ans << '\n';

	return 0;
}
