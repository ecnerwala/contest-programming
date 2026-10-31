/**
 *    author:  tourist
 *    created: 22.06.2024 15:53:23
**/
#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
#include "../debug.cpp"
#else
#define debug(...) 42
#endif

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int n;
  cin >> n;
  vector<vector<int>> a(n, vector<int>(n));
  for (int i = 0; i < n; i++) {
    string foo;
    cin >> foo;
    for (int j = 0; j < n; j++) {
      a[i][j] = int(foo[j] - '0');
    }
  }
  vector<vector<int>> b(n, vector<int>(n));
  for (int i = 0; i < n; i++) {
    string foo;
    cin >> foo;
    for (int j = 0; j < n; j++) {
      b[i][j] = int(foo[j] - '0');
    }
  }
	vector<pair<int,int> > ops;
	vector<pair<int,int> > ops_inv;
	auto do_row = [&](int i){
		ops.push_back({0, i});
		rotate(a[i].begin(), a[i].begin() + 1, a[i].end());
		// pr();
	};
	auto do_row_inv = [&](int i){
		ops_inv.push_back({0, i});
		rotate(b[i].begin(), b[i].end() - 1, b[i].end());
		// pr();
	};
	auto do_col = [&](int i){
		ops.push_back({1, i});
		int v = 1 ^ a[n-1][i];
		for(int j = n-1; j > 0; j--){
			a[j][i] = a[j-1][i];
		}
		a[0][i] = v;
		// pr();
	};
	auto do_col_inv = [&](int i){
		ops_inv.push_back({1, i});
		int v = 1 ^ b[0][i];
		for(int j = 0; j < n-1; j++) {
			b[j][i] = b[j+1][i];
		}
		b[n-1][i] = v;
		// pr();
	};
  vector<int> ptr(n, 1);
  while (*min_element(ptr.begin(), ptr.end()) < n) {
    bool any = false;
    for (int i = 0; i < n; i++) {
      if (ptr[i] < n && a[i][0] == (i == n - 1 ? 0 : b[i][ptr[i]])) {
        do_row(i);
        any = true;
        ptr[i] += 1;
      }
    }
    if (!any) {
      for (int i = 0; i < n; i++) {
        do_col(0);
      }
    }
  }
  for (int i = 0; i < n; i++) {
    if (a[n - 1][0] == 1) {
      do_row(n - 1);
    }
    do_col(0);
  }
  for (int i = 0; i < n; i++) {
    do_col(0);
  }
  for (int i = 0; i < n; i++) {
    do_row(n - 1);
    if (a[n - 1][0] == 1) {
      do_col(0);
    }
  }

  for (int it = 0; it < n; it++) {
    while (b[n - 1][0] != 0) {
      do_col_inv(0);
    }
    do_row_inv(n - 1);
  }
  debug(b);
  for (int i = 0; i < n; i++) {
    if (b[n - 1][0] == 1) {
      do_row_inv(n - 1);
    }
    do_col_inv(0);
  }
  debug(b);
  for (int i = 0; i < n; i++) {
    // do_col_inv(0);
  }
  for (int i = 0; i < n; i++) {
    if (b[n - 1][0] == 1) {
      do_col_inv(0);
    }
    do_row_inv(n - 1);
  }

  while (b[0][0] == 1 || b[n - 1][0] == 1) do_col_inv(0);

debug(a);
debug(b);
  assert(a == b);

  reverse(ops_inv.begin(), ops_inv.end());
  for (auto x : ops_inv) {
    ops.push_back(x);
  }
	cout << ops.size() << '\n';
	for(auto [type, row] : ops){
		cout << (vector<string>{"row", "column"}[type]) << ' ' << (row+1) << '\n';
	}
  return 0;
}
