#include "first.h"

int cacheSize;
char* asso;
int policy;
int blockSize;
int n;
int hit;	//# of hits
int miss;	//# of misses
int read;	//# of reads
int write;	//# of writes
int bps;	//blocks per set
int sets;	//# of sets
int bBits;	//# of block offsetbits
int iBits;	//# of index bits
int tBits;	//# of tag bits
FILE* fp;

int main (int argc, char **argv) {

	if (argc != 6) {
		printf("error - wrong number of inputs\n");
		return 0;
	}
//													<cache size>	<associativity>	<cache policy>	<block size>	<trace file>
	cacheSize = atoi(argv[1]);
	asso = argv[2];
	policy = policyEquals(argv[3]);
	blockSize = atoi(argv[4]);
	n = assocEquals(asso);
	fp = fopen(argv[5], "r");

	asso = argv[2];
	if (	powerOf2(cacheSize) == 0 ||					//prints "error" if all conditions for input are not met
			assocEquals(asso) == 0 ||
			policy == 0 ||
			powerOf2(blockSize) == 0 ||
			fp == NULL) {

		printf("error - conditions not met - check input\n");
		return 0;
	}
	
	if (n == -1) {bps=1;}								//direct
	else if (n == -2) {bps=cacheSize/blockSize;}		//fully associative
	else {bps=twoToThe(n);}										//n-way
	sets = cacheSize/(blockSize*bps);
	
	bBits = powerOf2(blockSize);
	if (n != -2)
		iBits = powerOf2(sets);
	else
		iBits = 0;
	tBits = 48 - (bBits+iBits);
	
	if (policy==1) {fifo();}
	else if (policy==2) {lru();}
	else if (policy==3) {nru();}
	
	
	printf("Memory Reads: %d\nMemory Writes: %d\nCache Hits: %d\nCache Misses: %d\n",read, write,hit,miss);
	return 0;
}

int twoToThe(int val) {
	int c=1;
	for(;val>0;val--) {c=c*2;}
	return c;
}

