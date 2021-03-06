#include "stdafx.h"
#include "unicode.h"

VENUS_BEG

const unicodeplane_t UNICODE_PLANE_INVALID = {unicodeplane_invalid, 0, 0, _T("invalid plane")};
const unicodeplane_t UNICODE_PLANES[unicodeplane_count] =
{
	{0, 0x0000, 0x001F, _T("C0 Controls")},	   // C0控制符	C0 Controls
	{1, 0x0020, 0x007F, _T("Basic Latin")},	   // 基本拉丁文	Basic Latin
	{2, 0x0080, 0x009F, _T("C1 Controls")},	   // C1控制符	C1 Controls
	{3, 0x00A0, 0x00FF, _T("Latin 1 Supplement")},	   // 拉丁文补充-1	Latin 1 Supplement
	{4, 0x0100, 0x017F, _T("Latin Extended-A")},	   // 拉丁文扩展-A	Latin Extended-A
	{5, 0x0180, 0x024F, _T("Latin Extended-B")},	   // 拉丁文扩展-B	Latin Extended-B
	{6, 0x0250, 0x02AF, _T("IPA Extensions")},	   // 国际音标扩展	IPA Extensions
	{7, 0x02B0, 0x02FF, _T("Spacing Modifiers")},	   // 占位修饰符号	Spacing Modifiers
	{8, 0x0300, 0x036F, _T("Combining Diacritics Marks")},	   // 结合附加符号	Combining Diacritics Marks
	{9, 0x0370, 0x03FF, _T("Greek and Coptic")},	   // 希腊字母及科普特字母	Greek and Coptic
	{10, 0x0400, 0x04FF, _T("Cyrillic")},	   // 西里尔字母	Cyrillic
	{11, 0x0500, 0x052F, _T("Cyrillic Supplement")},	   // 西里尔字母补充	Cyrillic Supplement
	{12, 0x0530, 0x058F, _T("Armenian")},	   // 亚美尼亚字母	Armenian
	{13, 0x0590, 0x05FF, _T("Hebrew")},	   // 希伯来文	Hebrew
	{14, 0x0600, 0x06FF, _T("Arabic")},	   // 阿拉伯文	Arabic
	{15, 0x0700, 0x074F, _T("Syriac")},	   // 叙利亚文	Syriac
	{16, 0x0750, 0x077F, _T("Arabic Supplement")},	   // 阿拉伯文补充	Arabic Supplement
	{17, 0x0780, 0x07BF, _T("Thaana")},	   // 它拿字母	Thaana
	{18, 0x07C0, 0x07FF, _T("N'Ko")},	   // 西非书面语言	N'Ko
	{19, 0x0800, 0x083F, _T("Samaritan")},	   // 撒玛利亚字母	Samaritan
	{20, 0x0840, 0x085F, _T("Mandaic")},	   // 曼达语	Mandaic
	{21, 0x0860, 0x08FF, _T("")},	   // 待定	
	{22, 0x0900, 0x097F, _T("Devanagari")},	   // 天城文	Devanagari
	{23, 0x0980, 0x09FF, _T("Bengali")},	   // 孟加拉文	Bengali
	{24, 0x0A00, 0x0A7F, _T("Gurmukhi")},	   // 果鲁穆奇字母	Gurmukhi
	{25, 0x0A80, 0x0AFF, _T("Gujarati")},	   // 古吉拉特文	Gujarati
	{26, 0x0B00, 0x0B7F, _T("Oriya")},	   // 奥里亚文	Oriya
	{27, 0x0B80, 0x0BFF, _T("Tamil")},	   // 泰米尔文	Tamil
	{28, 0x0C00, 0x0C7F, _T("Telugu")},	   // 泰卢固文	Telugu
	{29, 0x0C80, 0x0CFF, _T("Kannada")},	   // 卡纳达文	Kannada
	{30, 0x0D00, 0x0D7F, _T("Malayalam")},	   // 马拉雅拉姆文	Malayalam
	{31, 0x0D80, 0x0DFF, _T("Sinhala")},	   // 僧伽罗文	Sinhala
	{32, 0x0E00, 0x0E7F, _T("Thai")},	   // 泰文	Thai
	{33, 0x0E80, 0x0EFF, _T("Lao")},	   // 老挝文	Lao
	{34, 0x0F00, 0x0FFF, _T("Tibetan")},	   // 藏文	Tibetan
	{35, 0x1000, 0x109F, _T("Myanmar")},	   // 缅甸文	Myanmar
	{36, 0x10A0, 0x10FF, _T("Georgian")},	   // 格鲁吉亚字母	Georgian
	{37, 0x1100, 0x11FF, _T("Hangul Jamo")},	   // 谚文字母	Hangul Jamo
	{38, 0x1200, 0x137F, _T("Ethiopic")},	   // 埃塞俄比亚语	Ethiopic
	{39, 0x1380, 0x139F, _T("Ethiopic Supplement")},	   // 埃塞俄比亚语补充	Ethiopic Supplement
	{40, 0x13A0, 0x13FF, _T("Cherokee")},	   // 切罗基字母	Cherokee
	{41, 0x1400, 0x167F, _T("Unified Canadian Aboriginal Syllabics")},	   // 统一加拿大土著语音节	Unified Canadian Aboriginal Syllabics
	{42, 0x1680, 0x169F, _T("Ogham")},	   // 欧甘字母	Ogham
	{43, 0x16A0, 0x16FF, _T("Runic")},	   // 卢恩字母	Runic
	{44, 0x1700, 0x171F, _T("Tagalog")},	   // 他加禄字母	Tagalog
	{45, 0x1720, 0x173F, _T("Hanunóo")},	   // 哈努诺文	Hanunóo
	{46, 0x1740, 0x175F, _T("Buhid")},	   // 布迪文	Buhid
	{47, 0x1760, 0x177F, _T("Tagbanwa")},	   // 塔格巴努亚文	Tagbanwa
	{48, 0x1780, 0x17FF, _T("Khmer")},	   // 高棉文	Khmer
	{49, 0x1800, 0x18AF, _T("Mongolian")},	   // 蒙古文	Mongolian
	{50, 0x18B0, 0x18FF, _T("Unified Canadian Aboriginal Syllabics Extended")},	   // 加拿大原住民音节文字扩展	Unified Canadian Aboriginal Syllabics Extended
	{51, 0x1900, 0x194F, _T("Limbu")},	   // 林布文	Limbu
	{52, 0x1950, 0x197F, _T("Tai Le")},	   // 德宏傣文	Tai Le
	{53, 0x1980, 0x19DF, _T("New Tai Lue")},	   // 新傣仂文	New Tai Lue
	{54, 0x19E0, 0x19FF, _T("Khmer Symbols")},	   // 高棉文符号	Khmer Symbols
	{55, 0x1A00, 0x1A1F, _T("Buginese")},	   // 布吉文	Buginese
	{56, 0x1A20, 0x1AAF, _T("Tai Tham")},	   // 老傣文	Tai Tham
	{57, 0x1AB0, 0x1AFF, _T("")},	   // 待定	
	{58, 0x1B00, 0x1B7F, _T("Balinese")},	   // 巴厘字母	Balinese
	{59, 0x1B80, 0x1BBF, _T("Sundanese")},	   // 巽他字母	Sundanese
	{60, 0x1BC0, 0x1BFF, _T("Batak")},	   // 巴塔克文	Batak
	{61, 0x1C00, 0x1C4F, _T("Lepcha")},	   // 雷布查字母	Lepcha
	{62, 0x1C50, 0x1C7F, _T("Ol Chiki")},	   // 桑塔利文	Ol Chiki
	{63, 0x1C80, 0x1CBF, _T("")},	   // 待定	
	{64, 0x1CC0, 0x1CCF, _T("Sudanese Supplement")},	   // 巽他字母补充	Sudanese Supplement
	{65, 0x1CD0, 0x1CFF, _T("Vedic Extensions")},	   // 吠陀梵文	Vedic Extensions
	{66, 0x1D00, 0x1D7F, _T("Phonetic Extensions")},	   // 语音学扩展	Phonetic Extensions
	{67, 0x1D80, 0x1DBF, _T("Phonetic Extensions Supplement")},	   // 语音学扩展补充	Phonetic Extensions Supplement
	{68, 0x1DC0, 0x1DFF, _T("Combining Diacritics Marks Supplement")},	   // 结合附加符号补充	Combining Diacritics Marks Supplement
	{69, 0x1E00, 0x1EFF, _T("Latin Extended Additional")},	   // 拉丁文扩展附加	Latin Extended Additional
	{70, 0x1F00, 0x1FFF, _T("Greek Extended")},	   // 希腊语扩展	Greek Extended
	{71, 0x2000, 0x206F, _T("General Punctuation")},	   // 常用标点	General Punctuation
	{72, 0x2070, 0x209F, _T("Superscripts and Subscripts")},	   // 上标及下标	Superscripts and Subscripts
	{73, 0x20A0, 0x20CF, _T("Currency Symbols")},	   // 货币符号	Currency Symbols
	{74, 0x20D0, 0x20FF, _T("Combining Diacritics Marks for Symbols")},	   // 组合用记号	Combining Diacritics Marks for Symbols
	{75, 0x2100, 0x214F, _T("Letterlike Symbols")},	   // 字母式符号	Letterlike Symbols
	{76, 0x2150, 0x218F, _T("Number Form")},	   // 数字形式	Number Form
	{77, 0x2190, 0x21FF, _T("Arrows")},	   // 箭头	Arrows
	{78, 0x2200, 0x22FF, _T("Mathematical Operator")},	   // 数学运算符	Mathematical Operator
	{79, 0x2300, 0x23FF, _T("Miscellaneous Technical")},	   // 杂项工业符号	Miscellaneous Technical
	{80, 0x2400, 0x243F, _T("Control Pictures")},	   // 控制图片	Control Pictures
	{81, 0x2440, 0x245F, _T("Optical Character Recognition")},	   // 光学识别符	Optical Character Recognition
	{82, 0x2460, 0x24FF, _T("Enclosed Alphanumerics")},	   // 封闭式字母数字	Enclosed Alphanumerics
	{83, 0x2500, 0x257F, _T("Box Drawing")},	   // 制表符	Box Drawing
	{84, 0x2580, 0x259F, _T("Block Element")},	   // 方块元素	Block Element
	{85, 0x25A0, 0x25FF, _T("Geometric Shapes")},	   // 几何图形	Geometric Shapes
	{86, 0x2600, 0x26FF, _T("Miscellaneous Symbols")},	   // 杂项符号	Miscellaneous Symbols
	{87, 0x2700, 0x27BF, _T("Dingbats")},	   // 印刷符号	Dingbats
	{88, 0x27C0, 0x27EF, _T("Miscellaneous Mathematical Symbols-A")},	   // 杂项数学符号-A	Miscellaneous Mathematical Symbols-A
	{89, 0x27F0, 0x27FF, _T("Supplemental Arrows-A")},	   // 追加箭头-A	Supplemental Arrows-A
	{90, 0x2800, 0x28FF, _T("Braille Patterns")},	   // 盲文点字模型	Braille Patterns
	{91, 0x2900, 0x297F, _T("Supplemental Arrows-B")},	   // 追加箭头-B	Supplemental Arrows-B
	{92, 0x2980, 0x29FF, _T("Miscellaneous Mathematical Symbols-B")},	   // 杂项数学符号-B	Miscellaneous Mathematical Symbols-B
	{93, 0x2A00, 0x2AFF, _T("Supplemental Mathematical Operator")},	   // 追加数学运算符	Supplemental Mathematical Operator
	{94, 0x2B00, 0x2BFF, _T("Miscellaneous Symbols and Arrows")},	   // 杂项符号和箭头	Miscellaneous Symbols and Arrows
	{95, 0x2C00, 0x2C5F, _T("Glagolitic")},	   // 格拉哥里字母	Glagolitic
	{96, 0x2C60, 0x2C7F, _T("Latin Extended-C")},	   // 拉丁文扩展-C	Latin Extended-C
	{97, 0x2C80, 0x2CFF, _T("Coptic")},	   // 科普特字母	Coptic
	{98, 0x2D00, 0x2D2F, _T("Georgian Supplement")},	   // 格鲁吉亚字母补充	Georgian Supplement
	{99, 0x2D30, 0x2D7F, _T("Tifinagh")},	   // 提非纳文	Tifinagh
	{100, 0x2D80, 0x2DDF, _T("Ethiopic Extended")},	   // 埃塞俄比亚语扩展	Ethiopic Extended
	{101, 0x2E00, 0x2E7F, _T("Supplemental Punctuation")},	   // 追加标点	Supplemental Punctuation
	{102, 0x2E80, 0x2EFF, _T("CJK Radicals Supplement")},	   // 中日韩部首补充	CJK Radicals Supplement
	{103, 0x2F00, 0x2FDF, _T("Kangxi Radicals")},	   // 康熙部首	Kangxi Radicals
	{104, 0x2FF0, 0x2FFF, _T("Ideographic Description Characters")},	   // 表意文字描述符	Ideographic Description Characters
	{105, 0x3000, 0x303F, _T("CJK Symbols and Punctuation")},	   // 中日韩符号和标点	CJK Symbols and Punctuation
	{106, 0x3040, 0x309F, _T("Hiragana")},	   // 日文平假名	Hiragana
	{107, 0x30A0, 0x30FF, _T("Katakana")},	   // 日文片假名	Katakana
	{108, 0x3100, 0x312F, _T("Bopomofo")},	   // 注音字母	Bopomofo
	{109, 0x3130, 0x318F, _T("Hangul Compatibility Jamo")},	   // 谚文兼容字母	Hangul Compatibility Jamo
	{110, 0x3190, 0x319F, _T("Kanbun")},	   // 象形字注释标志	Kanbun
	{111, 0x31A0, 0x31BF, _T("Bopomofo Extended")},	   // 注音字母扩展	Bopomofo Extended
	{112, 0x31C0, 0x31EF, _T("CJK Strokes")},	   // 中日韩笔画	CJK Strokes
	{113, 0x31F0, 0x31FF, _T("Katakana Phonetic Extensions")},	   // 日文片假名语音扩展	Katakana Phonetic Extensions
	{114, 0x3200, 0x32FF, _T("Enclosed CJK Letters and Months")},	   // 带圈中日韩字母和月份	Enclosed CJK Letters and Months
	{115, 0x3300, 0x33FF, _T("CJK Compatibility")},	   // 中日韩兼容	CJK Compatibility
	{116, 0x3400, 0x4DBF, _T("CJK Unified Ideographs Extension A")}, // 中日韩统一表意文字扩展A	 CJK Unified Ideographs Extension A
	{117, 0x4DC0, 0x4DFF, _T("Yijing Hexagrams Symbols")},	   // 易经六十四卦符号	Yijing Hexagrams Symbols
	{118, 0x4E00, 0x9FFF, _T("CJK Unified Ideographs")},	   // 中日韩统一表意文字	CJK Unified Ideographs
	{119, 0xA000, 0xA48F, _T("Yi Syllables")},	   // 彝文音节	Yi Syllables
	{120, 0xA490, 0xA4CF, _T("Yi Radicals")},	   // 彝文字根	Yi Radicals
	{121, 0xA4D0, 0xA4FF, _T("Lisu")},	   // 老傈僳文	Lisu
	{122, 0xA500, 0xA63F, _T("Vai")},	   // 瓦伊语	Vai
	{123, 0xA640, 0xA69F, _T("Cyrillic Extended-B")},	   // 西里尔字母扩展-B	Cyrillic Extended-B
	{124, 0xA6A0, 0xA6FF, _T("Bamum")},	   // 巴姆穆语	Bamum
	{125, 0xA700, 0xA71F, _T("Modifier Tone Letters")},	   // 声调修饰字母	Modifier Tone Letters
	{126, 0xA720, 0xA7FF, _T("Latin Extended-D")},	   // 拉丁文扩展-D	Latin Extended-D
	{127, 0xA800, 0xA82F, _T("Syloti Nagri")},	   // 锡尔赫特文	Syloti Nagri
	{128, 0xA830, 0xA83F, _T("Ind. No.")},	   // 	Ind. No.
	{129, 0xA840, 0xA87F, _T("Phags-pa")},	   // 八思巴字	Phags-pa
	{130, 0xA880, 0xA8DF, _T("Saurashtra")},	   // 索拉什特拉	Saurashtra
	{131, 0xA8E0, 0xA8FF, _T("Deva. Ext.")},	   // 	Deva. Ext.
	{132, 0xA900, 0xA92F, _T("Kayah Li")},	   // 克耶字母	Kayah Li
	{133, 0xA930, 0xA95F, _T("Rejang")},	   // 勒姜语	Rejang
	{134, 0xA980, 0xA9DF, _T("Javanese")},	   // 爪哇语	Javanese
	{135, 0xA9E0, 0xA9FF, _T("")},	   // 待定	
	{136, 0xAA00, 0xAA5F, _T("Cham")},	   // 占语字母	Cham
	{137, 0xAA60, 0xAA7F, _T("Myanmar ExtA")},	   // 缅甸语扩展	Myanmar ExtA
	{138, 0xAA80, 0xAADF, _T("Tai Viet")},	   // 越南傣文	Tai Viet
	{139, 0xAAE0, 0xAAFF, _T("Meetei Ext")},	   // 曼尼普尔文扩展	Meetei Ext
	{140, 0xAB00, 0xAB2F, _T("Ethiopic Ext-A")},	   // 埃塞俄比亚文	Ethiopic Ext-A
	{141, 0xAB30, 0xABBF, _T("")},	   // 待定	
	{142, 0xABC0, 0xABFF, _T("Meetei Mayek")},	   // 	Meetei Mayek
	{143, 0xAC00, 0xD7AF, _T("Hangul Syllables")},	   // 谚文音节	Hangul Syllables
	{144, 0xD7B0, 0xD7FF, _T("Hangul Jamo Extended-B")},	   // 谚文字母扩展-B	Hangul Jamo Extended-B
	{145, 0xD800, 0xDBFF, _T("High-half zone of UTF-16")},	   // 	High-half zone of UTF-16
	{146, 0xDC00, 0xDFFF, _T("Low-half zone of UTF-16")},	   // 	Low-half zone of UTF-16
	{147, 0xE000, 0xF8FF, _T("Private Use Zone")},	   // 自行使用区域	Private Use Zone
	{148, 0xF900, 0xFAFF, _T("CJK Compatibility Ideographs")},	   // 中日韩兼容表意文字	CJK Compatibility Ideographs
	{149, 0xFB00, 0xFB4F, _T("Alphabetic Presentation Forms")},	   // 字母表达形式（拉丁字母连字、亚美尼亚字母连字、希伯来文表现形式）	Alphabetic Presentation Forms
	{150, 0xFB50, 0xFDFF, _T("Arabic Presentation Forms A")},	   // 阿拉伯文表达形式A	Arabic Presentation Forms A
	{151, 0xFE00, 0xFE0F, _T("Variation Selector")},	   // 异体字选择符	Variation Selector
	{152, 0xFE10, 0xFE1F, _T("Vertical Forms")},	   // 竖排形式	Vertical Forms
	{153, 0xFE20, 0xFE2F, _T("Combining Half Marks")},	   // 组合用半符号	Combining Half Marks
	{154, 0xFE30, 0xFE4F, _T("CJK Compatibility Forms")},	   // 中日韩兼容形式	CJK Compatibility Forms
	{155, 0xFE50, 0xFE6F, _T("Small Form Variants")},	   // 小写变体形式	Small Form Variants
	{156, 0xFE70, 0xFEFF, _T("Arabic Presentation Forms B")},	   // 阿拉伯文表达形式B	Arabic Presentation Forms B
	{157, 0xFF00, 0xFFEF, _T("Halfwidth and Fullwidth Forms")},	   // 半角及全角	Halfwidth and Fullwidth Forms
	{158, 0xFFF0, 0xFFFF, _T("Specials")},	   // 特殊	Specials
	{159, 0x10000, 0x100FF, _T("Linear B")},   // 线形文字B	Linear B
	{160, 0x10100, 0x101CF, _T("Ancient numeric systems")},   // 古代记数系统	Ancient numeric systems
	{161, 0x101D0, 0x107FF, _T("Alphabetic and syllabic LTR scripts and sets of symbols")},   // 从左向右书写的音素和音节文字和符号集	Alphabetic and syllabic LTR scripts and sets of symbols
	{162, 0x10800, 0x10FFF, _T("Alphabetic and syllabic RTL scripts")},   // 从右向左书写的音素和音节文字	Alphabetic and syllabic RTL scripts
	{163, 0x11000, 0x11FFF, _T("Brahmic scripts")},   // 婆罗米文字	Brahmic scripts
	{164, 0x12000, 0x12FFF, _T("Cuneiform and other ancient scripts")},   // 楔形文字和其他古代文字	Cuneiform and other ancient scripts
	{165, 0x13000, 0x15BFF, _T("Egyptian and Maya hieroglyphs")},   // 埃及和玛雅圣书体	Egyptian and Maya hieroglyphs
	{166, 0x15C00, 0x15FFF, _T("Aztec pictograms")},   // 阿兹台克象形文字	Aztec pictograms
	{167, 0x16000, 0x16FFF, _T("Recently-devised scripts")},   // 新创文字	Recently-devised scripts
	{168, 0x17000, 0x1B5FF, _T("Large Asian scripts")},   // 亚洲大文字	Large Asian scripts
	{169, 0x1B600, 0x1BFFF, _T("unassigned")},   // 未分配	unassigned
	{170, 0x1C000, 0x1CDFF, _T("Micmac hieroglyphs")},   // 米格马赫圣书体	Micmac hieroglyphs
	{171, 0x1CE00, 0x1CFFF, _T("Proto-Elamite")},   // 原始埃兰文字	Proto-Elamite
	{172, 0x1D000, 0x1DFFF, _T("Notational systems")},   // 记号系统	Notational systems
	{173, 0x1E000, 0x1E7FF, _T("unassigned")},   // 未分配	unassigned
	{174, 0x1E800, 0x1EFFF, _T("RTL scripts")},   // 从右向左书写的文字	RTL scripts
	{175, 0x1F000, 0x1F0FF, _T("Game symbols")},   // 游戏符号	Game symbols
	{176, 0x1F100, 0x1F2FF, _T("Alphanumeric and ideographic sets")},   // 字母数字和表意字符集	Alphanumeric and ideographic sets
	{177, 0x1F300, 0x1F7FF, _T("Pictographic sets")},   // 象形字符集	Pictographic sets
	{178, 0x1F800, 0x1FFFD, _T("unassigned")},   // 未分配	unassigned

	// 中日韩统一表意文字扩展B CJK Unified Ideographs Extension B
	{179, 0x20000, 0x2A6D6, _T("CJK Unified Ideographs Extension B")},
	{180, 0x2F800, 0x2FA1D, _T("CJK Compatibility Supplement")},

};

