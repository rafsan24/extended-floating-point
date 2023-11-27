#include "floatx.h"
#include <assert.h>
#include <limits.h> // for CHAR_BIT - number of bits per byte
#include <math.h> // for isinf and isnan
#include <stdio.h>
#include "bitFields.h" // from HW04


floatx doubleToFloatx(double val,int totBits,int expBits) 
{

/*-----------------------------------------------------------------------------------------------
	From the README: 	At a high level, doing the conversion requires several manipulations:
	1. Extracting the sign bit from the double value, and inserting it into the floatx value at
		the correct position.
	2. Handle special cases, such as 0.0, infinity, or not-a-number.
	3. Extract the biased exponent from the double value.
			Check to see if the double value is sub-normal. If so, handle it.
			Check to make sure the floatx exponent won't overflow or underflow).
					If so, handle as a special case.
			If not, rebias the exponent using the floatx bias (which depends on the number of exponent bits)
			and write the result to the correct location in the floatx result.
	4. Extract the fraction bits from the double value.
			Determine how many bits are available for the fraction in the floatx value,
			and truncate or extend the original value,
			and write the resulting bits to the floatx result.
	6. Return the floatx result.
----------------------------------------------------------------------------------------------------*/

	// First, make some assertions to ensure the totBits and expBits parameters are OK
	// Then, implement the algorithm
   	
   	assert(1 <= expBits && expBits <= (totBits - 2));
	assert(3 <= totBits && totBits <= 64);

	union hexDouble 
	{
		double dbl;
		long lng;
	} unionVal;
	unionVal.dbl = val;
	
	int r_bnum; 
	unsigned long fractn = getBitFldU(51, 52, unionVal.lng);
	unsigned long signd = getBit(63, unionVal.lng);
	unsigned long expont = getBitFldU(62, 11, unionVal.lng);
	floatx result = 0;

	if (expont == 2047) 
	{  
		expont = (1 << expBits) - 1;
		if (fractn != 0) 
		{ 
			fractn = 1;
		}  
	} 
	else if (expont == 0) 
	{  
		if (fractn != 0) 
		{    
			fractn = 0;
		}  
  	} 
  	else 
  	{ 
    		int reb_cons = (1 << (expBits - 1)) - 1; 
    		long signdEx = expont - 1023;  
    		
    		signdEx += reb_cons;  
    		expont = signdEx;

    		if (signdEx < 1) 
    		{  
      			expont = 0;
      			fractn = 0;
    		} 
    		else if (expont > ((1 << expBits) - 2))
    		{  
      			expont = (1 << expBits) - 1; 
      			fractn = 0;
    		} 
    		else 
    		{ 
      			if (totBits - expBits < 53) 
      			{  
        			r_bnum = 53 - (totBits - expBits);
        			fractn >>= r_bnum;
      			} 
      			else if (totBits - expBits > 53) 
      			{  
        			fractn <<= (totBits - expBits - 53);
      			}
    		}
  	}

	result = result | fractn;
	result = result | (signd << (totBits - 1));
	result = result | (expont << (totBits - expBits - 1));
  	return result; 
}