void fifo() {
	Set* cache =  (Set*)malloc(sets * sizeof(Set));
	
	int i = 0;
	for (i = 0; i<sets; i++) {
		cache[i].array = (Block*)malloc(bps * sizeof(Block));
		
		int j = 0;
		for (j = 0; j<bps; j++) {
			cache[i].array[j].tStamp=0;
			cache[i].array[j].valid=0;
			cache[i].array[j].tag=0;
		}
	}
	
	int time = 0;
	
	while (!feof(fp)) {
		int hitBool = 0;
		char pc[30];
		char input;
		unsigned long int address;
		unsigned long int tag;
		unsigned long int index;
		
		fscanf(fp,"%s %c %lx", pc, &input, &address);			 //reads file and sets address
		if (pc[0] == '#') {break;}

		if (n == -1) { 							//direct
			index = (address>>bBits)&(twoToThe(iBits)-1);
			tag = (address>>(bBits+iBits));
	
			if (cache[index].array[0].valid == 1) {
				if (cache[index].array[0].tag == tag) {
					hitBool = 1;
				}
			}
			if (hitBool == 0) {
				cache[index].array[0].valid = 1;
				cache[index].array[0].tag = tag;
				cache[index].array[0].tStamp = time;
				hitBool = -1;
			}
			
			if (input == 'R' && hitBool == 1) {hit++;}
			else if (input == 'W' && hitBool == 1) {write++; hit++;}
			else if (input == 'R' && hitBool == -1) {read++; miss++;}
			else if (input == 'W' && hitBool == -1) {read++; write++; miss++;}
			input = 'P';
			hitBool = 0;
			time++;
		} else if (n != -2) {									//n-way
			index = (address>>bBits)&(twoToThe(iBits)-1);
			tag = (address>>(bBits+iBits));
			int k = 0;
			
			while(cache[index].array[k].valid != 0 && cache[index].array[k].tag != tag && k<bps){
				k++;
			}
			if (cache[index].array[k].valid == 0) {
				cache[index].array[k].valid = 1;
				cache[index].array[k].tStamp = time;
				cache[index].array[k].tag = tag;
				hitBool= -1;
			} else if (cache[index].array[k].tag == tag) {
				hitBool=1;
			} else {
				int min = 0;
				k = 0;
				while (k<bps) {
					if (cache[index].array[k].tStamp < cache[0].array[min].tStamp) {min = k;}
					k++;
				}
				cache[index].array[min].valid = 1;
				cache[index].array[min].tag = tag;
				cache[index].array[min].tStamp = time;
				hitBool = -1;
			}
	/*
			int k = 0;
			for(; k< bps; k++) {
				if (cache[index].array[k].valid == 1) {
					if (cache[index].array[k].tag == tag) {
						hitBool = 1;
						break;
					}
				}
			}
			if (hitBool == 0) {
				int min = 0;
				int k = 0;
				for(; k< (sizeof(cache[index]) / sizeof(cache[index].array[0])); k++) {
					if (cache[index].array[k].tStamp < cache[index].array[min].tStamp) {min = k;}
				}
				cache[index].array[min].valid = 1;
				cache[index].array[min].tag = tag;
				cache[index].array[min].tStamp = time;
				hitBool = -1;
			}
	*/		
			if (input == 'R' && hitBool == 1) {hit++;}
			else if (input == 'W' && hitBool == 1) {write++; hit++;}
			else if (input == 'R' && hitBool == -1) {read++; miss++;}
			else if (input == 'W' && hitBool == -1) {read++; write++; miss++;}
			input = 'P';
			hitBool = 0;
			time++;
		} else { //no index bits -- assoc
			
			tag = (address>>bBits);
			
			int k = 0;
			
			while(cache[0].array[k].valid != 0 && cache[0].array[k].tag != tag && k<bps){
				k++;
			}
			if (cache[0].array[k].valid == 0) {
				cache[0].array[k].valid = 1;
				cache[0].array[k].tStamp = time;
				cache[0].array[k].tag = tag;
				hitBool= -1;
			} else if (cache[0].array[k].tag == tag) {
				hitBool=1;
			} else {
				int min = 0;
				k = 0;
				while (k<bps) {
					if (cache[0].array[k].tStamp < cache[0].array[min].tStamp) {min = k;}
					k++;
				}
				cache[0].array[min].valid = 1;
				cache[0].array[min].tag = tag;
				cache[0].array[min].tStamp = time;
				hitBool = -1;
			}
			
			if (input == 'R' && hitBool == 1) {hit++;}
			else if (input == 'W' && hitBool == 1) {write++; hit++;}
			else if (input == 'R' && hitBool == -1) {read++; miss++;}
			else if (input == 'W' && hitBool == -1) {read++; write++; miss++;}
			input = 'P';
			hitBool = 0;
			time++;
		}
	}
	
}

