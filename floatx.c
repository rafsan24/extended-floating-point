#include "floatx.h"
#include <assert.h>
#include <limits.h> // for CHAR_BIT - number of bits per byte
#include <math.h> // for isinf and isnan
#include <stdio.h>
#include "bitFields.h" 


floatx doubleToFloatx(double val,int totBits,int expBits) 
{
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
