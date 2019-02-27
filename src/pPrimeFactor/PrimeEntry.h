/************************************************************/
/*    NAME: Austin Jolley                                   */
/*    ORGN: MIT                                             */
/*    FILE: PrimeEntry.h                                   */
/*    DATE:                                                 */
/************************************************************/
#ifndef PRIME_ENTRY_HEADER
#define PRIME_ENTRY_HEADER

#include <string>
#include <vector>
#include <cstdint>
#include "MBUtils.h"
#include "MOOS/libMOOS/MOOSLib.h"

class PrimeEntry
{
public:

	PrimeEntry();
	~PrimeEntry();

	void setOriginalVal(uint64_t v)				{m_orig = v; m_start_index = v;};
	void setRecievedIndex(unsigned int v)	 	{m_received_index = v;};
	void setCalculatedIndex(unsigned int v) 	{m_calculated_index = v;};
	void setDone(bool v)						{m_done = v;};
	bool done() 								{return(m_done);};
	bool factor(unsigned long int max_steps);
	std::string getReport();
	

protected:
	uint64_t		m_start_index;
	uint64_t		m_orig;
	bool			m_done;
	unsigned int	m_received_index;
	unsigned int	m_calculated_index;
	uint64_t		m_divisor;
	double			m_start_time;
	double			m_end_time;
	std::string		m_report;
	std::string 	m_factors;
};



#endif