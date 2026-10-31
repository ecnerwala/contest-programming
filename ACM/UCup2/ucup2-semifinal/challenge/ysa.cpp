const double TL = 1.95 / 1;

double start_time_ = -1;
bool first_time = true;

double get_time() {
  timeval tv; 
  gettimeofday(&tv, 0); 
  auto ret = tv.tv_sec + tv.tv_usec * 1e-6;
  if (first_time) {
    start_time_ = ret;
    first_time = false;
  }
  return ret - start_time_;
}

...

  int64_t score = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      score += a[i][j]();
    }
  }
  auto best_score = score;
  debug(score, get_time());
  vector<double> exps(18000);
  double init_temp = 5e9;
  double final_temp = 0.1e9;
  double cur_temp = init_temp;
  int iter = 0;
  while (get_time() < TL) {
    ++iter;
    if (iter % 100000 == 0) {
      cur_temp = init_temp * pow(final_temp / init_temp, get_time() / TL);
      debug(iter, get_time(), cur_temp, score);
      for (int i = 0; i < int(exps.size()); i++) {
        exps[i] = exp(-i * 1e6 / cur_temp);
      }
      // score = EvalDPWhere();
    }
    int i = rng() % k;
    int64_t delta = 0;
    int old_id = id[i];
    int old_rs = rs[i];
    int old_cs = cs[i];
    for (int r = 0; r < 3; r++) {
      for (int c = 0; c < 3; c++) {
        delta -= a[rs[i] + r][cs[i] + c]();
        a[rs[i] + r][cs[i] + c] -= s[id[i]][r][c];
        delta += a[rs[i] + r][cs[i] + c]();
      }
    }
    id[i] = rng() % (m + 1);
    rs[i] = rng() % (n - 2);
    cs[i] = rng() % (n - 2);
    for (int r = 0; r < 3; r++) {
      for (int c = 0; c < 3; c++) {
        delta -= a[rs[i] + r][cs[i] + c]();
        a[rs[i] + r][cs[i] + c] += s[id[i]][r][c];
        delta += a[rs[i] + r][cs[i] + c]();
      }
    }
    auto new_score = score + delta;
    if (new_score >= score || (new_score < score && urd(rng) < exps[int(double(score - new_score) / 1e6 + 1)])) {
      score = new_score;
    } else {
      for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
          a[rs[i] + r][cs[i] + c] -= s[id[i]][r][c];
        }
      }
      id[i] = old_id;
      rs[i] = old_rs;
      cs[i] = old_cs;
      for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
          a[rs[i] + r][cs[i] + c] += s[id[i]][r][c];
        }
      }
    }
    if (score > best_score) {
      best_score = score;
      for (int j = 0; j < k; j++) {
        best_id[j] = id[j];
        best_rs[j] = rs[j];
        best_cs[j] = cs[j];
      }
    }
  }
  debug(get_time(), test_id, iter, score);