const unicodeplane_t & unicodeplane_get(int_x plane)
{
	return UNICODE_PLANES[plane];
}

const unicodeplane_t & unicodeplane_find(char_32 ch)
{
	const int_x COUNT = sizeof(UNICODE_PLANES) / sizeof(unicodeplane_t);
	for(int_x cnt = 0; cnt < COUNT; ++cnt)
	{
		const unicodeplane_t & plane = UNICODE_PLANES[cnt];
		if(plane.beg <= ch && ch <= plane.end)
			return plane;
	}
	return UNICODE_PLANE_INVALID;
}

const unicodeplane_t & unicodeplane_find(const char_16 * text, int_x length)
{
	char_32 ch;
	if(utf16_to_unicode(text, length, ch) > 0)
		return unicodeplane_find(ch);
	else
		return UNICODE_PLANE_INVALID;
}

int_x unicode_to_utf16(char_32 ch, char_16 * text, int_x size)
{
	if(size < 0)
		size = textlen(text, 2);

	if(ch > 0xffff)
	{
		ch -= 0x10000;
		if(size >= 2)
		{
			const unicodeplane_t & plane_h = UNICODE_PLANES[unicodeplane_utf16_high];
			const unicodeplane_t & plane_l = UNICODE_PLANES[unicodeplane_utf16_low];
			text[0] = (ch >> 10) + plane_h.beg;
			text[1] = (ch & 0x03ff) + plane_l.beg;
			return 2;
		}
	}
	else
	{
		if(size >= 1)
		{
			*text = (char_16)ch;
			return 1;
		}
	}
	return 0;
}

