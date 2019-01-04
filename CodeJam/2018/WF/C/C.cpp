#include<bits/stdc++.h>
using namespace std;

using ll = long long;
const int MAXS = 1.1e5;
int S;

const int MAXV = 1e6;
const int G = 25;

int scanint() {
    int v;
    int r = scanf("%d", &v);
    assert(r);
    return v;
}

int num_sent;
int num_queued;
int guesses[MAXS];
bool answers[MAXS];
void flush_guesses() {
    assert(num_sent <= num_queued);
    if (num_sent == num_queued) return;
    ostringstream stream;
    for (int i = num_sent; i < num_queued; i ++) {
        stream << guesses[i] << '\n';
    }
    printf("%s", stream.str().c_str());
    fflush(stdout);
    for (int i = num_sent; i < num_queued; i++) {
        int v = scanint();
        if (v == -1) exit(0);
        answers[i] = bool(v);
    }
    num_sent = num_queued;
}
void guess(int v) {
    assert(1 <= v && v <= MAXV);
    assert(num_queued + 1 <= S);
    guesses[num_queued] = v;
    num_queued++;
}
void guess_to(int v, int n) {
    while (num_queued < n) {
        guess(v);
    }
}

struct frac {
    int n, d;
    frac() : frac(0) {}
    frac(int a) : frac(a, 1) {}
    frac(int a, int b) : n(a), d(b) {
        assert(d != 0);
        if (d < 0) {
            n = -n, d = -d;
        }
        int g = __gcd(n, d);
        n /= g, d /= g;
    }
    friend bool operator == (const frac &a, const frac &b) {
        return a.n == b.n && a.d == b.d;
    }
    friend bool operator != (const frac &a, const frac &b) {
        return a.n != b.n || a.d != b.d;
    }
    friend bool operator < (const frac &a, const frac &b) {
        return (a.n * b.d) < (b.n * a.d);
    }
    friend bool operator <= (const frac &a, const frac &b) {
        return (a.n * b.d) <= (b.n * a.d);
    }
    friend bool operator > (const frac &a, const frac &b) {
        return (a.n * b.d) > (b.n * a.d);
    }
    friend bool operator >= (const frac &a, const frac &b) {
        return (a.n * b.d) >= (b.n * a.d);
    }
    friend std::ostream& operator << (std::ostream& out, const frac &f) {
        return out << f.n << '/' << f.d;
    }
};

int next_mult(int a, int m) {
    a += m-1;
    return a - a % m;
}

set<int> possible;
frac check(int v) {
    //cerr << "checking " << v << '\n';
    int st = num_queued;
    int en = st;
    for (int d : possible) {
        en = max(en, next_mult(st, d) + d);
    }

    guess_to(v, en);
    flush_guesses();
    assert(num_sent == en);

    set<frac> fracs;
    set<int> npossible;
    for (int d : possible) {
        int dst = next_mult(st, d);
        int cnt = 0;
        for (int i = dst; i < dst + d; i++) {
            cnt += answers[i];
        }
        fracs.emplace(cnt, d);
        npossible.insert(d);
    }
    assert(!fracs.empty());
    if (fracs.size() == 1) {
        goto end;
    }
    //cerr << "doing full check" << '\n';
    //cerr << "possible:"; for (int d : possible) { cerr << ' ' << d; } cerr << '\n';
    // Otherwise, we'll just narrow it down
    for (int d1 : possible) {
        for (int d2 : possible) {
            // we need lcm(d1, d2) to disprove d1 vs d2
            int l = d1 / __gcd(d1, d2) * d2;
            en = max(en, next_mult(st, l) + l);
        }
    }
    guess_to(v, en);
    flush_guesses();
    assert(num_sent == en);

    fracs.clear();
    npossible.clear();
    for (int d : possible) {
        int dst = next_mult(st, d);
        int cnt = 0;
        for (int i = dst; i < dst + d; i++) {
            cnt += answers[i];
        }
        bool good = true;
        for (int a = dst + d; a + d <= en; a += d) {
            int cur = 0;
            for (int i = a; i < a + d; i++) {
                cur += answers[i];
            }
            if (cur != cnt) {
                good = false;
                break;
            }
        }
        if (!good) continue;
        fracs.emplace(cnt, d);
        npossible.emplace(d);
    }
    assert(!fracs.empty());
    assert(!npossible.empty());
end:
    possible = npossible;
    assert(fracs.size() == 1);
    frac res = *fracs.begin();
    //cerr << v << ' ' << res << '\n';
    return res;
}

void search(int mi, frac a, int ma, frac b) {
    assert(mi < ma);
    assert(a <= b);
    if (a == b) return;
    if (ma - mi == 1) {
        return;
    }
    int md = (mi + ma) / 2;
    frac c = check(md);
    search(mi, a, md, c);
    search(md, c, ma, b);
}

int go() {
    num_sent = num_queued = 0;
    for (int i = 2; i <= G; i++) {
        possible.insert(i);
    }
    search(0, frac(0), MAXV+1, frac(1));
    assert(!possible.empty());
    // the result is the smallest possible
    int res = *possible.begin();
    set<int> oposs;
    for (int i = res; i <= G; i += res) {
        oposs.insert(i);
    }
    assert(possible == oposs);
    possible.clear();
    return res;
}

int main() {
    int T = scanint();
    for (int i = 0; i < T; i++) {
        S = scanint();
        if (S == -1) {
            exit(0);
        }
        assert(S == int(1e5));
        int res = go();
        assert(2 <= res && res <= G);
        printf("%d\n", -res);
        fflush(stdout);
    }
    return 0;
}
