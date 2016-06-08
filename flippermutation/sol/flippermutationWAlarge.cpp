#include <bits/stdc++.h>
#include "flippermutation.h"

using namespace std;

typedef long long ll;
typedef vector<int> vi;
typedef pair<int,int> ii;
typedef pair<int, ii> iii;
typedef long double ld;

#define fi first
#define se second
#define pb push_back
#define mp make_pair

const int NN = 5001;
const int MOD = 1e9 + 7;

int p[NN];
vector<int> cycles;
bool visited[NN];
int cyclenum[NN];
int cyclecnt[NN];
int cyclenumber;
int cyclelength;

void dfs(int u)
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

iii powers[NN+1];
vector<int> primes;
ll lcm;
ll tmp;
bool prime[NN+1];

void Sieve(int n)
{	
	memset(prime, 1, sizeof(prime));
	prime[1] = 0;
	for(int i = 2; i <= n; i++)
	{
		if(prime[i])
		{
			primes.pb(i);
			for(int j = 2*i; j <= n; j += i)
			{
				prime[j] = false;
			}
		}
	}
}

map<int, int> factors;

void clr()
{
	factors.clear();
}

void factorize(int x)
{
	clr();
	for(int i = 0; primes[i]*primes[i] <= x; i++)
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

void add(int x)
{
	tmp = 1;
	factorize(x);
	for(map<int,int>::iterator it = factors.begin(); it != factors.end(); it++)
	{
		int p = it->fi; int occ = it->se;
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

void remove(int x)
{
	tmp = 1;
	factorize(x);
	for(map<int,int>::iterator it = factors.begin(); it != factors.end(); it++)
	{
		int p = it->fi; int occ = it->se;
		if(occ >= powers[p].fi)
		{
			//cout << powers[p].fi << ' ' << powers[p].se.fi << ' '<< powers[p].se.se << endl;
			int val = powers[p].fi;
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

int gcd(int a ,int b)
{
   if(b==0) return a;
   a%=b;
   return gcd(b,a);
}

int L(int a, int b)
{
	ll res = ll(a)*ll(b);
	res /= gcd(a, b);
	return res;	
}

int flip(int N, int P[])
{
	int n = N;
	for(int i = 1; i <= n; i++)
	{
		p[i] = P[i];
	}
	for(int i = 1; i <= n; i++)
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
	Sieve(NN);
	lcm = 1;
	ll ans = 0;
	int C = cycles.size();
	for(int i = 0; i < C; i++)
	{
		lcm = L(lcm, cycles[i]);
	}
	ans = lcm;
	//Try all possible swaps
	//Step 1 : Split
	ll tmpans;
	for(int i = 0; i < C; i++)
	{
		int c = cycles[i];
		for(int j = 1; j <= c/2; j++)
		{
			int k = c - j;
			if(k <= 0) continue;
			tmpans = 1;
			for(int x = 0; x < C; x++)
			{
				if(x != i)
				{
					tmpans = L(tmpans, cycles[x]);
				}
				else
				{
					tmpans = L(tmpans, k);
					tmpans = L(tmpans, j);
				}
			}
			//cout << j << ' ' << k << ' ' << tmpans << endl;
			ans = max(ans, tmpans);
		}
	}
	//Step 2 : Merge
	for(int i = 0; i < C; i++)
	{
		for(int j = i + 1; j < C; j++)
		{
			int a = cycles[i];
			int b = cycles[j];
			tmpans = 1;
			for(int x = 0; x < C; x++)
			{
				if(x != i && x != j) 
				{
					tmpans = L(tmpans, cycles[x]);
				}
			}
			tmpans = L(tmpans, a + b);
			ans = max(ans, tmpans);
		}
	}
	return ans;
}
