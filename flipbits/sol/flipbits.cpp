#include <bits/stdc++.h>
#include "flipbits.h"

using namespace std;

typedef long long ll;
typedef vector<int> vi;
typedef pair<int,int> ii;
typedef long double ld;

#define fi first
#define se second
#define pb push_back
#define mp make_pair

const int NN = 201;

ld prob[NN+1][2];

ld reci(int x)
{
	ld xx = ld(x);
	ld r = ld(1)/ld(xx);
	return r;
}

void fillprob(int n)
{
	prob[n-1][1] = log10(reci(n));
	prob[n-1][0] = log10(ld(1) - reci(n));
	//cout << n - 1 << ' ' << fixed << setprecision(11) << ' ' << prob[n-1][0] << ' ' << prob[n-1][1] << endl;
	for(int i = n - 2; i >= 0; i--)
	{
		if(i == 0)
		{
			prob[i][0] = prob[i+1][1];
			prob[i][1] = prob[i+1][0];
		}
		else
		{
			ld x = log10(ld(i+1));
			ld p1 = prob[i+1][1];
			ld q1 = prob[i+1][1] + log10(ld(i));
			ld p0 = prob[i+1][0];
			ld q0 = prob[i+1][0] + log10(ld(i));
			//prob[i][0] = q0 - x;
			//prob[i][1] = q1 - x;
			prob[i][0] = p1 + log10(ld(1) + pow(ld(10), q0 - p1)) - x;
			prob[i][1] = p0 + log10(ld(1) + pow(ld(10), q1 - p0)) - x;
		}
		//cout << i << ' ' << fixed << setprecision(11) << ' ' << prob[i][0] << ' ' << prob[i][1] << endl;
	}
}

ld log102n(int n)
{
	ld ans = 0;
	for(int i = 0; i < n; i++)
	{
		ans += log10(ld(0.5));
	}
	return ans;
}

void initialize()
{
	fillprob(200);
}

int guess(char S[]) //99.99%
{
	int n = 200;
	ld p2 = 0;
	for(int i = 0; i < n; i++)
	{
		int bit = S[i] - '0';
		p2 += prob[i][bit];
		p2 += log10(ld(2));
	}
	if(p2 < 1)
	{
		return 1;
	}
	else
	{
		return 2;
	}
}
