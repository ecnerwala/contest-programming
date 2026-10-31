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
  auto original_a = a;
  int cnt = n - h;//(n - h) / 1;//min(h + w, n - h);
  debug(h, w, n, cnt);
  const int N = 300;
  vector<bitset<N>> bw(cnt);
  vector<bitset<N>> cw(cnt);
  int very_init_score = -1;
  int iter = 0;
  const int M = 1 * cnt;
  for (int outer = 0; outer < M; outer++) {
    int which = outer % cnt;
    vector<double> exps(max(h, w) + 3);
    double init_temp = 1;
    double final_temp = 0.05;
    double cur_temp = init_temp;
    vector<bitset<N>> aw(h);
    vector<bitset<N>> ar(w);
    for (int i = 0; i < h; i++) {
      for (int j = 0; j < w; j++) {
        if (a[i][j] == 1) {
          aw[i][j] = 1;
          ar[j][i] = 1;
        }
      }
    }
    vector<int> ones(cnt);
    for (int i = 0; i < cnt; i++) {
      for (int j = 0; j < h; j++) {
        ones[i] += bw[i][j];
      }
      for (int j = 0; j < w; j++) {
        ones[i] += cw[i][j];
      }
    }
    int score = -h;
    vector<int> awc(h);
    vector<int> arc(w);
    for (int i = 0; i < h; i++) {
      for (int j = 0; j < w; j++) {
        score += (a[i][j] == 1);
        awc[i] += a[i][j];
        arc[j] += a[i][j];
      }
    }
    for (int i = 0; i < cnt; i++) {
      score += max(0, ones[i] - 1);
    }
    auto init_score = score;
    if (outer == 0) very_init_score = score;
    mt19937 rng(58);
    uniform_real_distribution<double> urd(0, 1);
    int64_t TOT = 0;
    int64_t ACC = 0;
    auto start_score = score;
    auto best_score = score;
    bitset<N> best_bw = bw[which];
    bitset<N> best_cw = cw[which];
    auto init_a = a;
    auto init_bw = best_bw;
    auto init_cw = best_cw;
    vector<pair<int, int>> order(h + w);
    for (int j = 0; j < h + w; j++) {
      order[j] = make_pair(which, j);
    }
    auto start = TL / M * outer;
    auto finish = TL / M * (outer + 1);
    // debug(get_time(), start, finish);
    while (get_time() < finish) {
      ++iter;
      // which = min(which, cnt - 1);
      // vector<pair<int, int>> order(cnt * (h + w));
      // for (int i = 0; i < cnt; i++) {
      //   for (int j = 0; j < h + w; j++) {
      //     order[i * (h + w) + j] = make_pair(i, j);
      //   }
      // }
      cur_temp = init_temp * pow(final_temp / init_temp, (get_time() - start) / (finish - start));
      // debug(iter, get_time(), cur_temp, score);
      for (int i = 0; i < int(exps.size()); i++) {
        exps[i] = exp(-i / cur_temp);
      }
      shuffle(order.begin(), order.end(), rng);
      for (auto [x, y] : order) {
        if (y < h) {
          int delta = 0;
          // if (delta >= 4) continue;
          delta += !!ones[x];
          delta -= bw[x][y];
          ones[x] -= bw[x][y];
          bw[x][y] = !bw[x][y];
          ones[x] += bw[x][y];
          delta -= !!ones[x];
          delta += bw[x][y];
          delta -= awc[y];
          delta += int((aw[y] ^ cw[x]).count());
          // for (int j = 0; j < w; j++) {
          //   if (c[x][j]) {
          //     delta -= a[y][j];
          //     a[y][j] ^= 1;
          //     delta += a[y][j];
          //   }
          // }
          auto new_score = score + delta;
          TOT += 1;
          if (new_score <= score || (new_score > score && urd(rng) < exps[new_score - score])) {
            score = new_score;
            ACC += 1;
            for (int j = 0; j < w; j++) {
              if (cw[x][j]) {
                awc[y] -= a[y][j];
                arc[j] -= a[y][j];
                a[y][j] ^= 1;
                aw[y][j] = !aw[y][j];
                ar[j][y] = !ar[j][y];
                awc[y] += a[y][j];
                arc[j] += a[y][j];
              }
            }
            if (score < best_score) {
              best_score = score;
              best_bw = bw[which];
              best_cw = cw[which];
            }
          } else {
            ones[x] -= bw[x][y];
            bw[x][y] = !bw[x][y];
            ones[x] += bw[x][y];
            // for (int j = 0; j < w; j++) {
            //   if (c[x][j]) {
            //     a[y][j] ^= 1;
            //   }
            // }
          }
        } else {
          y -= h;
          int delta = 0;
          delta += !!ones[x];
          delta -= cw[x][y];
          ones[x] -= cw[x][y];
          cw[x][y] = !cw[x][y];
          ones[x] += cw[x][y];
          delta += cw[x][y];
          delta -= !!ones[x];
          delta -= arc[y];
          delta += int((ar[y] ^ bw[x]).count());
          // for (int j = 0; j < h; j++) {
          //   if (b[x][j]) {
          //     delta -= a[j][y];
          //     a[j][y] ^= 1;
          //     delta += a[j][y];
          //   }
          // }
          auto new_score = score + delta;
          TOT += 1;
          if (new_score <= score || (new_score > score && urd(rng) < exps[new_score - score])) {
            score = new_score;
            ACC += 1;
            for (int j = 0; j < h; j++) {
              if (bw[x][j]) {
                awc[j] -= a[j][y];
                arc[y] -= a[j][y];
                a[j][y] ^= 1;
                aw[j][y] = !aw[j][y];
                ar[y][j] = !ar[y][j];
                awc[j] += a[j][y];
                arc[y] += a[j][y];
              }
            }
            if (score < best_score) {
              best_score = score;
              best_bw = bw[which];
              best_cw = cw[which];
            }
          } else {
            ones[x] -= cw[x][y];
            cw[x][y] = !cw[x][y];
            ones[x] += cw[x][y];
            // for (int j = 0; j < h; j++) {
            //   if (b[x][j]) {
            //     a[j][y] ^= 1;
            //   }
            // }
          }
        }
      }
    }
    // if (iter % (200000000 / (h * w)) == 0) {
    {
      double ratio = double(ACC) / double(TOT);
      double dsc = 100 * (1 - double(score) / double(very_init_score));
      debug(iter, get_time(), cur_temp, very_init_score, init_score, score, ACC, TOT, ratio, dsc, which);
    }
    a = init_a;
    for (int i = 0; i < h; i++) {
      for (int j = 0; j < w; j++) {
        if (init_bw[i] && init_cw[j]) {
          a[i][j] ^= 1;
        }
        if (best_bw[i] && best_cw[j]) {
          a[i][j] ^= 1;
        }
      }
    }
    score = best_score;
    bw[which] = best_bw;
    cw[which] = best_cw;
  }
  for (int i = 0; i < cnt; i++) {
    // debug(i, b[i], c[i]);
  }
  for (int i = 0; i < h; i++) {
    // debug(i, a[i]);
  }
  vector<vector<int>> b(cnt, vector<int>(h, 0));
  vector<vector<int>> c(cnt, vector<int>(w, 0));
  for (int i = 0; i < cnt; i++) {
    for (int j = 0; j < h; j++) {
      b[i][j] = bw[i][j];
    }
  }
  for (int i = 0; i < cnt; i++) {
    for (int j = 0; j < w; j++) {
      c[i][j] = cw[i][j];
    }
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
  #ifdef LOCAL
    for (int i = 0; i < h; i++) {
      bool any = false;
      for (int j = 0; j < cnt; j++) {
        if (b[j][i] != 0) {
          any = true;
          break;
        }
      }
      assert(any);
    }
    for (int i = 0; i < cnt; i++) {
      bool any = false;
      for (int j = 0; j < w; j++) {
        if (c[i][j] != 0) {
          any = true;
          break;
        }
      }
      assert(any);
    }
    for (int i = 0; i < h; i++) {
      for (int j = 0; j < w; j++) {
        int x = 0;
        for (int k = 0; k < cnt; k++) x ^= b[k][i] & c[k][j];
        assert(x == original_a[i][j]);
      }
    }
  #endif
  return 0;
}
