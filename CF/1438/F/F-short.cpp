#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int H; cin >> H;
	int N = (1 << H) - 1;

	auto query = [&](array<int, 3> a) -> int {
		cout << '?' << ' ' << a[0]+1 << ' ' << a[1]+1 << ' ' << a[2]+1 << '\n' << flush;
		int res; cin >> res; return res-1;
	};

	mt19937 mt(48);
	map<int, int> mp;
	for (int i = 0; i < 422; i++) {
		int a = int(mt() % N);
		int b = int(mt() % N);
		int c = int(mt() % N);
		if (a == b || b == c || c == a) {
			i--;
			continue;
		}
		int v = query({a,b,c});
		if (v == a || v == b || v == c) continue;
		mp[v]++;
	}

	vector<pair<int, int>> freq; freq.reserve(mp.size());
	for (auto it : mp) freq.push_back({it.second, it.first});

	assert(freq.size() >= 2);
    nth_element(freq.begin(), freq.end(), freq.begin()+2, std::greater());

	int x = freq[0].second, y = freq[1].second;

    for (int i = 0; i < N; i++) {
        if (i == x || i == y) continue;
        if (query({x, y, i}) == i) {
            cout << "! " << i << '\n';
            exit(0);
        }
    }

	return 0;
}
