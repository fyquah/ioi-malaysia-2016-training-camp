#include <bits/stdc++.h>

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

int p[NN][2];

int flip(int N, int P[])
{
	int ans = 0;
	int cnt = 0;
	for(int i = 1; i <= N; i++)
	{
		for(int j = i + 1; j <= N; j++)
		{
			swap(P[i], P[j]);
			cnt = 0;
			for(int k = 1; k <= N; k++)
			{
				p[k][0] = P[k];
			}
			while(1)
			{
				cnt++;
				bool isperm = true;
				for(int k = 1; k <= N; k++)
				{
					p[k][cnt&1] = P[p[k][(cnt+1)&1]];
					if(p[k][cnt&1] != P[k]) isperm = false;
				}
				if(isperm) break;
			}
			ans = max(ans, cnt);
			swap(P[i], P[j]);
		}
	}
	cnt = 0;
	for(int k = 1; k <= N; k++)
	{
		p[k][0] = P[k];
	}
	while(1)
	{
		cnt++;
		bool isperm = true;
		for(int k = 1; k <= N; k++)
		{
			p[k][cnt&1] = P[p[k][(cnt+1)&1]];
			if(p[k][cnt&1] != P[k]) isperm = false;
		}
		if(isperm) break;
	}
	ans = max(ans, cnt);
	return ans;
}

