#pragma once

VENUS_BEG

template<typename CharT>
CharT chlower(CharT ch)
{
	if('A' <= ch && ch <= 'Z')
		return 'a' + (ch - 'A');
	else
		return ch;
}
template<typename CharT>
CharT chupper(CharT ch)
{
	if('a' <= ch && ch <= 'a')
		return 'A' + (ch - 'a');
	else
		return ch;
}

template<typename CharT>
void textempty(CharT * text, int_x size = -1)
{
	if(text && size)
		text[0] = 0;
}

template<typename CharT>
bool textisempty(CharT * text, int_x size = -1)
{
	return !size || !text || !text[0];
}

/**
* @brief 计算文本的长度。
* @param pText 要计算长度的文本。
* @param iSize 容纳文本的缓冲区大小，即文本最大长度为 iSize - 1。默认为-1。
*              如果为负数，则计算过程不考虑文本缓冲区大小。。
* @return 返回计算的文本的长度。
*/
template<typename CharT>
int_x textlen(const CharT * text, int_x size = -1)
{
	if(!text || !size)
		return 0;

	int_x len = 0;
	while(*text++ && --size)
		++len;
	return len;
}

/**
* @brief 将源字符窜的指定长度复制到目标字符串，并添加终止符(除非目标字符串大小为0)。
* @param szText 目标字符串。
* @param iSize 目标字符串的大小，包括终止符，它就是目标字符串的容量。
* @param szSrc 源字符串。
* @param iLength 要拷贝的字符的个数，不包括终止符，如果为负数，则拷贝直至终止符为止。
*                就算 iLength 指定的长度大于 pSrc 的真实长度，那也只会拷贝直至终止符为止。
* @return 返回拷贝的字符的个数，不包括终止符。
*/
template<typename CharT>
int_x textcpy(CharT * dst, int_x size, const CharT * src, int_x length = -1)
{
	if(!size)
		return 0;
	else if(size == 1)
	{
		*dst = 0;
		return 0;
	}
	else
	{
		if(length < 0)
			length = minof(textlen(src), size - 1);
		else
		{
			int_x lengh_max = textlen(src, length + 1);
			if(length > lengh_max)
				length = minof(lengh_max, size - 1);
			else
				length = minof(length, size - 1);
		}


		if(length)
			buffcpy(dst, src, minof(size, length));
		dst[length] = 0;
		return length;
	}
}

inline int_x textcpyw(char_16 * dst, int_x size, const char_16 * src, int_x length = -1)
{
	return textcpy<char_16>(dst, size, src, length);
}

inline int_x textcpya(char_8 * dst, int_x size, const char_8 * src, int_x length = -1)
{
	return textcpy<char_8>(dst, size, src, length);
}

inline int_x textcpyx(char_x * dst, int_x size, const char_x * src, int_x length = -1)
{
	return textcpy<char_x>(dst, size, src, length);
}

/**
* @brief 将源字符串的指定长度里的大写字母转换为小写。
* @param szText 目标字符串。
* @param iLength 要转换的长度，不包括终止符。
* @return 被转换的字符的个数，不包括终止符。
*/
template<typename CharT>
int_x textlower(CharT * text, int_x length = -1)
{
	if(length < 0)
		length = textlen(text);

	int_x iCount = 0;
	for(int_x cnt = 0; cnt < length; ++cnt)
	{
		CharT & ch = text[cnt];
		if('A' <= ch && ch <= 'Z')
		{
			ch = 'a' + (ch - 'A');
			++iCount;
		}
	}
	return iCount;
}

/**
* @brief 将源字符串的指定长度里的大写字母转换为大写。
* @param szText 目标字符串。
* @param iLength 要转换的长度，不包括终止符。
* @return 被转换的字符的个数，不包括终止符。
*/
template<typename CharT>
int_x textupper(CharT * text, int_x length = -1)
{
	if(length < 0)
		length = textlen(text);

	int_x iCount = 0;
	for(int_x cnt = 0; cnt < length; ++cnt)
	{
		CharT & ch = text[cnt];
		if('a' <= ch && ch <= 'z')
		{
			ch = 'A' + (ch - 'a');
			++iCount;
		}
	}
	return iCount;
}

// -------------------------------------------------------------------------------------------------
// -------------------- 对文本进行增、删、改的操作 -----------------------------------------------------
// -------------------------------------------------------------------------------------------------

template<typename CharT>
int_x textch(const CharT * text, int_x length, const CharT & ch)
{
	if(!text || !length)
		return -1;
	const CharT * szTemp = text;
	while(*szTemp && length--)
	{
		if(*szTemp == ch)
			return szTemp - text;
		else
			++szTemp;
	}
	return -1;
}

template<typename CharT>
const CharT * textpch(const CharT * text, int_x length, const CharT & ch)
{
	int_x index = textch(text, length, ch);
	if(index < 0)
		return nullptr;
	else
		return text + index;
}

template<typename CharT>
int_x textrch(const CharT * text, int_x length, const CharT & ch)
{
	if(!text || !length)
		return -1;

	if(length < 0)
		length = textlen(text);

	const CharT * pLast = text + (length - 1);
	const CharT * pEnd = text - 1;

	while(length-- && pLast > pEnd)
	{
		if(*pLast == ch)
			return pLast - text;
		else
			--pLast;
	}
	return -1;
}

template<typename CharT>
const CharT * textprch(const CharT * text, int_x length, const CharT & ch)
{
	int_x index = textrch(text, length, ch);
	if(index < 0)
		return nullptr;
	else
		return text + index;
}

