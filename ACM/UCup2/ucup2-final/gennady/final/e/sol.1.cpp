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
  vector<vector<Mint>> dp(k + 1, vector<Mint>(3));
  dp[k][2] = 1;
  int pa = k + 1;
  int pb = k + 1;
  C(n + m + k, 0);
  Mint quo = fact[n + k] * fact[m] * inv_fact[k];
  for (auto& [foo, bar] : mp) {
    auto [na, nb] = bar;
    vector<vector<Mint>> new_dp(k + 1, vector<Mint>(3));
    for (int c = 0; c <= k; c++) {
      for (int v = 0; v < 3; v++) {
        for (int kill = 0; kill <= min(nb, c); kill++) {
          for (int use = 0; use <= min(na, nb - kill); use++) {
            Mint ft = dp[c][v];
            if (ft == 0) {
              continue;
            }
            ft *= Dist(use, k);
            int ra = na - use;
            int rb = nb - kill - use;
            if (v == 2) {
              for (int x = 0; x <= ra; x++) {
                for (int y = 0; y <= rb; y++) {
                  Mint new_ft = ft;
                  new_ft *= Dist(ra - x, pa - c - 1);
                  new_ft *= Dist(rb - y, pb - k - (k - c) - 1);
                  new_dp[c - kill][x > y ? 0 : (x < y ? 1 : 2)] += new_ft;
                }
              }
             } else {
              ft *= Dist(ra, pa - c);
              ft *= Dist(rb, pb - k - (k - c));
              new_dp[c - kill][v] += ft;
            }
            debug(c, v, kill, use, pa, pb, ra, rb, new_dp);
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
    sum += dp[0][i];
    cout << dp[0][i] / quo << '\n';
  }
  debug(sum, quo, sum == quo);
  // assert(sum == quo);
  return 0;
}
