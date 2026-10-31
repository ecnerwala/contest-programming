#include <algorithm>
#include <bits/stdc++.h>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>
#define sqr(x) ((x) * (x))
#define fz1(i, n) for ((i) = 1; (i) <= (n); (i)++)
#define fd1(i, n) for ((i) = (n); (i) >= 1; (i)--)
#define fz0g(i, n) for ((i) = 0; (i) <= (n); (i)++)
#define fd0g(i, n) for ((i) = (n); (i) >= 0; (i)--)
#define fz0k(i, n) for ((i) = 0; (i) < (n); (i)++)
#define fd0k(i, n) for ((i) = (((long long)(n)) - 1); (i) >= 0; (i)--)
#define fz(i, x, y) for ((i) = (x); (i) <= (y); (i)++)
#define fd(i, y, x) for ((i) = (y); (i) >= (x); (i)--)
#define fzin fz1(i, n)
#define fzim fz1(i, m)
#define fzjn fz1(j, n)
#define fzjm fz1(j, m)
#define ff(c, itr)                                                             \
  for (__typeof((c).begin()) itr = (c).begin(); itr != (c).end(); ++itr)
#define pb push_back
#define mk make_pair
#define rdst(st, len)                                                          \
  {                                                                            \
    static char ss[len];                                                       \
    scanf(" %s", ss);                                                          \
    (st) = ss;                                                                 \
  }
#define spln(i, n) (i == n ? '\n' : ' ')
#define fac_init(n)                                                            \
  {                                                                            \
    fac[0] = fac[1] = inv[1] = fi[0] = fi[1] = 1;                              \
    fz(i, 2, n) {                                                              \
      fac[i] = 1ll * fac[i - 1] * i % mod;                                     \
      inv[i] = 1ll * (mod - mod / i) * inv[mod % i] % mod;                     \
      fi[i] = 1ll * fi[i - 1] * inv[i] % mod;                                  \
    }                                                                          \
  }