template<typename CharT>
int_x textcmp(const CharT * text, int_x length, const CharT * pattern, int_x pattern_length, bool caps = false)
{
	if(!text && !pattern)
		return 0;
	if(!length && !pattern_length)
		return 0;

	CharT ch1, ch2;
	while(length-- && pattern_length--)
	{
		ch1 = *text++;
		ch2 = *pattern++;
		if(!caps)
		{
			ch1 = chlower(ch1);
			ch2 = chlower(ch2);
		}
		if(ch1 != ch2)
			return ch1 - ch2;
		if(ch1 == 0)
			return 0;
	}
	return 0;
}

template<typename CharT>
int_x textstr(const CharT * text, int_x length, const CharT * pattern, int_x pattern_length, bool caps = true)
{
	if(length < 0)
		length = textlen(text);
	if(!text || !length)
		return -1;

	if(pattern_length < 0)
		pattern_length = textlen(pattern);
	if(!pattern || !pattern_length)
		return -1;

	if(length < pattern_length)
		return -1;

	const CharT * beg = text;
	const CharT * end = text + (length - pattern_length + 1);
	while(beg < end)
	{
		if(textequalex(beg, pattern_length, pattern, pattern_length, caps))
			return beg - text;
		else
			++beg;
	}
	return -1;
}

template<typename CharT>
int_x textrstr(const CharT * text, int_x length, const CharT * pattern, int_x pattern_length, bool caps = true)
{
	if(length < 0)
		length = textlen(text);
	if(!text || !length)
		return -1;

	if(pattern_length < 0)
		pattern_length = textlen(pattern);
	if(!pattern || !pattern_length)
		return -1;

	if(length < pattern_length)
		return -1;

	const CharT * beg = text + (length - pattern_length);
	const CharT * end = text;
	while(beg >= end)
	{
		if(textequalex(text, pattern_length, pattern, pattern_length, caps))
			return beg - text;
		else
			--beg;
	}
	return -1;
}

template<typename CharT>
int_x textrplch(CharT * text, int_x length, const CharT & src, const CharT & dst)
{
	if(!text || !text[0] || !length)
		return 0;

	int_x iCount = 0;
	while(*text && length--)
	{
		if(*text == src)
		{
			*text = dst;
			++iCount;
		}
		else {}
		++text;
	}
	return iCount;
}

template<typename CharT>
int_x textrplstr(CharT * text, int_x length, int_x size,
				 int_x index_beg, int_x index_end, const CharT * dst, int_x dst_length)
{
	if(length < 0) 
		length = textlen(text);
	if(dst_length < 0) 
		dst_length = textlen(dst);
	if(!length)
		return -1;

	if(length >= size)
		return -1;

	int_x count = index_end - index_beg;
	if(index_beg < 0 || count <= 0 || index_end > length)
		return -1;

	int_x length_new = length - count + dst_length;
	if(length_new >= size)
		return length_new - size + 1;

	buffmov(text + (index_beg + dst_length), text + index_end, size - index_end + 1);
	buffcpy(text + index_beg, dst, dst_length);
	return 0;
}

/**
* @brief 将 szText 中的 szSrc 替换为 szDst。
* @param szText 待替换的文本。
* @param iLength 待替换为本的长度。
* @param iSize 待替换为本的缓冲区最大长度，包括终止符。
* @param szSrc 要查找的文本。
* @param iSrcLength 要查找文本的长度。
* @param szDst 替换成的文本。
* @param iDstLength 替换成文本的长度。
* @param iMaxCount 最多替换的数量。
* @return 返回成功替换掉的文本的数量。
*/
template<typename CharT>
int_x textrplstr(CharT * text, int_x length, int_x size,
				 const CharT * src, int_x src_length, const CharT * dst, int_x dst_length,
				 bool caps = true, int_x count_max = IX_MAX)
{
	if(length < 0) 
		length = textlen(text);
	if(src_length < 0) 
		src_length = textlen(src);
	if(dst_length < 0) 
		dst_length = textlen(dst);
	if(!length || !src_length || count_max <= 0)
		return 0;

	int_x diff = dst_length - src_length;
	int_x count = 0;
	int_x length_new = length;
	CharT * beg = text;
	const CharT * end = text + length;
	while(beg < end)
	{
		int_x index = textstr(beg, length, src, src_length, caps);
		if(index < 0)
			break;

		if(length + diff >= size)
			break;
		length -= src_length + index;

		CharT * szCurr = beg + index;
		buffmov(szCurr + dst_length, szCurr + src_length, length);
		buffcpy(szCurr, dst, dst_length);
		length_new += diff;
		beg += index + dst_length;
		++count;
	}
	text[length_new] = (CharT)'\0';
	return count;
}

template<typename CharT>
int_x textcntstr(const CharT * text, int_x length, const CharT * pattern, int_x patter_length, bool caps = true)
{
	if(length < 0)
		length = textlen(text);
	if(patter_length < 0)
		patter_length = textlen(pattern);
	if(!length || !patter_length)
		return 0;

	int_x count = 0;
	const CharT * beg = text;
	const CharT * end = text + (length - patter_length + 1);
	while(beg < end)
	{
		int_x index = textstr(beg, length, pattern, patter_length, caps);
		if(index < 0)
			break;

		++count;
		length -= patter_length + index;
		beg += patter_length + index;
	}
	return count;
}

