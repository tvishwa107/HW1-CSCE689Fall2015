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
#include <unordered_map>


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

int main(int argc, char *argv[])
{

	if(argc!=3)
	{
		std::cout << "Enter mapfile and datafile\n.";
		return 1;
	}
	std::unordered_map<uint64, int> edges;
	FILE *inMap, *inData;
	errno_t errMap, errData;
	//errMap = fopen_s(&inMap, argv[1], "rb");
	errData = fopen_s(&inData, argv[2], "rb");
	//if (errMap)
	//{
	//	std::cerr << "Can't open input file " << argv[1]<<std::endl;
	//}
	if (errData)
	{
		std::cerr << "Can't open input file " << argv[2] << std::endl;
	}
	uint64 off = 0;
	char *buf=(char *)malloc(50000*sizeof(char));   // file contents are here
	uint64 size = 50000;
	size_t success;
	
	success = fread(buf, sizeof(char), size, inData);
	//success = fread(buf, sizeof(char), size, inMap);
	try {
		
		while (off < size - sizeof(HeaderGraph))
		{
			// the header fits in the buffer, so we can read it   
			HeaderGraph *hg = (HeaderGraph *)(buf + off);
			off += sizeof(HeaderGraph) + hg->len * sizeof(uint64);
			if (off <= size)
			{
				printf("Node %I64u, degree %d\n", hg->hash, hg->len);
				uint64 *neighbors = (uint64*)(hg + 1);
				for (int i = 0; i < hg->len; i++)
				{
					printf("  %I64u\n", neighbors[i]);
					std::unordered_map<uint64, int>::iterator check = edges.find(neighbors[i]);
					if (check == edges.end())
					{
						edges[neighbors[i]] = 1;
					}
					else
					{
						check->second++;
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
			std::cout << "\nmypos = " << mypos<<"\n";
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
	std::unordered_map<uint64,int>::const_iterator iter ;
	for (iter = edges.begin(); iter != edges.end(); iter++)
		std::cout << iter->first << " " << iter->second << std::endl;
	getchar();

}

