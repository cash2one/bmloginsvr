#ifndef _INC_MACROHELPER_
#define _INC_MACROHELPER_
//////////////////////////////////////////////////////////////////////////
#define READWRITE_PROPERTY(VAR, NAME, TYPE)		protected:TYPE VAR;READWRITE_INTERFACE(VAR, NAME, TYPE);
#define READWRITE_INTERFACE(VAR, NAME, TYPE)	READ_INTERFACE(VAR, NAME, TYPE)WRITE_INTERFACE(VAR, NAME, TYPE)
#define READ_INTERFACE(VAR, NAME, TYPE)			public:TYPE Get##NAME(){return VAR;}
#define WRITE_INTERFACE(VAR, NAME, TYPE)		public:void Set##NAME(TYPE _var){VAR = _var;}
//////////////////////////////////////////////////////////////////////////
#endif