template<typename CharT>
bool textbegwith(const CharT * text, int_x length, const CharT & ch, bool caps = true)
{
	if(length < 0)
		length = textlen(text);

	if(!length)
		return false;
	else if(caps)
		return text[0] == ch;
	else
		return tolower(text[0]) == tolower(ch);
}

template<typename CharT>
bool textbegwith(const CharT * text, int_x length, const CharT * pattern, int_x pattern_length, bool caps = true)
{
	if(length < 0)
		length = textlen(text);
	if(pattern_length < 0)
		pattern_length = textlen(pattern);

	if(!length || !pattern_length || length < pattern_length)
		return false;

	if(textcmp(text, pattern_length, pattern, pattern_length, caps) == 0)
		return true;
	else
		return false;
}

template<typename CharT>
bool textendwith(const CharT * text, int_x length, const CharT & ch)
{
	if(length < 0)
		length = textlen(text);

	if(!length)
		return false;
	else if(caps)
		return text[length - 1] == ch;
	else
		return tolower(text[length - 1]) == tolower(ch);
}

template<typename CharT>
bool textendwith(const CharT * text, int_x length, const CharT * pattern, int_x pattern_length, bool caps = true)
{
	if(length < 0)
		length = textlen(text);
	if(pattern_length < 0)
		pattern_length = textlen(pattern);

	if(!length || !pattern_length || length < pattern_length)
		return false;

	const CharT * pText = text + (length - pattern_length);
	if(textcmp(pText, pattern_length, pattern, pattern_length, caps) == 0)
		return true;
	else
		return false;
}

template<typename CharT>
int_x textcat(CharT * dst, int_x size, const CharT * src, int_x length = -1)
{
	if(length < 0)
		length = textlen(src);
	if(!dst || size <= 1 || !src || !length)
		return 0;
	else
	{
#ifdef _DEBUG
		bool found_null = false;
		for(CharT * pTemp = dst, *pEnd = dst + size; pTemp < pEnd; ++pTemp)
		{
			if(*pTemp == 0)
			{
				found_null = true;
				break;
			}
		}
		if(!found_null)
			verify(false);
#endif
		int_x dst_length = textlen(dst, size);
		return textcpy(dst + dst_length, size - dst_length, src, length);
	}
}

template<typename CharT>
int_x textcat_format(CharT * buffer, int_x size, const CharT * format, ...)
{
	if(!buffer || size <= 1 || !format)
		return 0;
	else
	{
		int_x length = textlen(buffer, size);
		va_list pArgs = nullptr;
		va_start(pArgs, format);
		int_x length2 = textformat_args(buffer + length, size - length, format, pArgs);
		va_end(pArgs);

		return length + length2;
	}
}

// 合并 szSrcA 和 szSrcB 到 szDst
template<typename CharT>
int_x textcompose(CharT * dst, int_x size,
				  const CharT * src1, int_x src1_length,
				  const CharT * src2, int_x src2_length)
{
	if(!dst || size <= 0)
		return 0;
	else if(size == 1)
	{
		*dst = 0;
		return 0;
	}
	if(!src1 || !src1_length)
		return textcpy(dst, size, src2, src2_length);
	else if(!src2 || !src2_length)
		return textcpy(dst, size, src1, src1_length);
	else
	{
		int_x length = 0;
		while(size > 1 && src1_length && *src1)
		{
			*dst++ = *src1++;
			--size;
			--src1_length;
			++length;
		}
		while(size > 1 && src2_length && *src2)
		{
			*dst++ = *src2++;
			--size;
			--src2_length;
			++length;
		}
		*dst = 0;
		return length;
	}
}

// 合并 piLength 个 pszSrc 到 szDst
template<typename CharT>
int_x textcomposeex(CharT * dst, int_x size,
					const CharT ** arr_src, int_x * arr_length, int_x count)
{
	if(!dst || size <= 0)
		return 0;
	else if(size == 1)
	{
		*dst = 0;
		return 0;
	}
	else
	{
		int_x length = 0;
		const CharT * pSrc = nullptr;
		for(int_x cnt = 0; cnt < count && size > 1; ++cnt)
		{
			int_x src_length = arr_length[cnt];
			pSrc = arr_src[cnt];

			while(size > 1 && src_length && *pSrc)
			{
				*dst++ = *pSrc++;
				--size;
				--src_length;
				++length;
			}
		}
		*dst = 0;
		return length;
	}
}

template<typename CharT>
int_x texttrim(CharT * text, int_x length = -1, const CharT * trim_chars = nullptr, int_x trim_chars_length = -1, bool caps = true)
{
	if(length < 0)
		length = textlen(text);
	if(!length)
		return 0;

	if(trim_chars)
	{
		if(trim_chars_length < 0)
			trim_chars_length = textlen(trim_chars);
		if(!trim_chars_length)
			return 0;
		CharT * left = text;
		CharT * end = text + length;
		CharT ch = 0;

		while(left != end)
		{
			ch = *left;
			if(textch(trim_chars, trim_chars_length, ch, caps) >= 0)
				++left;
			else
				break;
		}

		if(left == end)
		{
			*text = 0;
			return length;
		}
		else
		{
			CharT * right = end;
			while(right != left)
			{
				ch = *(right - 1);
				if(textch(trim_chars, trim_chars_length, ch) >= 0)
					--right;
				else
					break;
			}

			int_x ret = length - (right - left);
			CharT * dst = text;
			while(left != right)
				*dst++ = *left++;
			*dst = 0;
			return ret;
		}
	}
	else
	{
		CharT * left = text;
		CharT * end = left + length;
		CharT ch = 0;
		while(left != end)
		{
			ch = *left;
			if(ch == ' ' || ch == '\t')
				++left;
			else
				break;
		}

		if(left == end)
		{
			*text = 0;
			return length;
		}
		else
		{
			CharT * right = end;
			while(right != left)
			{
				ch = *(right - 1);
				if(ch == ' ' || ch == '\t')
					--right;
				else
					break;
			}

			int_x ret = length - (right - left);
			CharT * dst = text;
			while(left != right)
				*dst++ = *left++;
			*dst = 0;
			return ret;
		}
	}
	return 0;
}


