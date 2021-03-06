// HW1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <errno.h>
#include <fstream>
#include <String>
#include <exception>
#include <tuple>
#include <vector>
#include <map>
#include <algorithm>
#include <Psapi.h>

#define MAX_RAM 250000

typedef unsigned __int64 uint64;
typedef unsigned uint;
#pragma pack(push,1)   // change struct packing to 1 byte 
class HeaderGraph {
public:
	uint64 hash;
	uint len;
	
};
#pragma pack(pop) 

typedef unsigned short ushort;
#pragma pack(push,1)   // change struct packing to 1 byte 
class HeaderMap {
public:
	uint64 hash;
	ushort len;
};
#pragma pack(pop) 


// Custom comparator function for std::sort
bool vecSort(std::pair<uint64, int> &firstElem, std::pair<uint64, int> &secondElem)
{
	 return firstElem.second > secondElem.second;
}


//These two incomplete functions were meant to make reading and parsing of data smoother
/*
void newfunc(char *buffer, bytesToRead, FileHandle, map &reference);


void readMyHandle(FILE *myHandle, )
{

uint64 off = 0;
char *buf=(char *)malloc(MAX_RAM*sizeof(char));   // file contents are here
uint64 size = MAX_RAM;
size_t success;

At this point, call new function that does the reading and parsing
newfunc(buf,size, InData)
success = fread(buf, sizeof(char), size, inData, MAP reference);



}

void newfunc(char *buffer, bytesToRead, FileHandle, map &reference)
{




}


*/
int main(int argc, char *argv[])
{

	if(argc!=2)
	{
		std::cout << "Enter only the name of the outgraph\n.";
		return 1;
	}
	
	FILE *inMap, *inData;
	errno_t errMap, errData;
	//errMap = fopen_s(&inMap, 'C:\Users\tvish_000\Downloads\PLD-map-(1).dat', "rb");
	errData = fopen_s(&inData, argv[1], "rb");
	
	//if (errMap)
	//{
	//	std::cerr << "Can't open input map file " <<std::endl;
	//}

	if (errData)
	{
		std::cerr << "Can't open input file " << argv[2] << std::endl;
	}
	uint64 off = 0;
	char *buf=(char *)malloc(MAX_RAM*sizeof(char));   // file contents are here
	uint64 size = MAX_RAM;
	size_t success;
	success = fread(buf, sizeof(char), size, inData);

	std::map<uint64, int> edges;
	
	
	//success = fread(buf, sizeof(char), size, inMap);


	try {
		std::map<uint64, int>::iterator it;
		while (off < size - sizeof(HeaderGraph))
		{
			// the header fits in the buffer, so we can read it   
			HeaderGraph *hg = (HeaderGraph *)(buf + off);
			off += sizeof(HeaderGraph) + hg->len * sizeof(uint64);
			if (off <= size)
			{
				//printf("Node %I64u, degree %d\n", hg->hash, hg->len);
				uint64 *neighbors = (uint64*)(hg + 1);
				for (int i = 0; i < hg->len; i++)
				{
					//printf("  %I64u\n", neighbors[i]);
					it = edges.find(neighbors[i]);
					if (it == edges.end())
					{
						edges[neighbors[i]] = 1;
					}
					else
					{
						it->second++;
					}

				}
			}
			else
				// neighbor list overflows buffer; handle boundary case 
			{
			uint64 mypos;
			long int mynewpos=0;
			int res;
			mypos = ftell(inData);
			//std::cout << "\nmypos = " << mypos<<"\n";
			mynewpos -=(sizeof(HeaderGraph) + hg->len*sizeof(uint64));
			res = fseek(inData, mynewpos,SEEK_CUR);
			success = fread(buf, sizeof(char), size, inData);
			}
		
		}
		
	
		/*while (off < size - sizeof(HeaderMap))
		{
			// the header fits in the buffer   
			HeaderMap *hm = (HeaderMap*)(buf + off);  // node hash & length
			std::cout << "From buf+off = "<<off<<" We are going to add off, sizeof HDRMAP and len whose values are " <<off<<", "<<sizeof(HeaderMap)<<", "<<hm->len<<std::endl ;
			off += sizeof(HeaderMap) + hm->len; 
			//std::cout << hm->len << " " << hm->hash << std::endl;
			
			if (off <= size)
			{
				char *name = (char*)(hm+1);   // pointer to domain 
                std::cout << "\nOff = " << off <<std::endl;
				printf("Node %I64u,Len %u\n", hm->hash,hm->len);
			}
          
			else
			{


			}
				// string overflows into next buffer; handle boundary case  
		}
		*/
		
	}
	catch (std::exception &e)
	{
		std::cerr << e.what();
	}	
	std::vector<std::pair<uint64, int > > myVec(edges.begin(), edges.end());
	//std::cout << myVec.size();
	std::sort(myVec.begin(), myVec.end(), vecSort);
	
	PROCESS_MEMORY_COUNTERS pp;
	HANDLE hProcess = GetCurrentProcess();
	if (GetProcessMemoryInfo(hProcess, &pp, sizeof(PROCESS_MEMORY_COUNTERS)) == FALSE)
		std::cout << "\nBoo hoo\n";
	printf("Peak working set : %zu\n   %0.2f", pp.PeakWorkingSetSize, (pp.PeakWorkingSetSize/1000000.0));
	getchar();

}

