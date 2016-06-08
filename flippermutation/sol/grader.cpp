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

const int MAXMUMN = 5001;

int permutationsss[MAXMUMN];

int main()
{
	int nnn; scanf("%d", &nnn);
	for(int i = 0; i < nnn; i++)
	{
		scanf("%d", &permutationsss[i+1]);
	}
	printf("%d\n", flip(nnn, permutationsss));
	return 0;
}