// ---------------------------------------------- 文件路径相关。
const int_x MAX_FILE_PATH = 260;

/**
* @brief 修正路径中的 "./" 和 "../" 标记。
* "./" 表示当前目录。
* "../" 表示上一层目录。
* 路径中的 '\' 会被替换为 '/'。
*/
template<typename CharT>
int_x textpathnormalize(CharT * path, int_x length = -1)
{
	if(length < 0)
		length = textlen(path);

	textrplch(path, length, (CharT)'\\', (CharT)'/');
	if(!length)
		return -1;

	int_x point = 0;

	CharT * pBeg = path;
	CharT * pEnd = pBeg + length;

	CharT * pPath = pBeg;
	CharT * pSlash = pBeg;
	CharT * pWrite = pBeg;
	CharT ch = 0;

	while(pPath != pEnd)
	{
		ch = *pPath;
		if(ch == '.')
			++point;
		else if(ch == '/')
		{
			// 忽略表示当前目录的 "./"。
			if(point == 1)
			{
				point = 0;
			}
			else if(point == 2)
			{
				if(pSlash == pBeg)
					break; // 没有更上层的目录了。

				--pSlash;
				// 回溯至上一个'/'。
				while(pSlash != pBeg)
				{
					if(*pSlash == '/')
						break;
					else
						--pSlash;
				}

				if(pSlash != pBeg)
					pWrite = pSlash + 1;
				else
					pWrite = pSlash;
				point = 0;
			}
			else
			{
				if(point > 2)
				{
					if(pWrite < pPath)
					{
						for(int_x cnt = 0; cnt < point; ++cnt)
							pWrite[cnt] = '.';
						pWrite += point;
						point = 0;
						*pWrite = ch;
						++pWrite;
					}
					point = 0;
				}
				else
				{
					if(pWrite < pPath)
						*pWrite = ch;
					++pWrite;
				}
				pSlash = pPath;
			}
		}
		else
		{
			if(pWrite < pPath)
			{
				for(int_x cnt = 0; cnt < point; ++cnt)
					pWrite[cnt] = '.';
				pWrite += point;
				point = 0;
				*pWrite = ch;
				++pWrite;
			}
			else
			{
				pWrite += point;
				++pWrite;
			}
		}
		++pPath;
	}

	if(pWrite > pBeg && *(pWrite - 1) == '/')
		--pWrite;

	if(pWrite < pEnd)
		*pWrite = 0;
	return pWrite - path;
}

/**
* @brief 将文件夹路径和相对目录生成完整目录。
* 内部会把 '\' 替换成 '/'。
*/
template<typename CharT>
int_x textmakepath(CharT * szPath, int_x iSize,
				   const CharT * szDirectory, int_x iDirectoryLength,
				   const CharT * szRelative, int_x iRelativeLength)
{
	textempty(szPath, iSize);
	if(!iSize)
		return 0;

	if(iDirectoryLength < 0)
		iDirectoryLength = textlen(szDirectory, MAX_FILE_PATH);
	if(iRelativeLength < 0)
		iRelativeLength = textlen(szRelative, MAX_FILE_PATH);

	const int_x COUNT = MAX_FILE_PATH * 2 + 4;
	CharT szTemp[COUNT] = {0};

	textcpy(szTemp, COUNT, szDirectory, iDirectoryLength);
	textrplch(szTemp, iDirectoryLength, (CharT)'\\', (CharT)'/');

	// 防止 '/' 重复。
	bool bEndL = szTemp[iDirectoryLength - 1] == '/';
	bool bEndR = szRelative[0] == '/';
	if(bEndL && bEndR)
	{
		--iDirectoryLength;
	}
	else if(!bEndL && !bEndR)
	{
		szTemp[iDirectoryLength] = '/';
		++iDirectoryLength;
	}
	else {}

	textcpy(szTemp + iDirectoryLength, COUNT - iDirectoryLength, szRelative, iRelativeLength);
	textrplch(szTemp + iDirectoryLength, iRelativeLength, (CharT)'\\', (CharT)'/');

	int_x iLength = iDirectoryLength + iRelativeLength;
	iLength = textpathnormalize(szTemp, iLength);
	if(iLength >= 0)
		return textcpy(szPath, iSize, szTemp, iLength);
	else
		return 0;
}