int_x utf16_to_unicode(const char_16 * text, int_x length, char_32 & ch)
{
	if(length < 0)
		length = textlen(text, 2);

	const unicodeplane_t & plane_h = UNICODE_PLANES[unicodeplane_utf16_high];
	const unicodeplane_t & plane_l = UNICODE_PLANES[unicodeplane_utf16_low];
	if(length == 0)
		ch = 0;
	else if(length == 1)
	{
		char_16 ch0 = text[0];
		if(plane_h.beg <= ch0 && ch0 <= plane_h.end)
			ch = 0;
		else if(plane_l.beg <= ch0 && ch0 <= plane_l.end)
			ch = 0;
		else
			ch = ch0;
	}
	else if(length >= 2)
	{
		char_16 ch0 = text[0];
		char_16 ch1 = text[1];
		if(plane_h.beg <= ch0 && ch0 <= plane_h.end)
		{
			if(plane_l.beg <= ch1 && ch1 <= plane_l.end)
			{
				ch0 -= plane_h.beg;
				ch1 -= plane_l.beg;
				ch = (ch1 | (ch0 << 10)) + 0x10000;
			}
			else
				ch = 0;
		}
		else
			ch = ch0;
	}
	else
		ch = 0;

	if(ch > 0xffff)
		return 2;
	else if(ch)
		return 1;
	else
		return 0;
}

VENUS_END
