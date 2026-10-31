#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
const ll MAXN = 110;

ll N, M;
int Q;

ll ac[MAXN], bc[MAXN];

ll dma[9] = {2, 1, 0, 1, 1, 0, 1, 1, 1};
ll cma[9] = {1, 1, 0, 1, 0, 0, 0, 0, 1};

ll cp[3][3];
ll np[3][3];
ll crpow[61][3][3];
ll drpow[61][3][3];

ll rpow[3][3];
ll nrpow[3][3];

typedef unsigned long long ull;
struct FastMod {
    ull b, m;
    FastMod(ull b) : b(b), m(-1ULL / b) {}
    ull reduce(ull a) { // a % b + (0 or b)
        ull v = a - (ull)((__uint128_t(m) * a) >> 64) * b; return (v >= b ? v - b : v);
    }
};

FastMod mm(1);

void pset()
{
    mm = FastMod(M);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            cp[i][j] = dma[3*i+j];
            rpow[i][j] = 0;
            rpow[i][i] = 1;
        }
    for (int d = 0; d <= 60; d++)
    {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                drpow[d][i][j] = cp[i][j];

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                np[i][j] = 0;
        for (int i = 0; i < 3; i++)
            for (int k = 0; k < 3; k++)
                for (int j = 0; j < 3; j++)
                    np[i][j] = mm.reduce(np[i][j] + cp[i][k] * cp[k][j]);
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                cp[i][j] = np[i][j];
    }

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            cp[i][j] = cma[3*i+j];
            rpow[i][j] = 0;
            rpow[i][i] = 1;
        }
    for (int d = 0; d <= 60; d++)
    {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                crpow[d][i][j] = cp[i][j];

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                np[i][j] = 0;
        for (int i = 0; i < 3; i++)
            for (int k = 0; k < 3; k++)
                for (int j = 0; j < 3; j++)
                    np[i][j] = mm.reduce(np[i][j] + cp[i][k] * cp[k][j]);
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                cp[i][j] = np[i][j];
    }
}

pair <ll, ll> dpow (ll nv, pair <ll, ll> ss)
{
    mm = FastMod(M);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            cp[i][j] = dma[3*i+j];
            rpow[i][j] = 0;
            rpow[i][i] = 1;
        }

    for (ll d = 0; d <= 60; d++)
    {
        if (nv & (1LL << d))
        {
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    nrpow[i][j] = 0;
            for (int i = 0; i < 3; i++)
                for (int k = 0; k < 3; k++)
                    for (int j = 0; j < 3; j++)
                        nrpow[i][j] = mm.reduce(nrpow[i][j] + drpow[d][i][k] * rpow[k][j]);
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    rpow[i][j] = nrpow[i][j];
        }
    }

    ll r1 = (ss.first * rpow[0][0] + ss.second * rpow[1][0] + rpow[2][0]) % M;
    ll r2 = (ss.first * rpow[0][1] + ss.second * rpow[1][1] + rpow[2][1]) % M;
    return make_pair (r1, r2);
}

pair <ll, ll> cpow (ll nv, pair <ll, ll> ss)
{
    mm = FastMod(M);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            cp[i][j] = cma[3*i+j];
            rpow[i][j] = 0;
            rpow[i][i] = 1;
        }

    for (ll d = 0; d <= 60; d++)
    {
        if (nv & (1LL << d))
        {
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    nrpow[i][j] = 0;
            for (int i = 0; i < 3; i++)
                for (int k = 0; k < 3; k++)
                    for (int j = 0; j < 3; j++)
                        nrpow[i][j] = mm.reduce(nrpow[i][j] + crpow[d][i][k] * rpow[k][j]);
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    rpow[i][j] = nrpow[i][j];
        }
    }

    ll r1 = (ss.first * rpow[0][0] + ss.second * rpow[1][0] + rpow[2][0]) % M;
    ll r2 = (ss.first * rpow[0][1] + ss.second * rpow[1][1] + rpow[2][1]) % M;
    return make_pair (r1, r2);
}

pair <ll, ll> gv (ll nv)
{
    pair <ll, ll> res = cpow (nv, make_pair (0, 1));
    res.first -= 2;
    res.second -= 1;
    return res;
}

