
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, m, k;
  cin >> n >> m >> k;
  vector<int> a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }
  vector<int> b(m);
  for (int i = 0; i < m; i++) {
    cin >> b[i];
  }
  map<int, pair<int, int>> mp;
  for (int x : a) {
    mp[-x].first += 1;
  }
  for (int x : b) {
    mp[-x].second += 1;
  }
  auto Dist = [&](int items, int boxes) -> Mint {
    assert(items >= 0 && boxes >= 0);
    if (boxes == 0) {
      return items == 0 ? 1 : 0;
    }
    return C(items + boxes - 1, items);
  };
  auto Count = [&](int equal, int greater, int rem_a, int rem_b, int items_a, int items_b) -> Mint {
    equal += greater;
    items_a -= greater;
    if (items_a < 0) return 0;
    rem_a += greater;
    Mint ret = 0;
    for (int in_eq = 0; in_eq <= min(items_a, items_b); in_eq++) {
      Mint cur = Dist(in_eq, equal);
      cur *= Dist(items_a - in_eq, rem_a);
      cur *= Dist(items_b - in_eq, rem_b);
      ret += cur;
    }
    return ret;
  };
  vector dp(k + 1, vector(k + 1, vector<Mint>(3)));
  dp[k][0][2] = 1;
  int pa = k + 1;
  int pb = k + 1;
  C(n + m + k, 0);
  Mint quo = fact[n + k] * fact[m] * inv_fact[k];
  for (auto& [foo, bar] : mp) {
    auto [na, nb] = bar;
    vector new_dp(k + 1, vector(k + 1, vector<Mint>(3)));
    // c = how many are still in state 0 (winner in b is not filled)
    // d = how many are already in state 2 (winner in b is filled, and a is winning)
    for (int c = 0; c <= k; c++) {
      for (int d = 0; d <= k - c; d++) {
        for (int v = 0; v < 3; v++) {
          for (int kill = 0; kill <= min(nb, c); kill++) {
            for (int win = 0; win <= k - d - c; win++) {
              Mint ft = dp[c][d][v];
              if (ft == 0) {
                continue;
              }
              if (v == 2) {
                Mint total;
                {
                  int equal = k - d - win;
                  int greater = win;
                  int rem_a = pa - equal - greater;
                  int rem_b = pb - equal - greater;
                  total = Count(equal, greater, rem_a, rem_b, na, nb - kill);
                }
                {
                  int equal = k - d - win + 1;
                  int greater = win;
                  int rem_a = pa - equal - greater;
                  int rem_b = pb - equal - greater;
                  auto got = Count(equal, greater, rem_a, rem_b, na, nb - kill);
                  new_dp[c - kill][d + win][2] += got;
                  total -= got;
                }
                {
                  int equal = k - d - win;
                  int greater = win + 1;
                  int rem_a = pa - equal - greater;
                  int rem_b = pb - equal - greater;
                  auto got = Count(equal, greater, rem_a, rem_b, na, nb - kill);
                  new_dp[c - kill][d + win][0] += got;
                  total -= got;
                }
                new_dp[c - kill][d + win][1] += total;
              } else {
                int equal = k - d - win;
                int greater = win;
                int rem_a = pa - equal - greater;
                int rem_b = pb - equal - greater;
                new_dp[c - kill][d + win][v] += Count(equal, greater, rem_a, rem_b, na, nb - kill);
              }
              debug(c, v, kill, use, pa, pb, ra, rb, new_dp);
            }
          }
        }
      }
    }
    pa += na;
    pb += nb;
    quo *= inv_fact[na] * inv_fact[nb];
    swap(dp, new_dp);
    debug(dp);
  }
  Mint sum = 0;
  for (int i = 0; i < 3; i++) {
    Mint cur = 0;
    for (int d = 0; d <= k; d++) {
      cur += dp[0][d][i];
    }
    sum += cur;
    cout << cur / quo << '\n';
  }
  debug(sum, quo, sum == quo);
  // assert(sum == quo);
  return 0;
}