using namespace std;
typedef long long i64;
typedef long double f80;
typedef unsigned int u32;
typedef unsigned long long u64;
inline void read(int &x) {
  char c;
  int f = 1;
  while (!isdigit(c = getchar()))
    if (c == '-')
      f = -1;
  x = (c & 15);
  while (isdigit(c = getchar()))
    x = (x << 1) + (x << 3) + (c & 15);
  x *= f;
}
int n, q, i, j;
struct edg {
  int dest, w, c;
};
vector<edg> bi[200005];
int parent[200005], edge_color[200005], fw[200005];
int bel[200005], lst[200005];
int sz[200005], tp[200005], son[200005], depth[200005];
i64 dis[200005];
int dfn[200005], ti, mp[200005];
int lg[200005], f[19][200005];
void dfs1(int x, int p) {
  parent[x] = p;
  sz[x] = 1;
  for (auto e : bi[x]) {
    if (e.dest == p)
      continue;
    depth[e.dest] = depth[x] + 1;
    dis[e.dest] = dis[x] + e.w;
    edge_color[e.dest] = e.c;
    dfs1(e.dest, x);
    sz[x] += sz[e.dest];
    if (sz[son[x]] < sz[e.dest])
      son[x] = e.dest;
  }
}
void dfs2(int x, int t) {
  tp[x] = t;
  mp[dfn[x] = ++ti] = x;
  f[0][ti] = dfn[parent[x]];
  if (son[x])
    dfs2(son[x], t);
  for (auto e : bi[x]) {
    if (e.dest == parent[x] || e.dest == son[x])
      continue;
    dfs2(e.dest, e.dest);
  }
}
int lca(int x, int y) {
  if (x == y)
    return x;
  x = dfn[x];
  y = dfn[y];
  if (x > y)
    swap(x, y);
  int l = x + 1, r = y;
  int t = lg[r - l + 1];
  assert((1<<t) <= r-l+1);
  return mp[min(f[t][l], f[t][r - (1 << t) + 1])];
}
int nxt(int x, int t) {
  while (dfn[parent[tp[x]]] > dfn[t])
    x = parent[tp[x]];
  if (parent[tp[x]] == t)
    return tp[x];
  return mp[dfn[t] + 1];
}
i64 calc(int x, int y) { return dis[x] + dis[y] - 2 * dis[lca(x, y)]; }
int tim[200005], col[200005];
vector<int> v[800005];
int vis[800005];
i64 ans[800005];
bool verbose = false;
void update(int x, int l, int r, int ql, int qr, int c) {
  //	if(x==1) cerr<<ql<<' '<<qr<<' '<<c<<endl;
  vis[x] = 1;
  if (ql <= l && r <= qr) {
    v[x].push_back(c);
    return;
  }
  int mid = (l + r) / 2;
  if (ql <= mid)
    update(x * 2, l, mid, ql, qr, c);
  if (qr > mid)
    update(x * 2 + 1, mid + 1, r, ql, qr, c);
}
struct node {
  int a, b;
  i64 c;
};
node operator+(node x, node y) {
  node t = x;
  i64 tmp;
  if (y.c > t.c)
    t = y;
  tmp = calc(x.a, y.a);
  if (tmp > t.c)
    t = (node){x.a, y.a, tmp};
  tmp = calc(x.a, y.b);
  if (tmp > t.c)
    t = (node){x.a, y.b, tmp};
  tmp = calc(x.b, y.a);
  if (tmp > t.c)
    t = (node){x.b, y.a, tmp};
  tmp = calc(x.b, y.b);
  if (tmp > t.c)
    t = (node){x.b, y.b, tmp};
  return t;
}
int *buf1[10000005], val1[10000005], tot1;
i64 *buf2[10000005], val2[10000005];
int tot2;
node *buf3[1000005], val3[1000005];
int tot3;
void upd(int &x, int y) {
  buf1[++tot1] = &x;
  val1[tot1] = x;
  x = y;
}
void upd(i64 &x, i64 y) {
  buf2[++tot2] = &x;
  val2[tot2] = x;
  x = y;
}
void upd(node &x, node y) {
  buf3[++tot3] = &x;
  val3[tot3] = x;
  x = y;
}
int dsu[200005], dsz[200005], cc_top[200005];
node cc_diam[200005];
i64 mx[200005], sec[200005];
std::set<std::pair<i64, int>> mxs[200005];
int mxid[200005], secid[200005];
i64 rc[200005];
int find(int x) { return dsu[x] == x ? x : find(dsu[x]); }
void merge(int x, int y) {
  assert(dsu[x] == x);
  assert(dsu[y] == y);
  if (dsz[x] > dsz[y])
    swap(x, y);
  upd(dsz[y], dsz[x] + dsz[y]);
  upd(dsu[x], y);
  int t = depth[cc_top[x]] < depth[cc_top[y]] ? cc_top[x] : cc_top[y];
  upd(cc_diam[t], cc_diam[cc_top[x]] + cc_diam[cc_top[y]]);
  int lt = nxt(cc_top[x] ^ cc_top[y] ^ t, t);
  upd(cc_diam[lt], cc_diam[lt] + cc_diam[cc_top[x] ^ cc_top[y] ^ t]);
  upd(cc_top[y], t);
}
int hv[200005];
int num_verts = 0;
auto solve_slow = []() -> std::pair<int64_t, int64_t> {
  std::vector<int64_t> dp_up(n + 1, 0);
  std::vector<std::map<int, int64_t>> dp_any(n + 1);
  int64_t ans = 0;
  int64_t lca = -1;
  auto setmax = [](int64_t &a, int64_t b) {
    if (b > a)
      a = b;
  };
  for (int i = n; i >= 1; i--) {
    int p = parent[i];
    if (p == -1)
      break;
    int64_t w = dp_up[i] + fw[i];
    if (hv[i] && hv[p]) {
      auto &ov = dp_any[p][edge_color[i]];
      if (ov + w > ans) {
        ans = ov + w;
        lca = p;
      }
      setmax(ov, w);
      if (edge_color[i] == edge_color[p]) {
        setmax(dp_up[p], w);
      }
    }
  }
  return {ans, lca};
};
void turn_on_vert(int u, i64 &cur) {
  assert(!hv[u]);
  upd(hv[u], 1);
  cur = max(cur, rc[u]);
  int id = find(u), t = cc_top[id];
  i64 ori = -1;
  assert(t == u);
  if (parent[t] && edge_color[parent[t]] == edge_color[t]) {
    int tf = find(parent[t]), tt = cc_top[tf];
    ori = max(dis[cc_diam[tt].a], dis[cc_diam[tt].b]) - dis[parent[tt]];
    assert(tf != id);
    if (verbose) {
      cerr << " orig" << ' ' << tt << '\n';
      cerr << " diam " << cc_diam[tt].a << ' ' << cc_diam[tt].b << ' ' << cc_diam[tt].c << '\n';
      cerr << " orig" << ' ' << t << '\n';
      cerr << " diam " << cc_diam[t].a << ' ' << cc_diam[t].b << ' ' << cc_diam[t].c << '\n';
    }
    merge(tf, id);
    id = find(u);
    t = cc_top[id];
    assert(t == tt);
  }

  if (hv[t]) {
    if (verbose) {
      cerr << " in hv[t]" << ' ' << t << '\n';
      cerr << " diam " << cc_diam[t].a << ' ' << cc_diam[t].b << ' ' << cc_diam[t].c << '\n';
    }
    cur = max(cur, cc_diam[t].c);
    if (parent[t]) {
      i64 len = max(dis[cc_diam[t].a], dis[cc_diam[t].b]) - dis[parent[t]];
      assert(len >= ori);
      int b = bel[t];
      assert(b);
      if (t != u) {
        assert(mxs[b].erase({ori, t}));
      }
      mxs[b].insert({len, t});
      if (mx[b] == ori)
        upd(mx[b], len);
      else if (mx[b] < len) {
        upd(sec[b], mx[b]);
        upd(mx[b], len);
      } else if (sec[b] < len)
        upd(sec[b], len);
      auto it = mxs[b].rbegin();
      assert(mx[b] == (it == mxs[b].rend() ? 0 : it->first));
      assert(sec[b] == (it == mxs[b].rend() ? 0 : std::next(it) == mxs[b].rend() ? 0 : std::next(it)->first));
      if (mx[b] + sec[b] > rc[parent[t]])
        upd(rc[parent[t]], mx[b] + sec[b]);
      if (hv[parent[t]])
        cur = max(cur, rc[parent[t]]);
    }
  } else {
    assert(u != t);
    cur = max(cur, cc_diam[nxt(u, t)].c);
  }
}
void solve(int x, int l, int r, i64 cur) {
  if (!(l <= 30836 && 30836 <= r))
    return;
  if (!vis[x])
    return;
  vis[x] = 0;
  int m1 = tot1, m2 = tot2, m3 = tot3;
  for (int u : v[x]) {
    if (l <= 30836 && 30836 <= r) {
      num_verts++;
    }
    if (num_verts >= 48530) {
      verbose = true;
      std::cerr << "inserting " << u << ' ' << parent[u] << ' ' << find(u) << ' ' << cc_top[find(u)] << ' ' << parent[cc_top[find(u)]] << '\n';
    }
    turn_on_vert(u, cur);
    if (num_verts >= 48530) {
      std::cerr << "inserted " << u << ' ' << parent[u] << ' ' << find(u) << ' ' << cc_top[find(u)] << ' ' << parent[cc_top[find(u)]] << '\n';
      for (int i = 0, vert = u; i < 10; i++) {
        cerr << "parent " << i << ' ' << vert << ' ' << edge_color[vert] << ' ' << hv[vert] << '\n';
        vert = parent[vert];
      }
      auto [slo, root] = solve_slow();
      std::cerr << x << ' ' << l << ' ' << r << ' ' << cur << ' ' << slo << ' ' << root << ' ' << num_verts << '\n';
      assert(slo == cur);
    }
  }
  v[x].clear();

  ans[x] = max(ans[x], cur);
  if (l < r) {
    int mid = (l + r) / 2;
    solve(x * 2, l, mid, cur);
    solve(x * 2 + 1, mid + 1, r, cur);
  }

  while (tot1 > m1)
    *buf1[tot1] = val1[tot1], tot1--;
  while (tot2 > m2)
    *buf2[tot2] = val2[tot2], tot2--;
  while (tot3 > m3)
    *buf3[tot3] = val3[tot3], tot3--;
}
void print(int x, int l, int r) {
  if (!(l <= 30836 && 30836 <= r))
    return;
  if (l == r) {
    printf("%lld\n", ans[x]);
    return;
  }
  ans[x + x] = max(ans[x + x], ans[x]);
  ans[x + x + 1] = max(ans[x + x + 1], ans[x]);
  int mid = (l + r) / 2;
  print(x + x, l, mid);
  print(x + x + 1, mid + 1, r);
}
struct rng {
  int id, l, r;
};
vector<rng> vq[200005];
int main(int argc, char const *argv[]) {
  /*
  std::mt19937 mt(std::atoi(argv[1]));
  n = std::uniform_int_distribution<int>(2, 20)(mt);
  q = 0;
  fz1(i, n) col[i] = i;
  fz(i, 2, n) parent[i] = std::uniform_int_distribution<int>(1, i - 1)(mt);
  fz(i, 2, n) edge_color[i] = std::uniform_int_distribution<int>(1, 2)(mt);
  fz(i, 2, n) {
    int a = 1;
    for (int z = 0; z < i-2; z++) { a %= 999999999; a *= 10; }
    fw[i] = a;
  }
  */
  read(n);
  read(q);
  fz1(i, n) read(col[i]);
  fz(i, 2, n) read(parent[i]);
  fz(i, 2, n) read(edge_color[i]);
  fz(i, 2, n) read(fw[i]);

  fz(i, 2, n) {
    bi[i].push_back((edg){parent[i], fw[i], edge_color[i]});
    bi[parent[i]].push_back((edg){i, fw[i], edge_color[i]});
  }
  dfs1(1, 0);
  dfs2(1, 1);
  fz1(j, 18) fz1(i, n - (1 << j) + 1) f[j][i] =
      min(f[j - 1][i], f[j - 1][i + (1 << (j - 1))]);
  fz(i, 2, n) lg[i] = lg[i/2] + 1;
  fz1(i, n) {
    for (auto e : bi[i]) {
      if (e.dest == parent[i])
        continue;
      if (!lst[e.c]) {
        lst[e.c] = e.dest;
        assert(bel[e.dest] == 0);
      }
      bel[e.dest] = lst[e.c];
    }
    for (auto e : bi[i]) {
      if (e.dest == parent[i])
        continue;
      lst[e.c] = 0;
    }
  }
  /*
  fz1(i, n) dsu[i] = cc_top[i] = i, dsz[i] = 1, cc_diam[i] = (node){i, i, 0};
  auto solve_slow = [&]() -> int64_t {
    std::vector<int64_t> dp_up(n+1, 0);
    std::vector<std::map<int, int64_t>> dp_any(n+1);
    int64_t ans = 0;
    auto setmax = [](int64_t& a, int64_t b) { if (b > a) a = b; };
    for (int i = n; i >= 1; i--) {
      int p = parent[i];
      if (p == -1)
        break;
      int64_t w = dp_up[i] + fw[i];
      if (hv[i] && hv[p]) {
        auto &ov = dp_any[p][edge_color[i]];
        setmax(ans, ov + w);
        setmax(ov, w);
        if (edge_color[i] == edge_color[p]) {
          setmax(dp_up[p], w);
        }
      }
    }
    return ans;
  };
  std::vector<int> perm(n);
  std::iota(perm.begin(), perm.end(), 1);
  std::shuffle(perm.begin(), perm.end(), mt);
  i64 cur_ans = 0;
  cerr << n << ' ' << q << '\n';
  for (int i = 1; i <= n; i++) cerr << col[i] << " \n"[i==n];
  for (int i = 2; i <= n; i++) cerr << parent[i] << " \n"[i==n];
  for (int i = 2; i <= n; i++) cerr << edge_color[i] << " \n"[i==n];
  for (int i = 2; i <= n; i++) cerr << fw[i] << " \n"[i==n];
  for (int u : perm) {
    cerr << "turning on " << u << '\n';
    turn_on_vert(u, cur_ans);
    i64 slow_ans = solve_slow();
    cerr << "cur_ans " << cur_ans << '\n';
    cerr << "slow_ans " << slow_ans << '\n';
    assert(slow_ans == cur_ans);
  }
  */
  fz1(i, n) tim[i] = 0;
  fz1(i, q) {
    int x, c;
    read(x);
    read(c);
    vq[col[x]].push_back((rng){x, tim[x], i - 1});
    tim[x] = i;
    col[x] = c;
  }
  fz1(i, n) vq[col[i]].push_back((rng){i, tim[i], q});
  fz1(i, n) dsu[i] = cc_top[i] = i, dsz[i] = 1, cc_diam[i] = (node){i, i, 0};
  fz1(j, n) if (!vq[j].empty()) {
    cerr << "color " << j << ":\n";
    if (j != 1064)
      continue;
    num_verts = 0;
    ff(vq[j], it) update(1, 0, q, it->l, it->r, it->id);
    solve(1, 0, q, 0);
  }
  print(1, 0, q);
  return 0;
}