void lru() {
	Set* cache =  (Set*)malloc(sets * sizeof(Set));
	
	int i = 0;
	for (i = 0; i<sets; i++) {
		cache[i].array = (Block*)malloc(bps * sizeof(Block));
		
		int j = 0;
		for (j = 0; j<bps; j++) {
			cache[i].array[j].tStamp=0;
			cache[i].array[j].valid=0;
			cache[i].array[j].tag=0;
		}
	}
	int time = 0;
	while (!feof(fp)) {
		int hitBool = 0;
		char pc[30]; //program counter
		char input;
		unsigned long int address;
		unsigned long int tag;
		unsigned long int index;
		
		fscanf(fp,"%s %c %lx", pc, &input, &address);			 //reads file and sets address
		if (pc[0] == '#') {break;}

		if (n == -1) { 							//direct
			index = (address>>bBits)&(twoToThe(iBits)-1);
			tag = (address>>(bBits+iBits));
	
			if (cache[index].array[0].valid == 1) {
				if (cache[index].array[0].tag == tag) {
					hitBool = 1;
				}
			}
			if (hitBool == 0) {
				cache[index].array[0].valid = 1;
				cache[index].array[0].tag = tag;
				cache[index].array[0].tStamp = time;
				hitBool = -1;
			}
			
			if (input == 'R' && hitBool == 1) {hit++;}
			else if (input == 'W' && hitBool == 1) {write++; hit++;}
			else if (input == 'R' && hitBool == -1) {read++; miss++;}
			else if (input == 'W' && hitBool == -1) {read++; write++; miss++;}
			input = 'P';
			hitBool = 0;
			time++;
		} else if (n != -2) {									//n-way
			index = (address>>bBits)&(twoToThe(iBits)-1);
			tag = (address>>(bBits+iBits));
	
			int k = 0;
			
			while(cache[index].array[k].valid != 0 && cache[index].array[k].tag != tag && k<bps){
				k++;
			}
			if (cache[index].array[k].valid == 0 && k<bps) {
				cache[index].array[k].valid = 1;
				cache[index].array[k].tStamp = time;
				cache[index].array[k].tag = tag;
				hitBool= -1;
			} else if (cache[index].array[k].tag == tag) {
				cache[index].array[k].tStamp = time;
				hitBool=1;
			} else {
				int min = 0;
				k = 0;
				while (k<bps) {
					if (cache[index].array[k].tStamp < cache[index].array[min].tStamp) {min = k;}
					k++;
				}
				cache[index].array[min].valid = 1;
				cache[index].array[min].tag = tag;
				cache[index].array[min].tStamp = time;
				hitBool = -1;
			}
			if (input == 'R' && hitBool == 1) {hit++;}
			else if (input == 'W' && hitBool == 1) {write++; hit++;}
			else if (input == 'R' && hitBool == -1) {read++; miss++;}
			else if (input == 'W' && hitBool == -1) {read++; write++; miss++;}
			input = 'P';
			hitBool = 0;
			time++;
		} else {						//assoc
			tag = (address>>bBits);
			
			int k = 0;
			
			while(cache[0].array[k].valid != 0 && cache[0].array[k].tag != tag && k<bps){
				k++;
			}
			if (cache[0].array[k].valid == 0) {
				cache[0].array[k].valid = 1;
				cache[0].array[k].tStamp = time;
				cache[0].array[k].tag = tag;
				hitBool= -1;
			} else if (cache[0].array[k].tag == tag) {
				cache[0].array[k].tStamp = time;
				hitBool=1;
			} else {
				int min = 0;
				k = 0;
				while (k<bps) {
					if (cache[0].array[k].tStamp < cache[0].array[min].tStamp) {min = k;}
					k++;
				}
				cache[0].array[min].valid = 1;
				cache[0].array[min].tag = tag;
				cache[0].array[min].tStamp = time;
				hitBool = -1;
			}
			
			if (input == 'R' && hitBool == 1) {hit++;}
			else if (input == 'W' && hitBool == 1) {write++; hit++;}
			else if (input == 'R' && hitBool == -1) {read++; miss++;}
			else if (input == 'W' && hitBool == -1) {read++; write++; miss++;}
			input = 'P';
			hitBool = 0;
			time++;
			
		}
	}
}

