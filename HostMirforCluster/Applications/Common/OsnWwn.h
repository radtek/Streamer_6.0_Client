#ifndef _OSNWWN_H_
#define _OSNWWN_H_


typedef struct _OSNWWN
{
	union
	{
		ULONGLONG		m_WWN;
		LARGE_INTEGER	m_WWNInLargeInteger;
		unsigned char   m_WWNInBytes[8];
	};


#ifdef __cplusplus

	bool operator == (const _OSNWWN& w)
	{
		return m_WWN==w.m_WWN;
	}

	bool operator != (const _OSNWWN& w)
	{
		return m_WWN!=w.m_WWN;
	}
	
	bool operator == (const ULONGLONG& w)
	{
		return m_WWN==w;
	}

	bool operator != (const ULONGLONG& w)
	{
		return m_WWN!=w;
	}

	bool operator <= (_OSNWWN& w)
	{
		return m_WWN<=w.m_WWN;
	}

	bool operator <= (const ULONGLONG& w)
	{
		return m_WWN<=w;
	}

	void operator = (const ULONGLONG& w)
	{
		m_WWN=w;
	}

#endif
} OSNWWN, *POSNWWN;

#ifdef __cplusplus

inline	ULONG Hash(const OSNWWN wwn) 
{
	return (ULONG)((wwn.m_WWNInLargeInteger.HighPart&0x0FFF0000)*
				(wwn.m_WWNInLargeInteger.LowPart&0x0000FFFF));
}

namespace OSNWWNNameSpace
{
	inline	ULONG Hash(const WCHAR *pIqnName, ULONG length ) 
	{
		ULONG h = 5381;
		const WCHAR *x = pIqnName;
		
		while (length--)
			h = ((h << 5) + h) ^ *x++;

		return h;
	}


	inline	OSNWWN	BuildOsnWWN(const WCHAR *pIqnName, ULONG length)
	{

		//discard string starting '.' ':' ',' ';' after first ':'
		bool	afterColon		= false;
		ULONG	i				= 0;
		ULONG	adjustedLength	= length;
//		char	c;

		/*
		for (i=0; i<length; i++)
		{
			c = pIqnName[i];
			if( c == ':' )
			{
				afterColon = true;
				continue;
			}

			if(afterColon)
			{
				if( (c == '.') || (c == ':') || (c == ';') || (c == ',') )
				{
					adjustedLength = i;
					break;
				}
			}
		}
		*/

		ULONG	h = Hash(pIqnName, adjustedLength);

		// the format of the WWN is 40xx00FFFFxxxxxx
		OSNWWN osnWWN;
		
		osnWWN.m_WWN= ((ULONGLONG)0x400000FFFF000000) + (h & 0xffffff) + 
		   ((ULONGLONG)(h & 0xff000000) << 24);

		return osnWWN;

	}

	inline	ULONG Hash(const char *pIqnName, ULONG length ) 
	{
		ULONG h = 5381;
		const char *x = pIqnName;
		
		while (length--)
			h = ((h << 5) + h) ^ *x++;

		return h;
	}


	inline	OSNWWN	BuildOsnWWN(const char *pIqnName, ULONG length)
	{

		//discard string starting '.' ':' ',' ';' after first ':'
		bool	afterColon		= false;
		ULONG	i				= 0;
		ULONG	adjustedLength	= length;
//		char	c;

		/*
		for (i=0; i<length; i++)
		{
			c = pIqnName[i];
			if( c == ':' )
			{
				afterColon = true;
				continue;
			}

			if(afterColon)
			{
				if( (c == '.') || (c == ':') || (c == ';') || (c == ',') )
				{
					adjustedLength = i;
					break;
				}
			}
		}
		*/

		ULONG	h = Hash(pIqnName, adjustedLength);

		// the format of the WWN is 40xx00FFFFxxxxxx
		OSNWWN osnWWN;
		
		osnWWN.m_WWN= ((ULONGLONG)0x400000FFFF000000) + (h & 0xffffff) + 
		   ((ULONGLONG)(h & 0xff000000) << 24);

		return osnWWN;

	}


	inline	OSNWWN	BuildOsnWWNFromMacAddress(const char *pMacAddress)
	{

		// If the input mac is xxxxyyyyyyyyy, WWN will be 40xx00xxyyyyyyyy
		OSNWWN osnWWN;

		if(strlen(pMacAddress) < 12)
		{
			osnWWN.m_WWN = -1;
			return osnWWN;
		}

		char		c;
		ULONGLONG	ull = (ULONGLONG) 0;
		for (int i=0; i<12; i++)
		{
			c		= pMacAddress[i];
			if( (c >= 'A') && (c <= 'F') )
				ull = ull*0x10 + (c - 'A' + 10);
			else if( (c >= 'a') && (c <= 'f') )
				ull = ull*0x10 + (c - 'a' + 10);
			else if( (c >= '0') && (c <= '9') )
				ull = ull*0x10 + (c - '0');
		}


		osnWWN.m_WWN= ((ULONGLONG)0x4000000000000000) 
			+ ((ull & 0xff0000000000) << 8) + (ull & 0xffffffffff);

		return osnWWN;

	}

	inline	OSNWWN	BuildOsnWWNFromMacAddress(const WCHAR *pMacAddress)
	{

		// If the input mac is xxxxyyyyyyyyy, WWN will be 40xx00xxyyyyyyyy
		OSNWWN osnWWN;

		if(wcslen(pMacAddress) < 12)
		{
			osnWWN.m_WWN = -1;
			return osnWWN;
		}

		WCHAR		c;
		ULONGLONG	ull = (ULONGLONG) 0;
		for (int i=0; i<12; i++)
		{
			c		= pMacAddress[i];
			if( (c >= 'A') && (c <= 'F') )
				ull = ull*0x10 + (c - 'A' + 10);
			else if( (c >= 'a') && (c <= 'f') )
				ull = ull*0x10 + (c - 'a' + 10);
			else if( (c >= '0') && (c <= '9') )
				ull = ull*0x10 + (c - '0');
		}


		osnWWN.m_WWN= ((ULONGLONG)0x4000000000000000) 
			+ ((ull & 0xff0000000000) << 8) + (ull & 0xffffffffff);

		return osnWWN;

	}
};

#endif


#endif //_OSNWWN_H_