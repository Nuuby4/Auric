#include <SDK/SDK.h>

#include <Base/Log.h>
#include <SDK/Funcs.h>

namespace Kyber
{

const char* TypeInfo::getName() const
{ 
	return typeInfoData->name; 
}

TypeCodeEnum TypeInfo::getBasicType() const
{ 
	return TypeCodeEnum((typeInfoData->flags >> 5) & 31); 
}
}