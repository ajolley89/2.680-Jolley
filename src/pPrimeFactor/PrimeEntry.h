/************************************************************/
/*    NAME: Austin Jolley                                   */
/*    ORGN: MIT                                             */
/*    FILE: PrimeEntry.h                                   */
/*    DATE:                                                 */
/************************************************************/

#include <string>
#include <vector>
#include <cstdint>
#include <sstream>

#ifndef PRIME_ENTRY_HEADER
#define PRIME_ENTRY_HEADER

class PrimeEntry
{
public:

	PrimeEntry();
	~PrimeEntry() {};

	void setOriginalVal(uint64_t v)	{m_orig = v;}
	void setRecievedIndex(unsigned int v)	 	{m_received_index = v;};
	void setCalculatedIndex(unsigned int v) 	{m_calculated_index = v;};
	void setDone(bool v)						{m_done = v;};

	bool done() {return(m_done);};

	bool factor(unsigned long int max_steps);

	std::string getReport();

protected:
	uint64_t		m_start_index;
	uint64_t		m_orig;
	bool			m_done;
	unsigned int	m_received_index;
	unsigned int	m_calculated_index;

	stringstream m_factors;

}



#endif