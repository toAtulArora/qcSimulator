#include <iostream> //Well..
//#include <conio.h> //For getch()
#include <vector> //For arrays
#include <complex> //For complex numbers
#include <math.h> //For pow etc.
#include <random> //For supporting random numbers
#include <bitset> //For printing some binary
#include <Eigen/Dense>	//For matrix multiplication an all
#include <algorithm> //For sort
using namespace Eigen;

using namespace std;
//////A useful function for printing arrays
template <class Collection>
string to_string(Collection const & collection,char const * delimiter)
{
    typedef typename Collection::const_iterator Iter;
    
    Iter beg = collection.begin();
    Iter const end = collection.end();
    
    if (beg == end)
    {
        return string();
    }
    
    std::ostringstream os;
    
    os << *beg;
    ++beg;
    
    for ( ; beg != end; ++beg)
    {
        os << delimiter << *beg;
    }
    
    return os.str();
}

template<class Collection>
string to_string(Collection const & collection, string const & delimiter)
{
    return to_string(collection, delimiter.c_str());
}
////////////

template<class decimal=float>
class QC
{
public:
	typedef complex<decimal> scalar;
//TODO: make it private
public: 
	int qBits; //Number of qubits
	decimal normalization; //Normalization constant
	vector <scalar> amplitudes; //stores the complex amplitudes
	decimal root2; //just for saving root2 as its used often
	//The following simply returns a nicely formatted message prefix to the messages
	//QC [0]: Here's the first message
	//like that
	string statusPrefix()
	{
		return "QC [" + to_string(msgCount++) + "]: ";
	}
//public:
	//Does something like this
	//num=0x110100
	//tBit=0
	//value=1
	//returned=0x1101001
	//string printNumFancy(int n)
	//{
		//return to_string((bitset<8>) n) + " [" + to_string(n) + "]";
	//}

	//This is for ease of viewing a number
	//it prints the number 2 as
	//2 [00000010]
	//Both decimal and binary
	string printNumFancy(int n)
	{
		stringstream temp;
		temp<<n<<" ["<<(bitset<8>) n << "]";
		return temp.str();
	}

	//This is used in some subroutine
	//This does something like this
	//For num=3, tBit=1, value=1, we have
	// num (in binary) = 0 0 0 0 0 0 1 1
	// target bit                     ^
	// result (binary) = 0 0 0 0 0 0 1 1 1
	// the first (leftmost) zero is truncated
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
    //Not used! I am using inline now
	bool insertBitsSort(const vector<int>& a, const vector<int>& b)
	{
		return a[0]<b[0]; //for ascending |
		//return a[0]>b[0]; //for descending
	}

	int insertBits(int num, vector < vector <int> > data)
	{
		//So data is expected to be something like this
		// tBit, value
		// data={ {2,1} , {1,0}, {3,1} }
		// So this whole thing is to sort the whole thing (sounds silly now..) so that insertion can be done to the right (smallest tBit) first
		//sort(data.begin(),data.end(),insertBitsSort);
		//TODO: Understand the lambda symbol
		sort(data.begin(),data.end(),[](const vector<int>& a, const vector<int>& b)
		{
		    return a[0]<b[0];
		});
		int resultantNum=num;
		for(vector<int> &a : data)
		{
			resultantNum=insertBit(resultantNum,a[0],a[1]);
		}
		return resultantNum;
	}
	//Not used, but it basically gives the value of the said bit in a number in base 2
	int getBit(int num,int tBit)
	{
		//return num/(int)pow(2,tBit) - 2*(num/(int)pow(2,tBit+1));
		return (((num & (1<<tBit)) == 0) ? 0 : 1);
	}
	//not used, not implemented
	int setBit(int num, int tBit, int value)
	{
		return 0;
	}
	//For a complex number, returns true if its non-zero
	bool nonZero(complex <decimal> num)
	{
		if(num.real()!=0 || num.imag()!=0)
			return true;
		else
			return false;
	}

    
	
	//Prints the number as a ket
	string printKet(int n)
	{
		stringstream temp;
		temp<<"|"<<(bitset<8>) n << " (" <<n<<") >";
		return temp.str();
	}
public:	
	typedef Matrix< scalar, 2, 2> mat2x2;
	typedef Matrix< scalar, 4, 4> mat4x4;
	typedef Matrix< scalar, 2, 1> mat2x1;
	
	//typedef Matrix< complex<decimal>, 2, 2> tMat2x2;