template<typename CharT>
void textsplitpath(const CharT * szPath, int_x iLength,
				   CharT * szDrive, int_x iDriveSize,
				   CharT * szFolder, int_x iFolderSize,
				   CharT * szName, int_x iNameSize,
				   CharT * szExt, int_x iExtSize,
				   CharT * szFileName, int_x iFileNameSize,
				   int_x * pDriveLength = nullptr,
				   int_x * pDirLength = nullptr,
				   int_x * pNameLength = nullptr,
				   int_x * pExtLength = nullptr,
				   int_x * pFileNameLength = nullptr)
{
	textempty(szDrive, iDriveSize);
	textempty(szFolder, iFolderSize);
	textempty(szName, iNameSize);
	textempty(szExt, iExtSize);
	textempty(szFileName, iFileNameSize);

	if(iLength < 0)
		iLength = textlen(szPath);

	if(!iLength || !szPath)
		return;

	if(szPath[iLength - 1] == '/' || szPath[iLength - 1] == '\\')
		--iLength;

	const CharT * pPath = szPath;
	const CharT * pEnd = pPath + iLength;
	int_x iDriveLength = 0, iDirLength = 0, iNameLength = 0, iExtLength = 0, iFileNameLength = 0;

	if((!szDrive && iDriveSize) || (szDrive && !iDriveSize) ||
	   (!szFolder && iFolderSize) || (szFolder && !iFolderSize) ||
	   (!szName && iNameSize) || (szName && !iNameSize) ||
	   (!szExt && iExtSize) || (szExt && !iExtSize))
	   return;

	// 驱动器
	const CharT * pTemp = pPath;
	const CharT * pColon = nullptr;
	while(pTemp < pEnd && *pTemp)
	{
		if(*pTemp == ':')
		{
			pColon = pTemp;
			++pTemp;
			break;
		}
		else
			++pTemp;
	}

	if(!pColon)
		pTemp = pPath;
	else
		iDriveLength = textcpy(szDrive, iDriveSize, pPath, (pColon - pPath) + 1);

	// 目录
	pPath = pTemp;
	const CharT * pSlash = nullptr;
	while(pTemp < pEnd && *pTemp)
	{
		if(*pTemp == '\\' || *pTemp == '/')
			pSlash = pTemp;
		++pTemp;
	}

	if(!pSlash)
		pTemp = pPath;
	else
	{
		iDirLength = textcpy(szFolder, iFolderSize, szPath, pSlash - szPath);
		pTemp = pSlash + 1;
	}

	// 名字
	pPath = pTemp;
	iNameLength = textcpy(szName, iNameSize, pPath, pEnd - pPath);
	const CharT * pDot = nullptr;
	while(pTemp < pEnd && *pTemp)
	{
		if(*pTemp == '.')
			pDot = pTemp;
		++pTemp;
	}

	if(!pDot)
		iFileNameLength = textcpy(szFileName, iFileNameSize, pPath, pEnd - pPath);
	else
	{
		iFileNameLength = textcpy(szFileName, iFileNameSize, pPath, pDot - pPath);
		iExtLength = textcpy(szExt, iExtSize, pDot, pEnd - pDot);
	}

	if(pDriveLength) *pDriveLength = iDriveLength;
	if(pDirLength) *pDirLength = iDirLength;
	if(pNameLength) *pNameLength = iNameLength;
	if(pExtLength) *pExtLength = iExtLength;
	if(pFileNameLength) *pFileNameLength = iFileNameLength;
}

template<typename CharT>
int_x textsplitpath_drive(const CharT * szPath, int_x iLength, CharT * szDrive, int_x iDriveSize)
{
	int_x iDriveLength = 0;
	textsplitpath<CharT>(szPath, iLength,
						 szDrive, iDriveSize, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0,
						 &iDriveLength, nullptr, nullptr, nullptr, nullptr);
	return iDriveLength;
}

template<typename CharT>
int_x textsplitpath_folder(const CharT * szPath, int_x iLength, CharT * szFolder, int_x iFolderSize)
{
	int_x iFolderLength = 0;
	textsplitpath<CharT>(szPath, iLength,
						 nullptr, 0, szFolder, iFolderSize, nullptr, 0, nullptr, 0, nullptr, 0,
						 nullptr, &iFolderLength, nullptr, nullptr, nullptr);
	return iFolderLength;
}

template<typename CharT>
int_x textsplitpath_name(const CharT * szPath, int_x iLength, CharT * szName, int_x iNameSize)
{
	int_x iNameLength = 0;
	textsplitpath<CharT>(szPath, iLength,
						 nullptr, 0, nullptr, 0, szName, iNameSize, nullptr, 0, nullptr, 0,
						 nullptr, nullptr, &iNameLength, nullptr, nullptr);
	return iNameLength;
}

template<typename CharT>
int_x textsplitpath_ext(const CharT * szPath, int_x iLength, CharT * szExt, int_x iExtSize)
{
	int_x iExtLength = 0;
	textsplitpath<CharT>(szPath, iLength,
						 nullptr, 0, nullptr, 0, nullptr, 0, szExt, iExtSize, nullptr, 0,
						 nullptr, nullptr, nullptr, &iExtLength, nullptr);
	return iExtLength;
}

template<typename CharT>
int_x textsplitpath_filename(const CharT * szPath, int_x iLength, CharT * szFileName, int_x iFileNameSize)
{
	int_x iFileNameLength = 0;
	textsplitpath<CharT>(szPath, iLength,
						 nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, szFileName, iFileNameSize,
						 nullptr, nullptr, nullptr, nullptr, &iFileNameLength);
	return iFileNameLength;
}