pair <ll, ll> figure (ll x, int n) // n <= 90
{
    //cout << x << " " << n << endl;
    if (n == 2)
    {
        if (x == 2) return make_pair (1, 0);
        else return make_pair (0, 0);
    }
    if (n <= 1)
        return make_pair (0, 0);

    if (n % 2 == 1)
    {
        if (ac[n-2] >= x)
        {
            pair <ll, ll> rres = figure (x, n - 2);
            ll na = 2 * rres.first + rres.second;
            ll nb = rres.first + rres.second;
            return make_pair ((na + 1) % M, (nb + 1) % M);
        }
        x -= ac[n-2];

        if (ac[n-2] + bc[n-2] >= x)
        {
            pair <ll, ll> rres = figure (x, n - 2);
            ll na = 2 * rres.first + rres.second;
            ll nb = rres.first + rres.second;
            return make_pair (na % M, nb % M);
        }
        x -= (ac[n-2] + bc[n-2]);

        pair <ll, ll> rres = figure (x, n - 2);
        ll na = 2 * rres.first + rres.second;
        ll nb = rres.first + rres.second;
        return make_pair ((na + 1) % M, nb % M);       
    }
    else
    {
        if (x <= ac[n-2])
        {
            ll bloc = x;
            if (x == 1)
            {
                bloc = ac[n-4] + 1;
            }
            else if (x <= ac[n-4] + 1)
            {
                bloc = x - 1;
            }
            pair <ll, ll> rres = figure (bloc, n - 2);
            ll na = 2 * rres.first + rres.second;
            ll nb = rres.first + rres.second;
            return make_pair ((na + 1) % M, (nb + 1) % M);
        }
        x -= ac[n-2];

        if (x <= ac[n-2] + bc[n-2])
        {
            ll bloc = x;
            if (x == 1)
                bloc = ac[n-2] + 1;
            else if (x == 2)
                bloc = ac[n-4] + 1;
            else if (x <= ac[n-4] + 2)
                bloc = x - 2;
            else if (x <= ac[n-2] + 1)
                bloc = x - 1;
            pair <ll, ll> rres = figure (bloc, n - 2);
            ll na = 2 * rres.first + rres.second;
            ll nb = rres.first + rres.second;
            return make_pair (na % M, nb % M);
        }
        x -= ac[n-2] + bc[n-2];

        {
            ll bloc = x;
            if (x == 1)
                bloc = ac[n-2] + 1;
            else if (x == 2)
                bloc = ac[n-4] + 1;
            else if (x <= ac[n-4] + 2)
                bloc = x - 2;
            else if (x <= ac[n-2] + 1)
                bloc = x - 1;
            pair <ll, ll> rres = figure (bloc, n - 2);
            ll na = 2 * rres.first + rres.second;
            ll nb = rres.first + rres.second;
            return make_pair ((na + 1) % M, nb % M);
        }
    }
}

const int MAXV = 91;

ll gogo (ll x)
{
    pair <ll, ll> res;
    if (N % 2 == 1)
    {
        if (N > MAXV)
        {
            pair <ll, ll> pres = figure (x, MAXV);
            res = dpow ((N - MAXV) / 2, pres);
        }
        else
            res = figure (x, N);
        return (res.first + res.second + 1) % M;
    }
    else
    {
        if (N > MAXV)
        {
            ll nrun = (N - (MAXV - 1)) / 2;
            if (nrun >= x)
            {
                // heh
                ll nlast = (N - (x - 1) * 2);
                pair <ll, ll> pres = gv (nlast);
                res = dpow ((x - 1), pres);
            }
            else
            {
                pair <ll, ll> pres = figure (x - nrun, MAXV - 1);
                res = dpow (nrun, pres);
            }
        }
        else
            res = figure (x, N);
        return (res.first + res.second + 1) % M;
    }
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    ac[0] = 0;
    bc[0] = 1;
    for (int i = 1; i < 92; i++)
    {
        ac[i] = ac[i-1] + bc[i-1];
        bc[i] = ac[i-1];

        //cout << i << " " << ac[i] << " " << bc[i] << "\n";
    }
    //M = 1e18;
    //pair <ll, ll> pp = gv (90);
    //cout << pp.first << " " << pp.second << "\n";

    while (cin >> N)
    {
        cin >> Q >> M;
        pset();

        for (int i = 0; i < Q; i++)
        {
            ll x; cin >> x;
            if (i) cout << " ";
            cout << gogo (x);
        }
        cout << "\n";
    }
}
