#include <bits/stdc++.h>

using namespace std;

typedef float fl;
int main(int argc, char **argv) {

	float score;
	int cnt = 0;
	FILE *fres = fopen(argv[2], "r");
	FILE *fout = fopen(argv[3], "r");

	int a, b;
	for(int i = 0; i < 100000; i++)
	{
		fscanf(fres, "%d", &a);
		fscanf(fout, "%d", &b);
		if(a == b) cnt++;
	}
	
	if(cnt <= 50000)
	{
		score = 0;
	}
	else if(cnt <= 74000)
	{
		score = fl(15)*fl(cnt - 50000)/fl(24000);
	}
	else if(cnt <= 99000)
	{
		score = fl(20) + fl(15)*fl(cnt - 74000)/fl(25000);
	}
	else if(cnt <= 99900)
	{
		score = fl(40) + fl(30)*fl(cnt - 99000)/fl(900);
	}
	else if(cnt <= 99990)
	{
		score = fl(80) + fl(20)*fl(cnt - 99900)/fl(90);
	}
	else
	{
		score = 100;
	}
	score /= 100;
	fprintf(stderr, "You got %d correct answers!\n", cnt);
	printf("%.4f\n", score);
	return 0;

}