/**
* @brief 解析一个完整 URL 中的协议、域名和文件名部分。
*/
template<typename CharT>
void textspliturl(const CharT * szUrl, int_x iLength,
				  CharT * szProtocol, int_x iProtocolSize,
				  CharT * szDomain, int_x iDomainSize,
				  CharT * szFile, int_x iFileSize,
				  int_x * piProtocolLength = nullptr,
				  int_x * piDomainLength = nullptr,
				  int_x * piFileLength = nullptr)
{
	textempty(szProtocol, iProtocolSize);
	textempty(szDomain, iDomainSize);
	textempty(szFile, iFileSize);

	const CharT * pUrl = szUrl;

	int_x iProtocolLength = 0, iDomainLength = 0, iFileLength = 0;
	const char * pSlashSlash = textstr(pUrl, iLength, "//", 2);
	if(pSlashSlash)
	{
		iProtocolLength = textcpy(szProtocol, iProtocolSize, pUrl, pSlashSlash - pUrl);
		pUrl = pSlashSlash + 2;
		int_x iSlash = textch(pUrl, iLength, '/');
		if(iSlash >= 0)
		{
			iDomainLength = textcpy(szDomain, iDomainSize, pUrl, iSlash);
			iFileLength = textcpy(szFile, iFileSize, szFile + iSlash + 1, szUrl + (iLength - iSlash + 1));
		}
		else {}
	}
	else {}
	if(piProtocolLength) *piProtocolLength = iProtocolLength;
	if(piDomainLength) *piDomainLength = iDomainLength;
	if(piFileLength) *piFileLength = iFileLength;
}

template<typename CharT>
void textspliturlprotocol(const CharT * szUrl, int_x iLength,
						  CharT * szProtocol, int_x iProtocolSize,
						  int_x * piProtocolLength = nullptr)
{
	textspliturl<CharT>(szUrl, iLength, szProtocol, iProtocolSize, nullptr, 0, nullptr, 0, piProtocolLength, nullptr, nullptr);
}

template<typename CharT>
void textspliturldomain(const CharT * szUrl, int_x iLength,
						CharT * szDomain, int_x iDomainSize,
						int_x * piDomainLength = nullptr)
{
	textspliturl<CharT>(szUrl, iLength, nullptr, 0, szDomain, iDomainSize, nullptr, 0, nullptr, piDomainLength, nullptr);
}

template<typename CharT>
void textspliturlfile(const CharT * szUrl, int_x iLength,
					  CharT * szFile, int_x iFileSize,
					  int_x * piFileLength = nullptr)
{
	textspliturl<CharT>(szUrl, iLength, nullptr, 0, nullptr, 0, szFile, iFileSize, nullptr, nullptr, piFileLength);
}

template<typename CharT>
bool texttobool(const CharT * szText, int_x iLength = -1)
{
	const CharT TRUE_STR[5] = {'t', 'r', 'u', 'e', '\0'};
	return textequalex<CharT>(szText, iLength, TRUE_STR, 4, false);
}

/// 将文本转换为整数
template<typename CharT, typename IntT>
IntT texttoi(const CharT * text, int_x length, int_x radix = 0)
{
	if(length < 0)
		length = textlen(text);

	while(length && *text == ' ')
	{
		++text;
		--length;
	}

	IntT sign = 1;
	if(text[0] == L'-')
	{
		sign = (IntT)-1;
		++text;
	}

	if(!length || !text)
		return 0;

	const CharT * curr = text;
	if(radix <= 0)
	{
		if(curr[0] == '0' && length >= 2)
		{
			if(curr[1] == 'b' || curr[1] == 'B' || curr[1] == '0')
			{
				radix = 2;
				curr += 2;
			}
			else if(curr[1] == 'x' || curr[1] == 'X')
			{
				radix = 16;
				curr += 2;
			}
			else if(curr[1] == 'h' || curr[1] == 'H')
			{
				radix = 8;
				curr += 2;
			}
			else if(curr[1] == 'd' || curr[1] == 'D')
			{
				radix = 10;
				curr += 2;
			}
			else
				radix = 10;
		}
		else
			radix = 10;
	}

	IntT integer = 0;
	IntT number = 0;
	CharT ch = 0;

	while(length--)
	{
		ch = *curr++;
		if(!ch)
			break;

		if(ch >= '0' && ch <= '9')
			number = ch - '0';
		else if(ch >= 'A' && ch <= 'Z')
			number = ch - 'A' + 10;
		else if(ch >= 'a' && ch <= 'z')
			number = ch - 'a' + 10;
		else
			break;

		if(number >= (IntT)radix)
			break;

		integer = integer * (IntT)radix + number;
	}
	return integer * sign;
}

template<typename CharT>
int_x texttoix(const CharT * text, int_x length, int_x radix = 0)
{
	return texttoi<CharT, int_x>(text, length, radix);
}

template<typename CharT>
int_32 texttoi32(const CharT * text, int_x length, int_x radix = 0)
{
	return texttoi<CharT, int_32>(text, length, radix);
}

template<typename CharT>
int_64 texttoi64(const CharT * text, int_x length, int_x radix = 0)
{
	return texttoi<CharT, int_64>(text, length, radix);
}

/// 将文本转换为整数
template<typename CharT, typename IntT>
int_x texttoiarr(const CharT * text, int_x length, IntT * arr, int_x count, CharT split = ',', int_x radix = 0)
{
	if(!text || !length || !arr || count <= 0)
		return 0;

	if(length < 0)
		length = textlen(text, 64);

	int_x index = 0;
	const CharT * beg = text;
	const CharT * end = text + length;
	for(int_x cnt = 0; cnt < count; ++cnt)
	{
		int_x count = textch<CharT>(beg, end - beg, split);
		if(count < 0)
		{
			if(beg < end)
				count = end - beg;
			else
				break;
		}

		arr[index++] = texttoi<CharT, IntT>(beg, count, radix);
		beg += count + 1;
	}
	return index;
}

