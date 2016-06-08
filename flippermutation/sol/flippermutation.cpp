#include <bits/stdc++.h>
#include "flippermutation.h"

using namespace std;

typedef long long ll;
typedef vector<ll> vi;
typedef pair<ll,ll> ii;
typedef pair<ll, ii> iii;
typedef long double ld;

#define fi first
#define se second
#define pb push_back
#define mp make_pair

const ll MAXN = 5001;
const ll MOD = 1e9 + 7;

ll p[MAXN];
vector<ll> cycles;
bool visited[MAXN];
ll cyclenum[MAXN];
ll cyclecnt[MAXN];
ll cyclenumber;
ll cyclelength;

void dfs(ll u)
{
	cyclenum[u] = cyclenumber;
	visited[u] = true; cyclelength++;
	if(!visited[p[u]])
	{
		dfs(p[u]);
	}
}

ll modpow(ll a, ll b)
{
	ll r = 1;
	while(b)
	{
		if(b&1) r = (r*a)%MOD;
		a = (a*a)%MOD;
		b >>= 1;
	}
	return r;
}

ll mult(ll a, ll b)
{
	ll r = (a*b)%MOD;
	if(r < 0) r += MOD;
	return r;
}

ll divide(ll a, ll b)
{
	ll r = modpow(a, MOD - 2);
	return mult(a, r);
}

iii powers[MAXN+1];
vector<ll> primes;
ll lcm;
ll tmp;
bool prime[MAXN+1];

void Sieve(ll n)
{	
	memset(prime, 1, sizeof(prime));
	prime[1] = 0;
	for(ll i = 2; i <= n; i++)
	{
		if(prime[i])
		{
			primes.pb(i);
			for(ll j = 2*i; j <= n; j += i)
			{
				prime[j] = false;
			}
		}
	}
}

map<ll, ll> factors;

void clr()
{
	factors.clear();
}

void factorize(ll x)
{
	clr();
	for(ll i = 0; primes[i]*primes[i] <= x; i++)
	{
		while(x % primes[i] == 0)
		{
			factors[primes[i]]++;
			x /= primes[i];
		}
	}
	if(x > 1)
	{
		factors[x]++;
	}
}

void add(ll x)
{
	tmp = 1;
	factorize(x);
	for(map<ll,ll>::iterator it = factors.begin(); it != factors.end(); it++)
	{
		ll p = it->fi; ll occ = it->se;
		if(occ >= powers[p].fi)
		{
			tmp = mult(tmp, modpow(p, occ - powers[p].fi));
			powers[p].se.se = powers[p].se.fi;
			powers[p].se.fi = powers[p].fi;
			powers[p].fi = occ;
		}
		else if(occ >= powers[p].se.fi)
		{
			powers[p].se.se = powers[p].se.fi;
			powers[p].se.fi = occ;
		}
		else if(occ >= powers[p].se.se)
		{
			powers[p].se.se = occ;
		}
	}
}

void remove(ll x)
{
	tmp = 1;
	factorize(x);
	for(map<ll,ll>::iterator it = factors.begin(); it != factors.end(); it++)
	{
		ll p = it->fi; ll occ = it->se;
		if(occ >= powers[p].fi)
		{
			//cout << powers[p].fi << ' ' << powers[p].se.fi << ' '<< powers[p].se.se << endl;
			ll val = powers[p].fi;
			powers[p].fi = powers[p].se.fi;
			powers[p].se.fi = powers[p].se.se;
			powers[p].se.se = 0;
			val = powers[p].fi - val + MOD - 1;
			tmp = mult(tmp, modpow(p, val));
		}
		else if(occ >= powers[p].se.fi)
		{
			powers[p].se.fi = powers[p].se.se;
			powers[p].se.se = 0;
		}
		else if(occ >= powers[p].se.se)
		{
			powers[p].se.se = 0;
		}
	}
}

void clearall()
{
	memset(p, 0, sizeof(p));
	for(int i = 0; i < MAXN + 1; i++)
	{
		powers[i] = iii(0, ii(0, 0));
	}
	tmp = 1;
	clr();
	primes.clear();
	lcm = 1;
	cyclelength = 0;
	cyclenumber = 0;
	memset(cyclecnt, 0, sizeof(cyclecnt));
	cycles.clear();
	memset(visited, false, sizeof(visited));
	memset(cyclenum, 0, sizeof(cyclenum));
}

int flip(int N, int P[])
{
	int n = N;
	for(ll i = 1; i <= n; i++)
	{
		p[i] = P[i];
	}
	for(ll i = 1; i <= n; i++)
	{
		if(!visited[i])
		{
			cyclelength = 0;
			dfs(i);
			cycles.pb(cyclelength);
			cyclecnt[cyclenumber] = cyclelength;
			cyclenumber++;
		}
	}
	Sieve(MAXN);
	lcm = 1;
	ll ans = 0;
	ll C = cycles.size();
	for(ll i = 0; i < C; i++)
	{
		add(cycles[i]);
		lcm = mult(lcm, tmp);
	}
	ans = lcm;
	//Try all possible swaps
	//Step 1 : Split
	ll tmpans;
	for(ll i = 0; i < C; i++)
	{
		ll c = cycles[i];
		for(ll j = 1; j <= c/2; j++)
		{
			ll k = c - j;
			if(k <= 0) continue;
			remove(c);
			tmpans = mult(lcm, tmp);
			add(j);
			tmpans = mult(tmpans, tmp);
			add(k);
			tmpans = mult(tmpans, tmp);
			ans = max(ans, tmpans);
			//cout << tmpans << endl;
			remove(k);
			remove(j);
			add(c);
		}
	}
	//Step 2 : Merge
	for(ll i = 0; i < C; i++)
	{
		for(ll j = i + 1; j < C; j++)
		{
			ll a = cycles[i];
			ll b = cycles[j];
			remove(a);
			tmpans = mult(lcm, tmp);
			remove(b);
			tmpans = mult(tmpans, tmp);
			add(a + b);
			tmpans = mult(tmpans, tmp);
			ans = max(ans, tmpans);
			remove(a + b);
			add(a);
			add(b);
		}
	}
	return int(ans);
}

