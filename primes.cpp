#include <stdio.h>
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <string>
#include <sstream>
using namespace std;

int bignumber;
stringstream prime_factors;

void factors(uint64_t bignumber) {
	uint64_t working = bignumber;
	cout << "working: " << working <<endl;
	cout << "prime_factors: " << prime_factors.str() <<endl;

	if(working%2==0) {
		prime_factors << "2";
		working /=2;
	}

	while(working%2 == 0) {
		prime_factors << ":2";
		working /= 2;
		cout << "prime_factors: " << prime_factors.str()<<endl;
		cout << "working:" << working <<endl;
	
	}

	for(int n=3; n<sqrt(bignumber); n+=2) {
		while(working%n == 0) {
		prime_factors << ":" << to_string(n);
		working /= n;
		cout << "prime_factors: " << prime_factors.str()<<endl;
		cout << "working:" << working << endl;

		}
	}

}


int main(int argc, char* argv[])
{
if(argc!=2){
		cout << "Usage: primes <integer>" << endl;
		cout << "Please use integer values." <<endl;
		return(1);
	}

string input = argv[1];
uint64_t origin = strtoul(input.c_str(), NULL, 0);
factors(origin);

cout << "The Prime Factors of " << origin << " are: " << prime_factors.str() << endl;

return(0);
}
