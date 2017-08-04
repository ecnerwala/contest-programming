#include <cmath>
#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
typedef double ld;

int N, M;
ll A, B;

struct num {
    ll pow;
    ld val;
    num& operator *= (ld x) {
        val *= x;
        while (val >= 10) {
            val /= 10;
            pow ++;
        }
        return *this;
    }
    num& operator /= (ld x) {
        val /= x;
        while (val < 1) {
            val *= 10;
            pow --;
        }
        return *this;
    }
    num& operator /= (const num& a) {
        pow -= a.pow;
        return *this /= a.val;
    }
    num& operator += (const num& a) {
        ll np = max(pow, a.pow);
        ld v = a.val / std::pow(10, np - a.pow);
        val /= std::pow(10, np - pow);
        //cerr << val << ' ' << v << '\n';
        pow = np;
        val += v;
        if (val >= 10) {
            val /= 10;
            pow++;
        }
        return *this;
    }

    num(ld a) {
        val = a;
        pow = 0;
        while (val >= 10) {
            val /= 10;
            pow++;
        }
        while (val < 1) {
            val *= 10;
            pow--;
        }
    }

    num() { }

    void p(int p) {
        pow *= p;
        val = std::pow(val, p);
        assert(val >= 1);
        while (val >= 10) {
            val /= 10;
            pow++;
        }
    }

};

num ipow (ll x, int p)
{
    //cerr << x << ' ' << p << '\n';
    num ans = 1.0;
    ans /= x;
    ans.p(p);
    //cerr << ans.val << 'E' << ans.pow << '\n';
    return ans;
}

void run()
{
    cin >> N >> A >> B >> M;
    int tot = 0;
    for (int i = 0; i < N; i++)
    {
        int x; cin >> x;
        tot += x;
    }
    //cerr << M << '\n';
    if (!M) tot += 2;

    num works, tval;
    int st = M ? M : (N + int(1e5));
    for (int i = st; i >= N; i--) {
        num d = ipow (i, tot);
        for (int j = 0; j < N; j++)
            d *= (i - j);
        //cerr << d.val << 'E' << d.pow << '\n';

        if (i == st) {
            tval = d;
        } else {
            //cerr << "adding" << '\n';
            tval += d;
        }
        //cerr << tval.val << 'E' << tval.pow << '\n';
    }
    if (N == tot) {
        num d = 1.0;
        d /= st;
        tval += d;
    }
    if (B < N) {
        cout << 0. << '\n';
        return;
    }
    A = max(A, ll(N));
    for (ll i = B; i >= A; i--) {
        //cerr << i << '\n';
        num d = ipow (i, tot);
        for (int j = 0; j < N; j++)
            d *= (i - j);

        if (i == B) {
            works = d;
        } else {
            works += d;
        }
    }
    /*
    for (int i = N; i <= M; i++)
    {
        double d = ipow (i, tot);
        for (int j = 0; j < N; j++)
            d *= (i - j);

        if (i >= A && i <= B)
            works += d;
        tval += d;
    }
    */

    works /= tval;
    assert(1 <= works.val && works.val < 10);
    cout << works.val << 'E' << works.pow << '\n';
    //cout << works / tval << "\n";
}

int main()
{
    cout << fixed << setprecision(10);
    int t; cin >> t;
    for (int i = 0; i < t; i++)
        run();
}
