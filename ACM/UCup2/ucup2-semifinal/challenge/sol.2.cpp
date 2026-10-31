/**
 *    author:  tourist
 *    created: 21.06.2024 13:04:53
**/
#undef _GLIBCXX_DEBUG
#include <bits/stdc++.h>

using namespace std;

#ifdef LOCAL
#include "debug.cpp"
#else
#define debug(...) 42
#endif

const double TL = 19.95;

double start_time_ = -1;
bool first_time = true;

double get_time() {
  timeval tv; 
  gettimeofday(&tv, 0); 
  auto ret = double(tv.tv_sec) + double(tv.tv_usec) * 1e-6;
  if (first_time) {
    start_time_ = ret;
    first_time = false;
  }
  return ret - start_time_;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  get_time();
  int h, w;
  cin >> h >> w;
  int k, n;
  cin >> k >> n;
  vector<vector<int>> a(h, vector<int>(w));
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      cin >> a[i][j];
    }
  }
  int cnt = n - h;
  vector<vector<int>> b(cnt, vector<int>(h, 0));
  vector<vector<int>> c(cnt, vector<int>(w, 0));
  vector<int> ones(cnt);
  vector<pair<int, int>> order(cnt * (h + w));
  for (int i = 0; i < cnt; i++) {
    for (int j = 0; j < h + w; j++) {
      order[i * (h + w) + j] = make_pair(i, j);
    }
  }
  int score = 0;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      score += (a[i][j] == 1);
    }
  }
  debug(score);
  auto init_score = score;
  auto best_score = score;
  debug(score, get_time());
  vector<double> exps(3 * (h + w));
  double init_temp = 0.01;
  double final_temp = 0.01;
  double cur_temp = init_temp;
  mt19937 rng(58);
  uniform_real_distribution<double> urd(0, 1);
  int64_t TOT = 0;
  int64_t ACC = 0;
  int iter = 0;
  while (get_time() < TL) {
    ++iter;
    cur_temp = init_temp * pow(final_temp / init_temp, get_time() / TL);
    // debug(iter, get_time(), cur_temp, score);
    for (int i = 0; i < int(exps.size()); i++) {
      exps[i] = exp(-i / cur_temp);
    }
    shuffle(order.begin(), order.end(), rng);
    for (auto [x, y] : order) {
      if (y < h) {
        int delta = 0;
        delta += !!ones[x];
        delta -= b[x][y];
        ones[x] -= b[x][y];
        b[x][y] ^= 1;
        ones[x] += b[x][y];
        delta -= !!ones[x];
        delta += b[x][y];
        for (int j = 0; j < w; j++) {
          if (c[x][j]) {
            delta -= a[y][j];
            a[y][j] ^= 1;
            delta += a[y][j];
          }
        }
        auto new_score = score + delta;
        TOT += 1;
        if (new_score <= score || (new_score > score && urd(rng) < exps[new_score - score])) {
          score = new_score;
          ACC += 1;
        } else {
          ones[x] -= b[x][y];
          b[x][y] ^= 1;
          ones[x] += b[x][y];
          for (int j = 0; j < w; j++) {
            if (c[x][j]) {
              a[y][j] ^= 1;
            }
          }
        }
      } else {
        y -= h;
        int delta = 0;
        delta += !!ones[x];
        delta -= c[x][y];
        ones[x] -= c[x][y];
        c[x][y] ^= 1;
        ones[x] += c[x][y];
        delta += c[x][y];
        delta -= !!ones[x];
        for (int j = 0; j < h; j++) {
          if (b[x][j]) {
            delta -= a[j][y];
            a[j][y] ^= 1;
            delta += a[j][y];
          }
        }
        auto new_score = score + delta;
        TOT += 1;
        if (new_score <= score || (new_score > score && urd(rng) < exps[new_score - score])) {
          score = new_score;
          ACC += 1;
        } else {
          ones[x] -= c[x][y];
          c[x][y] ^= 1;
          ones[x] += c[x][y];
          for (int j = 0; j < h; j++) {
            if (b[x][j]) {
              a[j][y] ^= 1;
            }
          }
        }
      }
    }
    if (iter % 10 == 0) {
      double ratio = double(ACC) / double(TOT);
      debug(iter, get_time(), cur_temp, init_score, score, ACC, TOT, ratio);
    }
  }
  debug(iter, get_time(), cur_temp, init_score, score);
  for (int i = 0; i < cnt; i++) {
    // debug(i, b[i], c[i]);
  }
  for (int i = 0; i < h; i++) {
    // debug(i, a[i]);
  }
  for (int i = 0; i < h; i++) {
    bool any = accumulate(a[i].begin(), a[i].end(), 0) > 0;
    if (any) {
      cnt += 1;
      b.emplace_back(h);
      b.back()[i] = 1;
      c.emplace_back(w);
      c.back() = a[i];
    }
  }
  for (int i = cnt - 1; i >= 0; i--) {
    bool any1 = accumulate(b[i].begin(), b[i].end(), 0) > 0;
    bool any2 = accumulate(c[i].begin(), c[i].end(), 0) > 0;
    if (!any1 || !any2) {
      b[i] = b[cnt - 1];
      c[i] = c[cnt - 1];
      cnt -= 1;
    }
  }
  cout << 2 << '\n';
  cout << h << " " << cnt << '\n';
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < cnt; j++) {
      cout << b[j][i] << " \n"[j == cnt - 1];
    }
  }
  cout << cnt << " " << w << '\n';
  for (int i = 0; i < cnt; i++) {
    for (int j = 0; j < w; j++) {
      cout << c[i][j] << " \n"[j == w - 1];
    }
  }
  return 0;
}
