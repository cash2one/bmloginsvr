
#include "stdafx.h"
#include "SPBFile.h"


int _tmain(int argc, _TCHAR* argv[])
{
	//SPBFile::Unpack(argv[1], argv[2]);
	SPBFile::Pack(argv[1], argv[2]);
	return 0;
}

