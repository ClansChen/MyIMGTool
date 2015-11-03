#pragma once
#include <QtGlobal>

/*http://www.gtamodding.com/wiki/Cryptography*/

class CKeyGen
{
private:
	static const quint32 keyTable[256];
	
public:
	static quint32 GetKeyOfUpCasedString(const char *);
};
