#pragma once
#include <string>

namespace CommonUtilities
{
	int Horspool(const std::string& aNeedle, const std::string& aHaystack)
	{
        auto hlen = aHaystack.size();
        const auto nlen = aNeedle.size();
        size_t scan;
        size_t bad_char_skip[UCHAR_MAX + 1];
		
        if (nlen <= 0 || aHaystack.empty() || aNeedle.empty())
            return -1;
		
        for (scan = 0; scan <= UCHAR_MAX; scan = scan + 1) 
            bad_char_skip[scan] = nlen;

		size_t last = nlen - 1;
        for (scan = 0; scan < last; scan = scan + 1) 
            bad_char_skip[aNeedle[scan]] = last - scan;

		size_t hOff = 0;
        while (hlen >= nlen)
        {
            for (scan = last; aHaystack[scan + hOff] == aNeedle[scan]; scan = scan - 1)
            {
                if (scan == 0)
                {
                    return hOff;
                }
            }
            hlen -= bad_char_skip[aHaystack[last + hOff]];
            hOff += bad_char_skip[aHaystack[last + hOff]];
        }
        return -1;
	}
}
