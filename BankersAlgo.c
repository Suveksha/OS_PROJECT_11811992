#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
int main()
{
int process,resource,current;
int avail[resource];
int max[process][resource];
int allocated[process][resource];
int n[process][resource];
int safe[process];
int complete = 0;


printf("\t Enter the total number of resources\n");
scanf("%d",&resource);
printf("\n\tEnter the total number of processes\n");
scanf("%d",&process);

printf("\t Enter the values for allocation of Resources of the process\n");
int i=0;
for(i=0;i<process;i++)
{
printf("\tAllocation for P%d:\n",(i));
int j=0;
for(j=0;j<resource;j++)
{
printf("\tAllocation for R%d:\n",(j));
scanf("%d",&allocated[i][j]);
}
}

printf("\tEnter the values for the Available Resources of the process\n");
int q;
for(q=0;q<resource;q++)
{
printf("\tR%d:\n",(i));
scanf("%d",&avail[i]);
}

printf("\tEnter the values for Maximum allocation of resources of the process\n");
int w;
for(w=0;w<resource;w++)
{
printf("P%d:\n",(i));
int x;
for(x=0;x<resource;x++)
{
printf("R%d:\n",(x));
scanf("%d",&max[w][x]);
}
}
int t;
for(t=0;t<resource;t++)
{
int m=avail[t];//total
int z;
for(z=0;z<resource;z++)
{
    m=m+allocated[z][t];
}
int p;
for(p=0;p<process;p++)
{
if(m<max[z][t])
{
printf("\t The Maximum allocated value of P%d of R%d is greater than the instances of R%d",z,t,t);
return 0;
}
}
}
int a;
for(a=0;a<process;a++)
{
int b;
for(b=0;b<resource;b++)
{
n[a][b]=max[a][b]-allocated[a][b];
}
}

bool flag[process];
int h;
for (h=0; h<process; h++)
{
flag[h] = false;
}

int c=0;
int l;
for (l= 0; l<process; l++)
{
safe[l] = 0;
}

do
{
current=-1;
int g;
for (g = 0; g<process; g++)
{
if (!flag[g])
{
current=i;
int it;
for(it=0; it<resource; it++)
{
if (avail[it]<n[g][it])
{
current=-1;
break;
}
}
}
if (current!=-1)
{
break;
}
}

if (current!= -1)
{
printf("\tP%d is executing",current);
safe[c] = current;
c++;
int v;
for( v=0; v<resource; v++)
{
if (avail[v] < n[i][v])
{
current = -1;
break;
}
}
}

if (current!= -1)
{
 break;
}


if(current!= -1)
{
printf("\tP%d is executing.....", current);

safe[c] = current;
c++;
int e;
for(e = 0; e < resource; e++)
{
avail[e] = avail[e] +allocated[current][e];

allocated[current][e] = 0;
max[current][e] = 0;
n[current][e] = 0;
flag[current] = true;
}
}
}
while (c!=process && current!= -1);
if (c ==process)
{
printf("\n\t Safe Sequence found:");
int f;
for (f = 0; f < c; f++)
{
if (f != c- 1)
printf(" P%d ", safe[i]);
else
printf(" P%d", safe[i]);
}
}
 else
{
printf("\n\tNO SAFE SEQUENCE FOUND ");
}
}
