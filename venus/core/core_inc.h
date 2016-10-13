#pragma once

#include "config.h"

#if defined _WINDLL && !defined VENUS_NO_EXPORT
#define CORE_API __declspec (dllexport)
#define VENUS_API __declspec (dllexport)
#else
#define CORE_API
#define VENUS_API
#endif

#ifdef VENUS_USE_NAMESPACE

#define VENUS_BEG namespace venus{
#define VENUS_END }
namespace venus
{
}

#else

#define VENUS_BEG
#define VENUS_END

#endif // VENUS_USE_NAMESPACE

#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <memory.h>
#include <typeinfo.h>
#include <new>

#include "core_def.h"
#include "core_api.h"
#include "core_exp.h"

#include "stl/stl.h"

#include "array.h"
#include "vector.h"
#include "pair.h"
#include "list.h"
#include "dictionary.h"
#include "hashtable.h"
#include "byte_buffer.h"
#include "object_ptr.h"

#include "function.h"

#include "codepage.h"
#include "unicode.h"
#include "text.h"
#include "color.h"
#include "datetime.h"

#include "platform_api.h"

#include "support.h"

#include "file.h"


#include "xml.h"
#include "tools/IData.h"
#include "tools/IMetaHelper.h"

#include "math.h"

#include "io/IStream.h"
#include "io/CMemoryStream.h"
#include "io/CFileStream.h"
#include "io/CByteStream.h"
#include "io/CBufferedStream.h"
#include "io/CDataStream.h"
#include "io/CTextStream.h"

