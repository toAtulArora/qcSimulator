#include <iostream> //Well..
#include <conio.h> //For getch()
#include <vector> //For arrays
#include <complex> //For complex numbers
#include <math.h> //For pow etc.
#include <random> //For supporting random numbers
#include <bitset> //For printing some binary

using namespace std;
template<class decimal=float>
class QC
{
	int qBits;
	decimal normalization;
	vector <complex <decimal> > amplitudes;
	string statusPrefix()
	{
		return "QC [" + to_string(msgCount++) + "]: ";
	}
public:
	//Does something like this
	//num=0x110100
	//tBit=0
	//value=1
	//returned=0x1101001
	int insertBit(int num, int tBit, int value)
	{
		//int numLHS=num/(int)pow(2,tBit);
		int numLHS=num>>tBit;
		//numLHS=numLHS*2*pow(2,tBit);
		numLHS=numLHS<<(tBit+1);
		int numRHS = num &((1<<(tBit)) - 1);
		//int numRHS=num%(int)pow(2,tBit);
		//int numRHS=num<<
		
		cout<<"Num:"<<(bitset<8>) num<<endl;
		cout<<"LHS:"<<(bitset<8>) numLHS<<endl;
		cout<<"RHS:"<<(bitset<8>) numRHS<<endl;
		//return numRHS+numLHS + (value==1?1:0)*pow(2,tBit);
		return numRHS+numLHS + (value==1?1:0)*(1<<tBit);
	}
	int getBit(int num,int tBit)
	{
		return num/(int)pow(2,tBit) - 2*(num/(int)pow(2,tBit+1));
	}
	int setBit(int num, int tBit, int value)
	{
		return 0;
	}
public:
	string status;
	int msgCount;
	string log;
	QC(int init_qBits)
		:log(""),status(""),msgCount(0)
	{
		qBits=init_qBits;
		amplitudes.resize((int)pow(2,qBits));
		amplitudes[0]=(1,0); //Set all qbits to zero
		status=statusPrefix()+"Initialized to simulate " + to_string(qBits) + " qBits. " + to_string((int)pow(2,qBits)) + " complex numbers reserved.";
		log+=status+"\n";
	}
	int init_qBit(int qBit,int value)
	{


	}
	//QC() : QC(10) {};
};


void main()
{
	QC<float> qc(8);
	cout<<qc.status<<endl;
	bitset<8> n(string("00001110"));
	//cout<<n.to_ullong()<<endl;
	//int n=0xF;
	int nDash=qc.insertBit(n.to_ulong(),0,1);
	cout<<"So we had\n"<<n<<endl<<(bitset<8>) nDash<<endl<<"Did that work?";
	//cout<<"This works";
	_getch();

}