/// 将文本转换为浮点数
template<typename CharT, typename FloatT>
FloatT texttof(const CharT * text, int_x length = -1)
{
	if(length < 0)
		length = textlen<CharT>(text, 32);

	if(!length)
		return (FloatT)0;

	const CharT * end = text + length;
	bool sign_radix = true;
	bool sign_exp = true;
	// reading integer part.
	bool read_integer = true;
	// reading exp part.
	bool read_exp = false;

	// radix = integer + decimal.
	// integer part.
	FloatT integer = 0;
	// decimal part.
	FloatT decimal = 0;
	// exp part.
	int_x exp = 0;
	// use to caculate decimal.
	FloatT decimal_index = (FloatT)1 / (FloatT)10;

	if(text[0] == '-')
	{
		sign_radix = false;
		++text;
	}
	else {}

	while(text < end)
	{
		CharT ch = *text++;
		if(ch == '-')
		{
			if(read_exp)
				sign_exp = false;
			else
				break;
		}
		else if(ch == '.')
		{
			read_integer = false;
		}
		else if(ch == 'E' || ch == 'e')
		{
			read_integer = false;
			read_exp = true;
		}
		else if('0' <= ch && ch <= '9')
		{
			int_x number = (int_x)(ch - '0');
			if(read_exp)
			{
				exp *= 10;
				exp += number;
			}
			else if(read_integer)
			{
				integer *= 10;
				integer += number;
			}
			else
			{
				decimal += number * decimal_index;
				decimal_index *= (FloatT)0.1;
			}
		}
		else
			break;
	}

	FloatT result = (FloatT)(integer + decimal);
	if(!sign_radix)
		result = -result;

	if(sign_exp)
	{
		for(int_x cnt = 0; cnt < exp; ++cnt)
			result *= 10;
	}
	else
	{
		for(int_x cnt = 0; cnt < exp; ++cnt)
			result *= (FloatT)0.1;
	}

	return result;
}

template<typename CharT>
float_32 texttof32(const CharT * text, int_x length = -1)
{
	return texttof<CharT, float_32>(text, length);
}

template<typename CharT>
float_64 texttof64(const CharT * text, int_x length = -1)
{
	return texttof<CharT, float_64>(text, length);
}

template<typename CharT, typename FloatT>
int_x texttofarr(const CharT * text, int_x length, FloatT * arr, int_x count, CharT split = ',')
{
	if(!text || !length || !arr || count <= 0)
		return 0;

	if(length < 0)
		length = textlen(text, 64);

	int_x index = 0;
	const CharT * beg = text;
	const CharT * end = text + length;
	for(int_x cnt = 0; cnt < count; ++cnt)
	{
		int_x count = textch<CharT>(beg, end - beg, split);
		if(count < 0)
		{
			if(beg < end)
				count = end - beg;
			else
				break;
		}

		arr[index++] = texttof<CharT, FloatT>(beg, count);
		beg += count + 1;
	}
	return index;
}


/// 将整数转换为文本
template<typename CharT, typename IntT>
int_x textfromi(CharT * text, int_x size, IntT value, int_x radix = 0, bool caps = false)
{
	if(radix <= 0)
		radix = 10;

	// 进制不合法
	if(radix < 2 || radix > 35)
	{
		if(text && size)
			text[0] = 0;
		return 0;
	}
	int_x length = 0;
	bool sign = true;
	if(value < 0)
	{
		value = value * (IntT)(-1);
		sign = false;
		++length;
	}

	IntT temp = value;
	do
	{
		++length;
		temp /= radix;
	}
	while(temp);

	if(!text)
		return length;
	else if(length >= size)
	{
		if(text && size)
			text[0] = 0;
		return length;
	}
	else {}

	int_x index = length;
	if(!sign)
	{
		text[0] = '-';
	}

	CharT ch_a = caps ? 'A' : 'a';
	IntT number = 0;
	do
	{
		number = value % radix;
		value /= radix;

		if(text)
		{
			if(number > 9)
				text[--index] = (CharT)(ch_a + (number - 10));
			else
				text[--index] = (CharT)('0' + number);
		}
	}
	while(value);
	text[length] = '\0';
	return length;
}

template<typename CharT>
int_x textfromix(CharT * text, int_x size, int_x value, int_x radix = 0, bool caps = false)
{
	return textfromi(text, size, value, radix, caps);
}

/// 判断两个文本是否相等
template<typename CharT>
bool textequalex(const CharT * text, int_x length, const CharT * pattern, int_x pattern_length, bool caps = true)
{
	if(!text || !pattern)
		return false;

	if(length < 0)
		length = textlen<CharT>(text);
	if(pattern_length < 0)
		pattern_length = textlen<CharT>(pattern);

	if(length != pattern_length)
		return false;
	else
	{
		if(caps)
		{
			while(length--)
			{
				if(*text != *pattern)
					return false;

				++text;
				++pattern;
			}
			return true;
		}
		else
		{
			while(length--)
			{
				if(chlower(*text) != chlower(*pattern))
					return false;

				++text;
				++pattern;
			}
			return true;
		}
	}
}

//! @brief 判断两个文本是否相等
template<typename CharT>
bool textequal(const CharT * text, const CharT * pattern)
{
	return textequalex(text, -1, pattern, -1, true);
}

/// 判断两个文本是否部分相等
template<typename CharT>
bool textnequal(const CharT * text, int_x length, const CharT * pattern, int_x pattern_length, bool caps = true)
{
	if(!text || !pattern)
		return false;

	if(length < 0)
		length = textlen<CharT>(text);
	if(pattern_length < 0)
		pattern_length = textlen<CharT>(pattern);

	if(pattern_length > length)
		return false;
	else
	{
		if(caps)
		{
			while(pattern_length--)
			{
				if(*text != *pattern)
					return false;

				++text;
				++pattern;
			}
			return true;
		}
		else
		{
			while(pattern_length--)
			{
				if(*text != *pattern)
				{
					if(chlower(*text) != chlower(*pattern))
						return false;
					else {}
				}

				++text;
				++pattern;
			}
			return true;
		}
	}
}

