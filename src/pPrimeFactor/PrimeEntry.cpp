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
    m_divisor = 2; //the first number to test as a factor is 2
    m_done = false; //ensure the program doesnt initialize a falsely "completed" object
    m_start_time = MOOSTime(); //Begins timing the calculation from the point the object is created

}

//-----------------------------------------------------------
// Destructor
PrimeEntry::~PrimeEntry()
{

}

bool PrimeEntry::factor(unsigned long int max_steps)
{
    for(unsigned long int i=1; i<max_steps; i++){ //Run the following up until the input number of iterations are complete
        if(m_divisor > sqrt(m_orig)){ //if the divisor is greater than the square root of the original number, we are done
            if(m_factors==""){ //if no primes were calculated before the square root, the number is prime
                m_factors += to_string(m_orig) + ":"; //add the original number to the list of primes. ":" added to be consistent with formatting.
            }
            else //if factors were already calculated, the original number was not prime but the current number being checked is
            {
                m_factors += to_string(m_start_index) + ":"; // add the current number to the list of primes. ":" added to be consistent with formatting.
            }
            setDone(true); //mark the object as complete
            m_end_time = MOOSTime(); //End timing the calculation.
            return true; //exit the loop
        } 

        while(m_start_index % m_divisor == 0){ //executes if m_divisor is an even divisor of the current working number
            m_factors += to_string(m_divisor) + ":"; //keeps adding the divisor to the list of factors as many times as it divides the working number
            m_start_index = m_start_index / m_divisor; //updates the working number by dividing it by its factor
            i++; //count these calculations towards the number of allowed steps
            if(i>max_steps){return false;} //exit if the max steps are exceeded
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
    m_factors.pop_back(); //removes the trailing ":" from the last added factor.
    m_report = "orig=" + to_string(m_orig) + ",received=" + to_string(m_received_index) + ",calculated=" + to_string(m_calculated_index) \
    + ",solve_time=" + to_string(m_end_time - m_start_time) + ",primes=" + m_factors + ",username=ajolley"; //formats the report

    return(m_report); //outputs the formatted report.
}
