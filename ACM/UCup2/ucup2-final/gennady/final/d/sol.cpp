/**
 *    author:  tourist
 *    created: 22.02.2025 15:08:35
**/
#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
#include "debug.cpp"
#else
#define debug(...) 42
#endif

vector<string> Rotate(vector<string> s) {
  if (s.empty()) {
    return {};
  }
  int h = int(s.size());
  int w = int(s[0].size());
  vector<string> ns(w, string(h, '.'));
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      ns[j][i] = s[i][j];
    }
  }
  return ns;
}

vector<string> Solve(int h, int w, int r, int c, int x) {
  if (h == 1) {
    assert(r == 0);
    if (x >= c + 1) {
      string res(w, '#');
      for (int i = 0; i < x; i++) {
        res[i] = '.';
      }
      return {res};
    }
    if (x >= w - c) {
      string res(w, '#');
      for (int i = 0; i < x; i++) {
        res[w - 1 - i] = '.';
      }
      return {res};
    }
    return {};
  }
  if (w == 1) {
    return Rotate(Solve(w, h, c, r, x));
  }
  if (h == 2) {
    bool flip = false;
    if (r == 1) {
      flip = true;
      r = 0;
    }
    int least = w - 2;
    if (c == 0 || c == w - 1) {
      least += 1;
    }
    vector<string> s;
    if (x >= least) {
      s.assign(2, string(w, '#'));
      for (int j = 1; j < w - 1; j++) {
        s[0][j] = '.';
      }
      s[0][c] = '.';
      x -= least;
      for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
          if (x > 0 && s[i][j] == '#') {
            x -= 1;
            s[i][j] = '.';
          }
        }
      }
    }
    if (flip) {
      reverse(s.begin(), s.end());
    }
    return s;
  }
  if (w == 2) {
    return Rotate(Solve(w, h, c, r, x));
  }
  assert(h >= 3 && w >= 3);
  if (h % 2 == 1 && w % 2 == 0) {
    return Rotate(Solve(w, h, c, r, x));
  }
  vector<string> s(h, string(w, '#'));
  for (int j = 1; j < w - 1; j += 2) {
    for (int i = 1; i < h - 1; i++) {
      s[i][j] = '.';
    }
  }
  for (int j = 2; j < w - 2; j += 2) {
    if (j == c) {
      s[max(1, min(h - 2, r))][j] = '.';
    } else {
      s[1][j] = '.';
    }
  }
  if (w % 2 == 0) {
    for (int i = 1; i < h - 1; i++) {
      if (i == 1 || i == h - 2 || i % 2 == r % 2) {
        s[i][w - 2] = '.';
      }
    }
  }
  // +2 for corner
  if ((r == 0 || r == h - 1) && (c == 0 || c == w - 1)) {
    s[r][c] = '.';
    s[r][max(1, min(w - 2, c))] = '.';
  } else {
    // +1 for side
    if (r == 0 || r == h - 1 || c == 0 || c == w - 1) {
      s[r][c] = '.';
      assert(s[max(1, min(h - 2, r))][max(1, min(w - 2, c))] == '.');
    }
  }
  int cnt = 0;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      cnt += int(s[i][j] == '.');
    }
  }
  if (x < cnt) {
    return {};
  }
  x -= cnt;
  for (int j = 1; j < w - 1; j++) {
    for (int i = 2; i < h - 1; i++) {
      if (x > 0 && s[i][j] == '#' && s[i - 1][j] == '.') {
        x -= 1;
        s[i][j] = '.';
      }
    }
    for (int i = h - 3; i >= 1; i--) {
      if (x > 0 && s[i][j] == '#' && s[i + 1][j] == '.') {
        x -= 1;
        s[i][j] = '.';
      }
    }
  }
  vector<pair<int, int>> seq;
  for (int j = 0; j < w - 1; j++) {
    seq.emplace_back(0, j);
  }
  for (int i = 0; i < h - 1; i++) {
    seq.emplace_back(i, w - 1);
  }
  for (int j = w - 1; j > 0; j--) {
    seq.emplace_back(h - 1, j);
  }
  for (int i = h - 1; i > 0; i--) {
    seq.emplace_back(i, 0);
  }
  bool any = false;
  for (auto& [i, j] : seq) {
    if (s[i][j] == '.') {
      any = true;
      break;
    }
  }
  if (!any && x > 0) {
    s[0][1] = '.';
    x -= 1;
  }
  for (int rot = 0; rot < 2; rot++) {
    for (int i = 0; i < int(seq.size()); i++) {
      auto [mi, mj] = seq[i];
      auto [pi, pj] = seq[i == 0 ? int(seq.size()) - 1 : i - 1];
      if (x > 0 && s[mi][mj] == '#' && s[pi][pj] == '.') {
        x -= 1;
        s[mi][mj] = '.';
      }
    }
  }
  assert(x == 0);
  return s;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int tt;
  cin >> tt;
  while (tt--) {
    int h, w, r, c, x;
    cin >> h >> w >> r >> c >> x;
    --r; --c;
    auto s = Solve(h, w, r, c, x);
    debug(h, w, r, c, x, s);
    if (s.empty()) {
      cout << "No" << '\n';
    } else {
      cout << "Yes" << '\n';
      for (int i = 0; i < h; i++) {
        cout << s[i] << '\n';
      }
      assert(s[r][c] == '.');
      auto Bfs = [&](int i, int j) -> int {
        vector<vector<bool>> was(h, vector<bool>(w, false));
        vector<pair<int, int>> que = {{i, j}};
        was[i][j] = true;
        for (int b = 0; b < int(que.size()); b++) {
          for (int di = -1; di <= 1; di++) {
            for (int dj = -1; dj <= 1; dj++) {
              if (di * di + dj * dj == 1) {
                int ni = que[b].first + di;
                int nj = que[b].second + dj;
                if (ni >= 0 && nj >= 0 && ni < h && nj < w && s[ni][nj] == s[i][j] && !was[ni][nj]) {
                  que.emplace_back(ni, nj);
                  was[ni][nj] = true;
                }
              }
            }
          }
        }
        return int(que.size());
      };
      assert(Bfs(r, c) == x);
      bool done = false;
      for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
          if (!done && s[i][j] == '#') {
            assert(Bfs(i, j) == h * w - x);
            done = true;
          }
        }
      }
    }
  }
  return 0;
}