/**
* @brief 移除文本中的一个字符。
* @param pText 要操作的文本。
* @param iLength 要操作的文本的长度。
* @param iSize 要操作的文本的容量。
* @param iIndex 移除的字符索引。
*/
template<typename CharT>
void textremove(CharT * text, int_x length, int_x size, int_x index)
{
	if(length <= size &&
	   index >= 0 && index < length)
	{
		if(index != length - 1)
			buffmov(text + index, length - index, text + index + 1, length - (index + 1));
		text[length - 1] = '\0';
	}
}

/**
* @brief 移除文本的一部分。
* @param pText 要操作的文本。
* @param iLength 要操作的文本的长度。
* @param iSize 要操作的文本的容量。
* @param iIndex 移除部分的起始索引。
* @param iCount 移除的字符个数。
*/
template<typename CharT>
void textremove(CharT * text, int_x length, int_x size, int_x index, int_x count)
{
	if(length < 0)
		length = textlen(text);

	if(length < size &&
	   index >= 0 && index <= length &&
	   count > 0 && index + count <= length)
	{
		buffmov(text + index, length - index, text + index + count, length - (index + count));
		text[length - count] = '\0';
	}
}

/**
* @brief 在文本中插入一个字符。
* @param pText 要操作的文本。
* @param iLength 要操作的文本的长度。
* @param iSize 要操作的文本的容量。
* @param iIndex 插入的位置索引。
*/
template<typename CharT>
int_x textinsert(CharT * text, int_x length, int_x size, int_x index, const CharT & ch)
{
	if(length < 0)
		length = textlen(text);

	if(length + 1 <= size &&
		index >= 0 && index <= length)
	{
		if(index != length)
			buffmov(text + index + 1, size - index - 1, text + index, length - index);
		text[index] = ch;
		text[length + 1] = '\0';
		return length + 1;
	}
	else
		return -1;
}

/**
* @brief 在文本中插入另一段文本。
* @param pText 要操作的文本。
* @param iLength 要操作的文本的长度。
* @param iSize 要操作的文本的容量。
* @param pSrc 插入的文本。
* @param iCount 插入的字符数量，如果小于 0，则插入全部字符。
* @param iIndex 插入的位置索引。
*/
template<typename CharT>
int_x textinsert(CharT * text, int_x length, int_x size, int_x index, const CharT * src, int_x src_length = -1)
{
	if(length < 0)
		length = textlen(text);

	if(src_length < 0)
		src_length = textlen(src);

	if(length + src_length < size &&
		index >= 0 && index <= length)
	{
		buffmov(text + index + src_length, size - (index + src_length), text + index, length - index);
		buffcpy(text + index, size - index, src, src_length);
		text[length + src_length] = '\0';
		return length + src_length;
	}
	else
		return -1;
}

/**
* @brief 计算指定的文本和参数格式化后所需的缓冲区大小(包括终止符)。
* @param format 格式化字符串。
* @param ... 参数。
*/
int_x CORE_API textformat_calc(const char_8 * format, ...);

/**
* @copydoc textformat_calc(const char_8 * format, ...) 。
*/
int_x CORE_API textformat_calc(const char_16 * format, ...);

/**
* @brief 计算指定的文本和参数列表格式化后所需的缓冲区大小(包括终止符)。
* @param format 格式化字符串。
* @param args 参数列表。
*/
int_x CORE_API textformatargs_calc(const char_8 * format, void * args);

/**
* @copydoc TextFormatCalcArgs(const char_8 * format, void * args) 。
*/
int_x CORE_API textformatargs_calc(const char_16 * format, void * args);

/**
* @brief 将格式啊字符串格式化至指定缓冲区内。
* @param buffer 目标缓冲区。
* @param size 目标缓冲区的大小(包括终止符)。
* @param format 格式化字符串。
* @param ... 参数列表。
*/
int_x CORE_API textformat(char_8 * buffer, int_x size, const char_8 * format, ...);

/**
* @copydoc TextFormatCalcArgs(const char_16 * format, void * args);
*/
int_x CORE_API textformat(char_16 * buffer, int_x size, const char_16 * format, ...);

/**
* @brief 将格式啊字符串格式化至指定缓冲区内。
* @param buffer 目标缓冲区。
* @param size 目标缓冲区的大小(包括终止符)。
* @param format 格式化字符串。
* @param args 参数列表指针。
*/
int_x CORE_API textformat_args(char_8 * buffer, int_x size, const char_8 * format, void * args);

/**
* @copydoc TextFormatArgs(char_8 * buffer, int_x size, const char_8 * format, void * args);
*/
int_x CORE_API textformat_args(char_16 * buffer, int_x size, const char_16 * format, void * args);

/**
* @brief 判断字符是否是可显示的。
* 可显示字符包括标点符号、大小写字母、数字以及大于0xFF的所有字符。
*/
CORE_API bool isprintchar(char_32 ch);

CORE_API int_x ansitounicode(const char_8 * src, int_x length, char_16 * dst, int_x size);
CORE_API int_x unicodetoansi(const char_16 * src, int_x length, char_8 * dst, int_x size);

VENUS_END
