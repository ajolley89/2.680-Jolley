/************************************************************/
/*    NAME: Austin Jolley                                   */
/*    ORGN: MIT                                             */
/*    FILE: PrimeEntry.cpp                                  */
/*    DATE:                                                 */
/************************************************************/

#include "PrimeEntry.h"
using namespace std;

//-----------------------------------------------------------
// Constructor
PrimeEntry::PrimeEntry()
{
    m_factors= ""; //initialize with an empty list of primes
    m_divisor = 2; //the first number to test is 2
    m_start_index = m_orig; //initialize with the working number equal to the original number to be factored
    m_done = false;
    m_start_time = MOOSTime(); //Begins timing the calculation from the point the object is created

}

//-----------------------------------------------------------
// Destructor
PrimeEntry::~PrimeEntry()
{

}

bool PrimeEntry::factor(unsigned long int max_steps)
{
    for(unsigned long int i=1; i<max_steps; i++){
        if(m_divisor > sqrt(m_orig) && m_factors==""){ //if the divisor is greater than the square root of the original number, the number was prime.
            m_factors += to_string(m_orig) + "sqrt:"; //ends with a ":" to be consistent with other formatting
            setDone(true);
            m_end_time = MOOSTime(); //End timing the calculation.
            return false;
        }
        while(m_start_index%m_divisor==0){ //executes if m_divisor is an even divisor of the current working number
            m_factors += to_string(m_divisor) + "while:"; //keeps adding the divisor to the list of factors as many times as it divides the working number
            m_start_index = m_start_index/m_divisor; //updates the working number by dividing it by its factor
            i++; //count these calculations towards the number of allowed steps
        }
        if(m_divisor >  2){m_divisor += 2;} //if the working number is no longer even, we can incriment by 2 to only test odd numbers
        if(m_divisor == 2){m_divisor=3;} //if the working number was 2, it should be incrimented by 1 so we dont skip over 3
        if(m_start_index == 1){ //if the working number reaches one, then it has been fully factored. Set the done flag and exit
            setDone(true);
            m_end_time = MOOSTime(); //End timing the calculation.
            return true;
        }
        
    }
    return false;

}

string PrimeEntry::getReport()
{
    m_factors.pop_back(); //removes the trailing ":"
    m_report = "orig=" + to_string(m_orig) + ",received=" + to_string(m_received_index) + ",calculated=" + to_string(m_calculated_index) \
    + ",solve_time=" + to_string(m_end_time - m_start_time) + ",primes=" + m_factors + ",username=ajolley"; //formats the report

    return(m_report); //outputs the formatted report.
}
