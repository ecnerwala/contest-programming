#include <cstdio>
#include <numeric>
#include <iostream>
#include <vector>
#include <set>
#include <cstring>
#include <string>
#include <map>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <bitset>
#include <queue>
#include <list>
#include <sstream>

using namespace std;

#define mp make_pair
#define pb push_back
#define rep(i,n) for(int i = 0; i < (n); i++)
#define re return
#define fi first
#define se second
#define sz(x) ((int) (x).size())
#define all(x) (x).begin(), (x).end()
#define sqr(x) ((x) * (x))
#define sqrt(x) sqrt(abs(x))
#define y0 y3487465
#define y1 y8687969
#define fill(x,y) memset(x,y,sizeof(x))
#define prev PREV

typedef vector<int> vi;
typedef long long ll;
typedef long double ld;
typedef double D;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef deque<int> deqi;
typedef deque<ii> deqii;
typedef vector<string> vs;
typedef vector<vi> vvi;

template<class T> T abs(T x) { re x > 0 ? x : -x; }

deqii tmp[222222];
deqii *arr = tmp + 111111;
vii used;

int main () {
	int n;
	cin >> n;
	for(int i = 0; i < n; i++) {
		ii p; cin >> p.fi >> p.se;
		int w = p.se - p.fi;
		arr[w].insert(lower_bound(all(arr[w]), p), p);
	}

	deqii res;
	for(int i = 0; i < n; i++) {
		int w; cin >> w;
		if(arr[w].empty()) {
			cout << "NO" << endl;
			return 0;
		}
		ii p = arr[w].front(); arr[w].pop_front();
		if(p.fi && !binary_search(all(used), ii(p.fi - 1, p.se))) {
			cout << "NO" << endl;
			return 0;
		}
		if(p.se && !binary_search(all(used), ii(p.fi, p.se - 1))) {
			cout << "NO" << endl;
			return 0;
		}
		used.insert(lower_bound(all(used), p), p);
		res.push_back(p);
	}

	cout << "YES" << endl;
	for(ii p : res) cout << p.fi << ' ' << p.se << endl;
}
