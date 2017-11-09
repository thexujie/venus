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

#include "base/array.h"
#include "base/vector.h"
#include "base/pair.h"
#include "base/list.h"
#include "base/dictionary.h"
#include "base/hashtable.h"
#include "base/byte_buffer.h"
#include "base/object_ptr.h"
#include "base/function.h"

#include "base/codepage.h"
#include "base/unicode.h"
#include "base/text.h"
#include "base/suid.h"
#include "base/color.h"
#include "base/datetime.h"

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

