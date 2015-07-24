#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <cassert>
#include <cmath>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;
typedef long long ll;

int main() {
	int T; cin >> T;
	while(T--) {
		int d; cin >> d;
		int N = 1;
		vector<pair<pair<int, int>, int>> edges;
		int cur = 0;
		for (int l = 0; d > 3; l++) {
			if(d % 2 == 1) {
				int c = N++;
				edges.push_back(make_pair(make_pair(cur, c), 0));
				cur = c;
				d--;
			}
			int mult = (1 << (27 - l));
			int a = N++;
			int b = N++;
			edges.push_back(make_pair(make_pair(cur, a), mult * -3));
			edges.push_back(make_pair(make_pair(cur, b), mult * -5));
			edges.push_back(make_pair(make_pair(a, b), mult * -4));
			cur = b;

			d -= 2;
			assert(d % 2 == 0);
			d /= 2;
		}

		while(d > 1) {
			int c = N++;
			edges.push_back(make_pair(make_pair(cur, c), 0));
			cur = c;
			d--;
		}

		cerr << N << '\n';
		cout << N << '\n' << int(edges.size()) << '\n';

		for(auto it : edges) {
			cout << it.first.first << ' ' << it.first.second << ' ' << it.second << '\n';
		}
	}
	return 0;
}
