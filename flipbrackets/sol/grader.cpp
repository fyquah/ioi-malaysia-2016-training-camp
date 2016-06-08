#include <bits/stdc++.h>
#include "flipbrackets.h"

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

const int NN = 200001;
const int QQ = 200001;

int parentnode[NN];
char charedge[NN];

int main()
{
	int n, q; cin>>n>>q;//scanf("%d %d", &n, &q);
	for(int i = 1; i <= n - 1; i++)
	{
		//read input
		cin>>parentnode[i]>>charedge[i];
	}
	initialize(n, parentnode, charedge);
	//cout << "INITIALIZED" << endl;
	for(int i = 0; i < q; i++)
	{
		int uu, vv; char tt;
		cin>>tt>>uu>>vv;
		if(tt=='F') flipbrackets(uu, vv);
		else cout << querypath(uu, vv) << endl;
	}
	return 0;
}

