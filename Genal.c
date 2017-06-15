#include <stdio.h>
#include <stdlib.h>
#define MCH 100 //defines max string size
#define POP 100//defines population size
#define mutation 1//mutation in percent

typedef struct{
	char item[MCH];
	int fitn;
}DATA;

//global variables ---------
DATA mainl;//main line
int size;//size of main line
DATA pop[POP];//population matrix
int psize = -1;//population size
int tfit;//total fitness
DATA hfit;//highest fitness
int gen;//counts the generation
DATA parefit;
char phfit[MCH];
//--------------------------

void readfile(const char* filename)//reads file for main line, sets main line.
{
	size = 0;
	FILE *infile;
	infile = fopen(filename, "r");
	
	while(fscanf(infile, "%c", &mainl.item[size])!=EOF)
	{
		size++;
	}
	if(size<3)
	{
		printf("size too small, has to be larger than three characters");
		return;
	}
}

void fitness(void)//gets fitness of whole population
{
	tfit = 0;
	hfit.fitn = 0;
	if(psize == -1)
	{
		printf("not initialized");
		return;
	}
	int i, j, k,tmp;
	for(i = 0; i < POP; i++)
	{
		tmp = 0;
		for(j = 0; j<size; j++)
		{
			if(pop[i].item[j] == mainl.item[j])tmp++;
		}
		if(hfit.fitn < tmp)
		{
			hfit.fitn = tmp;
			for(k = 0; k < size; k++)
			{
				hfit.item[k] = pop[i].item[k];
			}
		} 
		pop[i].fitn = tmp;//assigns individual fitness
		tfit = tfit + tmp;//gets total fitness
	}
}

void init(void)//initializes data, fill population with random characters. assigns fitness.
{
	int i,j;
	psize = 0;
	gen = 0;
	for(i =0; i<POP; i++)
	{
		for(j = 0; j < size; j++)
		{
			pop[i].item[j] = rand()%256;
		}
		psize++;
	}
	fitness();
}

void reorg(void)//reorganizes pop to prefer larger fitness
{

	DATA tmp[POP];//temp pop for now, will be new population
	int fitr;//fitness ratio
	int i,j,k,infit;//individual fitness space and indicies
	fitr = POP/tfit;
	j = 0;
	for(k = 0; k<POP; k++)//makes sure there is no empty space in tmp
	{
		tmp[k] = pop[k];
	}


	for(i =0; i < POP; i++)//makes tmp data
	{
		infit = fitr*(pop[i].fitn);//gets fitness for every pop element
		while(infit != 0)
		{
			tmp[j] = pop[i];
			j++;
			infit--;//counts down eact time
		}
	}
	for(k = 0; k<POP; k++)//copies everything back
	{
		pop[k] = tmp[k];
	}
	
	
}
void crossover(int val)
{
	int p1, p2;//index for parent 1 and parent 2
	int crt,crt2;//number of char per crossover
	char child[MCH];//creats child
	int i, j;
	crt = rand()%size;
	crt2 = rand()%size;
	while(crt > crt2)
	{
		crt = rand()%size;
		crt2 = rand()%size;	
	}
	p1 = rand()%100;
	p2 = rand()%100;
	while(pop[p1].item == pop[p2].item)//ensures both parents are different
	{
		p2 = rand()%100;
	}
	if(pop[p1].fitn>pop[p2].fitn)
	{
		parefit = pop[p1];
	}else{
		parefit = pop[p2];
	}
	j = 0;
	for(i = 0; i < size; i++)//crossover
	{
		child[i] = pop[p2].item[i];
		if(i==crt)
		{
			child[i] = pop[p1].item[i];
		}
		if(i == crt2)
		{
			child[i] = pop[p2].item[i];
		}

	}
	for(i = 0; i < size; i++)//reinitializes i, copies child into pop item with index, val
	{
		pop[val].item[i] = child[i];
	}
}


void mutate(void)//mutates population
{
	int mlet;//number of letters to mutate
	int mpop;//number of population to mutate
	int i,j,ra,le;
	mlet = (mutation*size)/100 + 1;
	mpop = (POP*mutation)/100 + 1;
	if(mutation == 100)
	{
		mlet--;
		mpop--;
	}
	for(i = 0; i < mpop; i++)
	{
		ra = rand()%POP;//selects random member to mutate
		for(j = 0; j < mlet; j++)
		{
			le = rand()%size;//selects random letter to mutate
			pop[ra].item[le] = rand()%256;
		}
	}
}

void crossmut(void)//Makes new population and gets their fitness 
{
	int start;
	for(start = 0; start<POP; start++)
	{
		crossover(start);
		if(pop[start].fitn < parefit.fitn)pop[start]=parefit;
	}
	mutate();
	fitness();
}


int main(void)
{

	int i,j;
	readfile("data.txt");
	init();
	reorg();
	while(hfit.fitn != size)
	{
		j=0;
		crossmut();
		gen++;
		for(i = 0; i < size; i++)
		{
			if(phfit[i] == hfit.item[i])j++;
		}
		if(j!=size)
		{
			printf("highest fitness: %d, generation: %d, item: ", hfit.fitn, gen);
			for(i = 0; i < size; i++)
			{
				printf("%c", hfit.item[i]);
			}
			printf("\n");
		}
		for(i = 0; i < size; i++)
		{
			phfit[i] = hfit.item[i];
		}
		reorg();
	}

	return(0);
}