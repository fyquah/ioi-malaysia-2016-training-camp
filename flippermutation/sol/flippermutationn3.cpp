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

ll lcm;

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
	for(int i = 1; i <= n; i++)
	{
		for(int j = i+1; j <= n; j++)
		{
			int tmp = p[j];
			p[j] = p[i];
			p[i] = tmp;
			cycles.clear();
			memset(visited, false, sizeof(visited));
			cyclenumber = 0;
			memset(cyclecnt, 0, sizeof(cyclecnt));
			for(int k = 1; k <= n; k++)
			{
				if(!visited[k])
				{
					cyclelength = 0;
					dfs(k);
					cycles.pb(cyclelength);
					cyclecnt[cyclenumber] = cyclelength;
					cyclenumber++;
				}
			}
			tmpans = 1;
			int C = cycles.size();
			for(int k = 0; k < C; k++)
			{
				tmpans = L(tmpans, cycles[k]);
			}
			tmp = p[j];
			p[j] = p[i];
			p[i] = tmp;
			ans = max(ans, tmpans);
		}
	}
	return ans;
}