	mat2x2 hadamard;	
	mat4x4 cNot;
	//tMat2x2 hadamard2;
    
	stringstream statusStream;
	string status;
	int msgCount;
	string log;
	//Constructor :D
	QC(int init_qBits)
		:log(""),status(""),msgCount(0),normalization(1),root2(sqrt(2))
	{
		hadamard << 1/root2 , 1/root2,
			1/root2, -1/root2;
		cNot << 1, 0, 0, 0,
		        0, 1, 0, 0,
		        0, 0, 0, 1,
		        0, 0, 1, 0;
		qBits=init_qBits;
		amplitudes.resize((int)pow(2,qBits));
		amplitudes[0]=complex<decimal>(1,0); //Set all qbits to zero
		status=statusPrefix()+"Initialized to simulate " + to_string(qBits) + " qBits. " + to_string((int)pow(2,qBits)) + " complex numbers reserved.";
		log+=status+"\n";
	}
	//Initialize the given qubit to the given value
	void init_qBit(int qBit,int value)
	{
		statusStream.str("");
		statusStream<<statusPrefix()<<"Initializing qBit: " << qBit << " to " << value << "\n";
		for(int i=0;i< (1<<(qBits-1)) - 1; i++)
		{
			int survivorBasis=insertBit(i,qBit,value);
			int killBasis=insertBit(i,qBit,!value);
			//assigned by reference
			complex<decimal>& amplitude=amplitudes[survivorBasis];
			complex<decimal>& modifyAmplitude=amplitudes[killBasis];
			if(!(amplitude.real()==0 && amplitude.imag()==0))
			{
				normalization -= norm(modifyAmplitude);
				modifyAmplitude=0;
				statusStream<< "Normalization: "<<normalization<<endl
						<<	"Modified basis: "<< printNumFancy(killBasis)<<endl
						<<	"Survivor basis: "<< printNumFancy(survivorBasis)<<endl<<"--"<<endl;
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

	//UPdates the status to read the current qubits
	void status_qBits()
	{
		statusStream.str("");
		statusStream<<statusPrefix()<<"Current state of qubits:\n  ";
		for(int i=0;i<(1<<qBits)-1;i++)
		{
			if(nonZero(amplitudes[i]))
				statusStream<<"("<<amplitudes[i].real()/sqrt(normalization)<<" + i"<<amplitudes[i].imag()/sqrt(normalization)<<") "<<printKet(i)<<"\n";
			if(i+1 < (1<<qBits) -1)	//if this wasn't the last element
				if(nonZero(amplitudes[i+1])) //and if the last element is non-zero
					statusStream<<"+ ";
		}
		statusStream<<"--"<<endl;
		status=statusStream.str();
		log+=status+"\n";
	}
	//Make sure the gate matrix is normalized!
	void gate1_qBit(const mat2x2& gate, int qBit1)
	{
		//mat2x2 gate;
		//gate<< 1/root2, 1/root2, 1/root2, -1/root2;
		//gate+=gate2;
		statusStream.str("");
		statusStream<<statusPrefix()<<"Applying the following Gate on a single qBit \n"<<gate<<endl;
		statusStream<<"Allocating memory for evaluation"<<endl;
		vector <scalar> evaluatedAmplitudes;
		evaluatedAmplitudes.assign((1<<qBits),0);	//Intialize to 0

		mat2x1 basis0(1,0);
		mat2x1 transformedBasis0=gate*basis0;
		statusStream<<"Transformed Basis 0:\n"<<transformedBasis0<<endl;
		mat2x1 basis1(0,1);
		mat2x1 transformedBasis1=gate*basis1;
		statusStream<<"Transformed Basis 1:\n"<<transformedBasis1<<endl;
		for(int i=0; i<(1<<(qBits-1)) - 1; i++)
		{
			int j=insertBit(i,qBit1,0);
			int k=insertBit(i,qBit1,1);
			//This is essentially (for hadamard), applying
			// |0> ----> (|0> + |1>)/sqrt(2)
			// ^amplitudes[j]
			//			   ^ evaluatedAmplitudes[j]
			//					 ^ evaluatedAmplitudes[k]
			evaluatedAmplitudes[j]+=amplitudes[j] * transformedBasis0(0);
			evaluatedAmplitudes[k]+=amplitudes[j] * transformedBasis0(1);
			//Note += is used since its a linear transformation that's being applied (just give it a thought and it should be clear)

			//And now the same thing for
			// |1> ----> (|0> - |1>)/sqrt(2)
			evaluatedAmplitudes[j]+=amplitudes[k] * transformedBasis1(0);
			evaluatedAmplitudes[k]+=amplitudes[k] * transformedBasis1(1);
		}
		//Now that we're done, we simply swap the vectors
		amplitudes.swap(evaluatedAmplitudes);
		//and upon destruction, evaluatedAmplitudes will be freed from the memory :D
		status=statusStream.str();
		log+=status+"\n";
	}
	vector<decimal> measure_qBits(vector <int> qBitX)
	{
	    statusStream.str("");
	    statusStream<<statusPrefix()<<"Measuring qBits {"<<to_string(qBitX,",")<<"} \n";
	    #define remaining_qBits qBits-qBitX.size()
	    vector<scalar> evaluatedAmplitudes;
	    vector<decimal> collapseProbabilities;    //This stores the collapse probabilities for a given value of the measured qubits
	    evaluatedAmplitudes.assign((1<<qBits),0); 
	    collapseProbabilities.assign((1<<qBitX.size()),0);
	    
	    statusStream<<"Entering the loop\n Note: The results are NOT displayed normalized (normalization constant="<<normalization<<")\n";
	    statusStream<<"Probability \t State";
        for(int i=0;(1<<qBitX.size());i++)
        {
    	    for(int j=0;j<(1<<remaining_qBits);j++)
    	    {
	            vector< vector <int> > insertBitsData;
	            int k=0;
	            for(int &x: qBitX)
	            {
	                insertBitsData.push_back(vector <int> {x,getBit(i,k++)});
	            }
	            int ii=insertBits(j,insertBitsData);
	            collapseProbabilities[i]+=norm(amplitudes[ii])/(normalization);
	        }
            
            statusStream<<collapseProbabilities[i]<<"\t";
            
    	    for(int j=0;j<(1<<remaining_qBits);j++)
    	    {
	            vector< vector <int> > insertBitsData;
	            int k=0;
	            for(int &x: qBitX)
	            {
	                insertBitsData.push_back(vector <int> {x,getBit(i,k++)});
	            }
	            int ii=insertBits(j,insertBitsData);
    
    			if(nonZero(amplitudes[ii]))
    				statusStream<<"("<<amplitudes[ii].real()/sqrt(normalization)<<" + i"<<amplitudes[ii].imag()/sqrt(normalization)<<") "<<printKet(ii);
    			if(j+1 < (1<<remaining_qBits) -1)	//if this wasn't the last element
    				if(nonZero(amplitudes[i+1])) //and if the last element is non-zero
    					statusStream<<" + ";

	           // statusStream<<printKet(ii);
	        }

	    }
	    

	    //TODO: implement collapsing using random variables
	    int iCollapsed=0;
	    
	   // for(int i=0;(1<<qBitX.size());i++)
	   // {
        int i=iCollapsed;
        decimal reNormalize=0;
        for(int j=0;j<(1<<remaining_qBits);j++)
        {
            vector< vector <int> > insertBitsData;
            int k=0;
            for(int &x: qBitX)
            {
                insertBitsData.push_back(vector <int> {x,getBit(i,k++)});
            }
            int ii=insertBits(j,insertBitsData);
            evaluatedAmplitudes[ii]=amplitudes[ii];
            reNormalize+=norm(evaluatedAmplitudes[ii]);
        }
	   // }
	    normalization=reNormalize;
	    amplitudes.swap(evaluatedAmplitudes);
	    
	    statusStream<<"Measured\n----"<<endl;
	    status=statusStream.str();
	    log+=status;
	    return collapseProbabilities;
	}
	//TODO: Understand how derived is working
	template <typename Derived>
	void gateN_qBit(const MatrixBase<Derived>& gate, vector <int> qBitX)
	{
	    statusStream.str("");
	    statusStream<<statusPrefix()<<"Applying the following on {"<<to_string(qBitX,",")<<"} qubits\n"<<gate<<endl;
		//int num_qBits=qBitX.size();             //Number of qbits involved in the operation of the gate, eg 2
        vector<scalar> evaluatedAmplitudes;
        evaluatedAmplitudes.assign((1<<qBits), 0);
        Matrix< scalar, Dynamic, Dynamic> basis (1<<qBitX.size(),1 );  //A row vector to hold the amplitudes corresponding to all the participating basis, |00>, |01>, |10>, |11>
        Matrix< scalar, Dynamic, Dynamic> transformedBasis (1<<qBitX.size(),1); //Same thing for the transformed basis
        #define remaining_qBits qBits-qBitX.size()
        //TODO: assert that the matrix's dimensiona nd size(qBitX) are equal
        statusStream<<"Entering the loop\n Note: The results aren't normalized (normalization constant="<<normalization<<")\n";
        for(int j=0;j<(1<<remaining_qBits);j++)
        {
            // statusStream<<"j="<<printNumFancy(j)<<endl;
            //Find out why the following isn't working (although it isn't even needed)
            //basis()=Matrix< scalar, Dynamic, Dynamic>::Zero(1<<qBitX.size(), 1);
            bool notBlank=false;
            for(int i=0;i<(1<<qBitX.size());i++)
            {
                // statusStream<<"i="<<i<<endl;
                vector< vector <int> > insertBitsData;
                int k=0;
                for(int &x: qBitX)
                {
                    insertBitsData.push_back(vector <int> {x,getBit(i,k++)});
                    // statusStream<<"x="<<x<<" val="<<getBit(i,k-1)<<endl;
                }
                    
                int ii=insertBits(j,insertBitsData);
                // statusStream<<"ii="<<printNumFancy(ii)<<endl;
                basis(i)=amplitudes[ii];
                if(nonZero(basis(i)))
                    notBlank=true;
            }
            if(notBlank)
            {
                statusStream<<"The basis for the given j (= "<<printNumFancy(j)<<") is \n"<<basis<<endl;
                transformedBasis=gate*basis;
                statusStream<<"The transformed basis is \n"<<transformedBasis<<endl;
                //The following seems duplicated, but there is little point in converting this to a function
                //The last assignment is different, that's all.
                for(int i=0;i<(1<<qBitX.size());i++)
                {
                    vector< vector <int> > insertBitsData;
                    int k=0;
                    for(int &x: qBitX)
                        insertBitsData.push_back(vector <int> {x,getBit(i,k++)});
                    int ii=insertBits(j,insertBitsData);
                    evaluatedAmplitudes[ii]+=transformedBasis(i);
                }
            }
        }
        statusStream<<"Evaluation completed"<<endl<<"----"<<endl;
        amplitudes.swap(evaluatedAmplitudes);
        status=statusStream.str();
        log+=status;
        // This was for Testing
// 		statusStream.str("");
// 		statusStream<<statusPrefix()<<"Testing now\n";
// 		int num=0;
// 		vector <vector <int> > insertBitsData;
// 		vector<int> a;

// 		for(int &x : qBitX)
// 			insertBitsData.push_back(vector <int> {x,1});
		
// 		statusStream<<printNumFancy(num)<<"\n"<<printNumFancy( insertBits(num,insertBitsData))<<endl;
// 		status=statusStream.str();
// 		log+=status;
	}
	//QC() : QC(10) {};

	//
	void cheatInitializeState(vector<scalar>& newAmplitudes)
	{
		statusStream.str("");
		statusStream<<statusPrefix()<<"Initializing the state (cheating)"<<endl;
		amplitudes.swap(newAmplitudes);
		normalization=0;
		for(int i=0;i<amplitudes.size();i++)
		{
			normalization+=norm(amplitudes[i]);
		}
		statusStream<<"Done";
		status=statusStream.str();
		log+=status+"\n";
	}
};


int main()
{
	typedef QC<float> QCf;
	QCf qc(8);
	cout<<qc.status<<endl;

	vector<QCf::scalar> newAmplitudes;
	newAmplitudes.assign(1<<8,0);
	newAmplitudes[0]=qc.root2;
	newAmplitudes[1]=qc.root2;
	qc.cheatInitializeState(newAmplitudes);


	//qc.init_qBit(2,1);
	cout<<qc.status<<endl;	
	qc.status_qBits();
	cout<<qc.status<<endl;
// 	qc.gate1_qBit(qc.hadamard,2);
// 	cout<<qc.status<<endl;

// 	qc.gateN_qBit(qc.hadamard, {2} );
    qc.gateN_qBit(qc.cNot,{1,0});
	cout<<qc.status<<endl;

	qc.status_qBits();
	cout<<qc.status<<endl;
// 	vector <int> a;
	//a={0,2,3,4};

	//Matrix<double,2,2> mat1,mat2;
	//mat2<<1,1,2,2;
	//mat1=mat2;
	//cout<<mat1<<endl<<mat2;
	//_getch();
	cin.get();
	//QCf::mat2x2 m;
    return 0;
}