void nru() {
	Set* cache =  (Set*)malloc(sets * sizeof(Set));
	int i = 0;
	for (i = 0; i<sets; i++) {
		cache[i].array = (Block*)malloc(bps * sizeof(Block));
		
		int j = 0;
		for (j = 0; j<bps; j++) {
			cache[i].array[j].tStamp=0;
			cache[i].array[j].valid=0;
			cache[i].array[j].tag=0;
		}
	}
	
	int time = 0;
	
	while (!feof(fp)) {
		int hitBool = 0;
		char pc[30]; //program counter
		char input;
		unsigned long int address;
		unsigned long int tag;
		unsigned long int index;
		
		fscanf(fp,"%s %c %lx", pc, &input, &address);			 //reads file and sets address
		if(pc[0] == '#') {break;}

		if (n == -1) { 							//direct
			index = (address>>bBits)&(twoToThe(iBits)-1);
			tag = (address>>(bBits+iBits));
	
			if (cache[index].array[0].valid == 1) {
				if (cache[index].array[0].tag == tag) {
					hitBool = 1;
				}
			}
			if (hitBool == 0) {
				cache[index].array[0].valid = 1;
				cache[index].array[0].tag = tag;
				cache[index].array[0].tStamp = time;
				hitBool = -1;
			}
			
			if (input == 'R' && hitBool == 1) {hit++;}
			else if (input == 'W' && hitBool == 1) {write++; hit++;}
			else if (input == 'R' && hitBool == -1) {read++; miss++;}
			else if (input == 'W' && hitBool == -1) {read++; write++; miss++;}
			input = 'P';
			hitBool = 0;
			time++;
		} else if (n != -2) {									//n-way
			index = (address>>bBits)&(twoToThe(iBits)-1);
			tag = (address>>(bBits+iBits));
			hitBool=0;
			
				
			int k = 0;
			int trigger = -1;
			
			for (k=0; k<bps; k++) {
				if (cache[index].array[k].tag == tag && cache[index].array[k].valid == 1) {
					cache[index].array[k].tStamp = 0;
					hitBool = 1;
					break;
				} else if (cache[index].array[k].valid == 0) {
					cache[index].array[k].valid = 1;
					cache[index].array[k].tag = tag;
					cache[index].array[k].tStamp = 0;
					hitBool = -1;
					break;
				}
				if (cache[index].array[k].valid == 1 && cache[index].array[k].tStamp == 1 && trigger == -1) {
					trigger = k;
				}
			}
			
			if (hitBool == 0) {
				k = 0;
				if (trigger == -1) {
					for (k=0; k<bps; k++) {
						cache[index].array[k].tStamp = 1;
					}
					k = 0;
					trigger = k;
				}
				cache[index].array[trigger].valid = 1;
				cache[index].array[trigger].tStamp = 0; 
				cache[index].array[trigger].tag = tag;
				hitBool = -1;
			}
			
			if (input == 'R' && hitBool == 1) {hit++;}
			else if (input == 'W' && hitBool == 1) {write++; hit++;}
			else if (input == 'R' && hitBool == -1) {read++; miss++;}
			else if (input == 'W' && hitBool == -1) {read++; write++; miss++;}
			input = 'P';
			hitBool = 0;
		} else { //no index bits -- assoc
			index = 0;
			tag = (address>>bBits);
			hitBool = 0;
			
			int k = 0;
			int trigger = -1;
			
			for (k=0; k<bps; k++) {
				if (cache[0].array[k].tag == tag && cache[0].array[k].valid == 1) {
					cache[0].array[k].tStamp = 0;
					hitBool = 1;
					break;
				} else if (cache[0].array[k].valid == 0) {
					cache[0].array[k].valid = 1;
					cache[0].array[k].tag = tag;
					cache[0].array[k].tStamp = 0;
					hitBool = -1;
					break;
				}
				if (cache[0].array[k].valid == 1 && cache[0].array[k].tStamp == 1 && trigger == -1) {
					trigger = k;
				}
			}
			
			if (hitBool == 0) {
				k = 0;
				if (trigger == -1) {
					for (k=0; k<bps; k++) {
						cache[0].array[k].tStamp = 1;
					}
					k = 0;
					trigger = k;
				}
				cache[0].array[trigger].valid = 1;
				cache[0].array[trigger].tStamp = 0; 
				cache[0].array[trigger].tag = tag;
				hitBool = -1;
			}
			
			if (input == 'R' && hitBool == 1) {hit++;}
			else if (input == 'W' && hitBool == 1) {write++; hit++;}
			else if (input == 'R' && hitBool == -1) {read++; miss++;}
			else if (input == 'W' && hitBool == -1) {read++; write++; miss++;}
			input = 'P';
			hitBool = 0;
		}
		
		
	}
}

int powerOf2(int val) {								//returns 1 if power of 2, 0 otherwise	
	if (val == 2)
		return 1;
	
	int i = 2;
	int c = 1;

	while (i < val) {i = i*2;c++;}
	if (val == i)
		return c;
	return 0;
}

int assocEquals (char* str) {						//returns -1 if direct, -2 if fully associative, n if n-way fully associative, 0 if other
	if (strcmp(str, "direct")==0) {
		return -1;
	} else if (strcmp(str, "assoc")==0) {
		return -2;
	} else if (str[0] =='a' && str[1] =='s' && str[2] == 's'  && str[3] =='o' && str[4] =='c' && str[5] ==':') {
		const char colon[2] = ":";
		char* nAsso = strtok(str, colon);
		nAsso = strtok(NULL, colon);

		if (powerOf2(atoi(nAsso)) > 0){return powerOf2(atoi(nAsso));}
		else{return 0;}

	} else {
		return 0;
	}
}

int policyEquals (char* str) {						//returns 1 if fifo, 2 if LRU, 3 if NRU, 0 if other
	if (strcmp(str, "fifo")==0) {return 1;}
	else if (strcmp(str, "lru")==0) {return 2;}
	else if (strcmp(str, "nru")==0) {return 3;}
	return 0;
}


