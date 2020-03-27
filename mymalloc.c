#include "mymalloc.h"
#define Heapsize 25000
#define numberofmemslot  100

typedef struct memslot{
	unsigned int size;
	char * start;
}memslot;


static char Heap[Heapsize];
static int freeslot=-1;
static int useslot=-1;
static int usage=(sizeof(memslot)*numberofmemslot)*2;
static memslot * freearr=(memslot*)(Heap+sizeof(Heap)-(sizeof(memslot)*numberofmemslot));
static memslot * usearr=(memslot*)(Heap+Heapsize-(sizeof(memslot)*numberofmemslot)*2);


static void swap(memslot *a,memslot *b){
	memslot temp=*a;
	*a=*b;
	*b=temp;
}



static int Partion(memslot * arr,int start,int end){
	int pivot=arr[end].size;
	int pindex=start;
	int i;
	for(i=start;i<end;i++){
		if(pivot<=arr[i].size){
			swap(&arr[i],&arr[pindex]);
			pindex++;
		}
	}
	swap(&arr[end],&arr[pindex]);
	return pindex;
}



static void sort(memslot * arr,int start,int end){
	if(start<end){
		int pindex=Partion(arr,start,end);
		sort(arr,start,pindex-1);
		sort(arr,pindex+1,end);
	
	}
}

static int merge(memslot * p){
// merge  p node with pre node 
	memslot * temp1=freearr;
	int i=0;
	while(!(i>freeslot)){
		char * ptr=temp1->start+temp1->size;
		if(p->start==ptr)break;
		temp1++;
		i++;
	}
	if(!(i>freeslot)){
		temp1->size+=p->size;
		p->size=0;
		
	}else{
		temp1=p;
	}
//merge p node with next node
//if 
	
	int j=0;
	memslot * temp2=freearr;
	while(!(j>freeslot)){
		if(temp2->start==temp1->start+temp1->size){
			break;
		}
		temp2++;
		j++;
	}
	if(!(j>freeslot)){
		temp1->size+=temp2->size;
		temp2->size=0;
	}
	sort(freearr,0,freeslot);
	if(!(i>freeslot)){
		if(!(j>freeslot)){
			freeslot-=2;
		}else{
			freeslot--;
		}
	}

}

void * MyMalloc(unsigned int size){
	if(size==0){
		printf("OOPsss!\n");
		return NULL;
	}
	if(Heapsize-usage<size && useslot<numberofmemslot-1){
		printf("Memory overflow\n");
		return NULL;
	}
	if(freeslot==-1 && useslot==-1){
		usearr[++useslot].size=size;
		usearr[useslot].start=Heap;
		usage+=size;
		freearr[++freeslot].size=Heapsize-usage;
		freearr[freeslot].start=Heap+size;
		return usearr[useslot].start;
	}
	//find the freeslot starting from last eliment of the array because array has sort by descendong order
	memslot * temp=freearr+freeslot;
	
	//searching free slot
	while(temp->size<size && temp!=freearr){
		temp--;
	}
	useslot++;
	usearr[useslot].size=size;
	usearr[useslot].start=temp->start;
	temp->size-=size;
	temp->start+=size;
	int val=temp->size;
	sort(freearr,0,freeslot);
	if(val==0)freeslot--;
	usage+=usearr[useslot].size;
	return usearr[useslot].start;

}


void MyFree(void *p){
	memslot * temp=usearr;
	while(temp->start!=p){
		temp++;
	}
	memslot * temp2=freearr;
	if(freeslot==numberofmemslot-1){
		temp2 += freeslot;
		temp2=temp;
	}else{
		freeslot++;
		temp2 += freeslot;
		temp2->size = temp->size;
		temp2->start= temp->start;
	}

	
	temp->size=0;
	sort(usearr,0,useslot);
	useslot--;
	usage-=temp2->size;
	merge(temp2);
	
}





