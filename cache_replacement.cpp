#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>

using namespace std;

// address
vector<string> address;
	// int[2] = { Tag, Index }

// cashe
struct Block
{
	bool valid;		// true if is occupied
	int tag;		// show who occupy
	int rec;		// show the putin time or nearest time access this address
};
vector<Block*> Cache;

// control
int mode;			// =0, FIFO; =1, LRU

//==============================================================

void readfile() 
{
	ifstream file;
	string line;
	int datatype = 0;
	
	file.open("address.txt", ios::in);
	if (file.is_open())
	{
		while(getline(file, line))
		{
			address.push_back(line);
		}
		file.close();
	}
	else
	{
		cout << "Unable to open the file!" << endl;
	}
}

void generateCache(int n)
{
	Block *ptrb;
	for(int i = 0; i < n; i++)
	{
		ptrb = new Block;
		ptrb->valid = false;
		Cache.push_back(ptrb);
	}
}

//================================================================

string getbinary(char a)
{
	if(a == '0'){return "0000";	}
	else if(a == '1'){return "0001";	}
	else if(a == '2'){return "0010";	}
	else if(a == '3'){return "0011";	}
	else if(a == '4'){return "0100";	}
	else if(a == '5'){return "0101";	}
	else if(a == '6'){return "0110";	}
	else if(a == '7'){return "0111";	}
	else if(a == '8'){return "1000";	}
	else if(a == '9'){return "1001";	}
	else if(a == 'A'){return "1010";	}
	else if(a == 'B'){return "1011";	}
	else if(a == 'C'){return "1100";	}
	else if(a == 'D'){return "1101";	}
	else if(a == 'E'){return "1110";	}
	else if(a == 'F'){return "1111";	}
	else{return "N";	}
}

string HextoBinary(string a)
{
	string ttemp;
	string temp;
	for(int i = 0; i < a.size(); i++)
	{
		ttemp = getbinary(a[i]);
		if(ttemp == "N")
		{
			return "N";
		}
		else
		{
			temp = temp + ttemp;
		}
	}
	return temp;
}

int getTagorIndex(int num, int skip, string a)
{
	int temp = 0;
	int limit = num + skip;
	for(int i = skip; i < limit; i++)
	{
		if(a[i] == '0')	// = 0
		{
			continue;
		}
		else			// = 1
		{
			temp = temp + pow(2, limit - 1 - i);
		}
	}
	return temp;
}

//================================================================

bool hit(Block *ptrb, int tag, int cycle)
{
	if(ptrb->valid == true)
	{
		if(ptrb->tag == tag)
		{
			if(mode == 1)
			{
				ptrb->rec = cycle;
			}
			return true;
		}
	}
	return false;
}

bool Putin(Block *ptrb, int tag, int cycle)
{
	if(ptrb->valid == false)
	{
		ptrb->valid = true;
		ptrb->tag = tag;
		ptrb->rec = cycle;
		return true;
	}
	return false;
}

int findrec(Block *ptrb)
{
	return ptrb->rec;
}

void replace(Block *ptrb, int tag, int nowcycle)
{
	ptrb->valid = true;
	ptrb->tag = tag;
	ptrb->rec = nowcycle;
}

int main(int argc, char **argv) 
{
	// cashe 
	int Cache_size = 16;	// (Kbyte)
	int Block_size = 16;	// (byte) 
	int associative = 4;
	
	// address
	int addr_bit = 20;
	int Block_num, Set_num;
	int Tag_bit, Index_bit, Offset_bit;
	int Tag, Index;
	
	// temp
	bool skip;
	int temp, minrec, nowblock, replaceblock;
	string bi_addr;
	
	// count
	int miss_count = 0, rep_count = 0;
	
	readfile();
	
	// Cashe
	Block_num = (Cache_size * 1024) / Block_size;
	Set_num = Block_num / associative;
	generateCache(Block_num);
	
	// get index, tag
	Offset_bit = log2(Block_size);
	Index_bit = log2(Set_num);
	Tag_bit = addr_bit - Offset_bit - Index_bit;
	
	cout << "# of Offset bits: " << Offset_bit << endl;
	cout << "# of Index bits: " << Index_bit << endl;
	cout << "# of Tag bits: " << Tag_bit <<endl << endl;
	
	// control
	while(1)		// determine using which mode
	{
		cout << "Input 0(for FIFO) or 1(LRU) to determine using which mode: ";
		cin >> mode;
		if(mode == 0)
		{
			cout << endl << "Using FIFO for cache replacement!" << endl << endl;
			break;
		}
		else if(mode == 1)
		{
			cout << endl << "Using LRU for cache replacement!" << endl << endl;
			break;
		}
		else
		{
			cout << "Unvalid input! Please enter again!" << endl;
		}
	}
	
	cout << "Address	" << "Hit?	" << "Putin	" << "Replace?	" << endl;
	
	for(int i = 0; i < address.size(); i++)
	{
		skip = false;
		cout << address[i] << "	";
		
		// translate
		bi_addr = HextoBinary(address[i]);
		
		// get tag and index
		if(bi_addr == "N")			// including unvalid input
		{
			cout << "Include nonvalid address!" << endl;
			break;
		}
		else
		{
			Tag = getTagorIndex(Tag_bit, 0, bi_addr);
			Index = getTagorIndex(Index_bit, Tag_bit, bi_addr);
		}
		
		// check if already in
		for(int j = 0; j < associative; j++)
		{
			nowblock = Index * associative + j;
			if(hit(Cache[nowblock], Tag, i))
			{
				cout << "Hit!	" << endl;
				skip = true;
				break;
			}
		}
		if(skip){ continue;	}
		
		cout << "Miss!	";
		
		// search empty block
		for(int k = 0; k < associative; k++)
		{
			nowblock = Index * associative + k;
			if(Putin(Cache[nowblock], Tag, i))		// exist empty block
			{
				cout <<  nowblock << endl;
				skip = true;
				miss_count++;
				break;
			}
		}
		if(skip){ continue;	}
		
		// find replace block
		minrec = address.size();
		for(int m = 0; m < associative; m++)
		{
			nowblock = Index * associative + m;
			temp = findrec(Cache[nowblock]);
			if(temp < minrec)
			{
				minrec = temp;
				replaceblock = nowblock;
			}
		}
		replace(Cache[replaceblock], Tag, i);
		
		cout << replaceblock << "	Yes!" << endl;
		miss_count++;
		rep_count++;
	}
	
	cout << endl << "Total miss time: " << miss_count << endl;
	cout << "Total replace time: " << rep_count << endl;
	
	
	system("pause");
}

