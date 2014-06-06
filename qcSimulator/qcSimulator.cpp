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
	//string printNumFancy(int n)
	//{
		//return to_string((bitset<8>) n) + " [" + to_string(n) + "]";
	//}

	string printNumFancy(int n)
	{
		stringstream temp;
		temp<<n<<" ["<<(bitset<8>) n << "]";
		return temp.str();
	}

	int insertBit(int num, int tBit, int value)
	{
		//USEFUL TEST CODE (USE IT OUTSIDE AND MAKE THIS PUBLIC FIRST)
		//bitset<8> n(string("00001110"));
		//cout<<n.to_ullong()<<endl;
		//int n=0xF;
		//int nDash=qc.insertBit(n.to_ulong(),0,1);
		//cout<<"So we had\n"<<n<<endl<<(bitset<8>) nDash<<endl<<"Did that work?";
		//cout<<"This works";


		//int numLHS=num/(int)pow(2,tBit);
		int numLHS=num>>tBit; //Calculate quotient
		//numLHS=numLHS*2*pow(2,tBit);
		numLHS=numLHS<<(tBit+1); //Bit shift to proper place
		int numRHS = num &((1<<(tBit)) - 1); //Calculate remainder
		//int numRHS=num%(int)pow(2,tBit);
		//int numRHS=num<<
		
		//YOU CAN ENABLE THIS FOR DEBUGGING
		//cout<<"Num:"<<(bitset<8>) num<<endl;
		//cout<<"LHS:"<<(bitset<8>) numLHS<<endl;
		//cout<<"RHS:"<<(bitset<8>) numRHS<<endl;
		
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
	stringstream statusStream;
	string status;
	int msgCount;
	string log;
	QC(int init_qBits)
		:log(""),status(""),msgCount(0),normalization(1)
	{
		qBits=init_qBits;
		amplitudes.resize((int)pow(2,qBits));
		amplitudes[0]=complex<decimal>(1,0); //Set all qbits to zero
		status=statusPrefix()+"Initialized to simulate " + to_string(qBits) + " qBits. " + to_string((int)pow(2,qBits)) + " complex numbers reserved.";
		log+=status+"\n";
	}
	void init_qBit(int qBit,int value)
	{
		statusStream.str("");
		statusStream<<statusPrefix()<<"Initializing qBit: " << qBit << " to " << value << "\n";
		for(int i=0;i< (1<<(qBits-1)) - 1; i++)
		{
			int survivorBasis=insertBit(i,qBit,value);
			int killBasis=insertBit(i,qBit,!value);
			complex<decimal> amplitude=amplitudes[survivorBasis];
			complex<decimal> modifyAmplitude=amplitudes[killBasis];
			if(!(amplitude.real()==0 && amplitude.imag()==0))
			{
				normalization -= norm(modifyAmplitude);
				modifyAmplitude=0;
				statusStream<< "Normalization: "<<normalization<<endl
						<<	"Modified qBit: "<< printNumFancy(killBasis)<<endl
						<<	"Survivor qBit: "<< printNumFancy(survivorBasis)<<endl<<"--"<<endl;
			}
			else
			{
				if(!(modifyAmplitude.real()==0 && modifyAmplitude.imag()==0))
				{
				amplitude = modifyAmplitude;
				modifyAmplitude = 0;
				//No need to change normalization, we just swapped
				statusStream<<	"Modified basis: "<< printNumFancy(killBasis)<<endl
						<<	"Survivor basis: "<< printNumFancy(survivorBasis)<<endl<<"--"<<endl;

				}
			}
			
		}
		status=statusStream.str();
		log+=status+"\n";
	}
	
	//QC() : QC(10) {};
};


void main()
{
	QC<float> qc(8);
	cout<<qc.status<<endl;
	qc.init_qBit(2,1);
	cout<<qc.status<<endl;
	_getch();

}