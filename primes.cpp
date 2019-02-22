#include <stdio.h>
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <string>
#include <sstream>
using namespace std;

stringstream prime_factors;
uint64_t factor = 2;
uint64_t original = 0;

void write_factors(uint64_t bignumber) {
	uint64_t working = bignumber;
	if(working == 1) {factor = 2; return;} //if we are at one, we are done. reset the factor variable and quit.

	else if(working%factor == 0) { //if "factor" is a factor of "working"
		prime_factors << factor << ":"; //add "factor" to our list of factors
		factor = 2; //reset "factor" to start again from the lowest prime
		write_factors(working/factor); //find and add factors of our new smaller number
	}
	else {
		if(factor == 2) {factor++;}; //if factor was 2, incriment to 3
		if(factor > 2) {factor+=2;}; //if factor was 3 or higher, incriment by 2 to skip even numbers
		write_factors(working);
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
prime_factors.str("");
original = strtoul(input.c_str(), NULL, 0);
write_factors(original);

cout << "The Prime Factors of " << input << " are: " << prime_factors.str() << endl;

return(0);
}
