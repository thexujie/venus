#pragma once

#include "BaseInc.h"

enum RtfTagE
{
	RtfTag_unknown = 0,
	RtfTag_invalid,
	RtfTag_trunk_beg, // 字符 {
	RtfTag_trunk_end, // 字符 }
	RtfTag_semicolon, // 分号 ;
	RtfTag_char, // 一个MultiByte字符（或者一部分）
	RtfTag_u, // 一个Unicode字符（也或者一部分）
	RtfTag_ref, // 引用
	// --------------------------------------------------------- 基本功能(增加命中率)
	RtfTag_b, // 字体加粗。
	RtfTag_ul, // 连续的下划线。\ul0关闭所有下划线。
	RtfTag_ulnone, // 停止所有下划线。
	RtfTag_rtf, // RTF版本号。(N)
	RtfTag_tab, // 一个水平制表符(\t)。
	RtfTag_fcharset, // 字体的字符集。(N)
	RtfTag_fs, // 字体的大小(单位：磅)。(N)
	RtfTag_f, // 字体，字体表里的索引，从0开始。(N)
	RtfTag_fnil, // 字体簇(FunT)
	RtfTag_ansi, // 内容使用Ansi字符集。
	RtfTag_ansicpg, // 使用的Ansi字符集的代码页。(N)
	RtfTag_deff, // 文档的默认字体，字体表里的索引，从0开始。(N)
	RtfTag_nouicompat,
	RtfTag_fonttbl, // 字体表
	RtfTag_colortbl, // 颜色表
	RtfTag_red, // 颜色里的红色分量[0,255](N)
	RtfTag_green, // 颜色里的绿色分量[0,255](N)
	RtfTag_blue, // 颜色里的蓝色分量[0,255](N)
	RtfTag_viewkind, // 
	RtfTag_uc, // 内容使用Unicode字符的字节数。(N)
	RtfTag_pard, // 重置为默认段落属性。
	RtfTag_par, // 段落标记
	RtfTag_cf, // 颜色，颜色表里的索引，从1开始。0表示使用默认颜色。
	RtfTag_lang, // 内容的语言。
	RtfTag_deflangfe, // 
	RtfTag_deflang, // 内容的默认语言。

	// --------------------------------------------------------- 底纹相关
	RtfTag_chbrdr, // 字符边框(每边均有边框)。
	RtfTag_chshdng, // 字符阴影。参数N的值文字阴影的百分比。(N)
	RtfTag_chcfpat, // N是背景图案的颜色，指定文档颜色表的一个索引。(N)
	RtfTag_chcbpat, // N是填充色，指定文档颜色表的一个索引。(N)
	RtfTag_chbghoriz, // 指定水平线文本背景图案。
	RtfTag_chbgvert, // 指定垂直线文本背景图案。
	RtfTag_chbgfdiag, // 指定正向对角线文本背景图案(\\\\)。
	RtfTag_chbgbdiag, // 指定反向对角线文本背景图案(\\\\)。
	RtfTag_chbgcross, // 指定十字线文本背景图案。
	RtfTag_chbgdcross, // 指定对角十字线文本背景图案。
	RtfTag_chbgdkhoriz, // 指定粗水平线文本背景图案。
	RtfTag_chbgdkvert, // 指定粗垂直线文本背景图案。
	RtfTag_chbgdkfdiag, // 指定粗前斜线文本背景图案(\\\\)。
	RtfTag_chbgdkbdiag, // 指定粗后斜线文本背景图案(////)。
	RtfTag_chbgdkcross, // 指定粗十字线文本背景图案。
	RtfTag_chbgdkdcross, // 指定粗对角十字线文本背景图案。

	// ------------------------------------------------------ 下划线相关
	RtfTag_ulc, // 下划线颜色。(N)
	RtfTag_uld, // 点下划线。
	RtfTag_uldash, // 短划下划线。
	RtfTag_uldashd, // 点划下划线。
	RtfTag_uldashdd, // 双点划下划线。
	RtfTag_uldb, // 双下划线。
	RtfTag_ulhwave, // 加重波浪下划线。
	RtfTag_ulldash, // 长划下划线。
	RtfTag_ulth, // 粗下划线。
	RtfTag_ulthd, // 粗点下划线。
	RtfTag_ulthdash, // 粗短划下划线。
	RtfTag_ulthdashd, // 粗点划下划线。
	RtfTag_ulthdashdd, // 粗双点划下划线。
	RtfTag_ulthldash, // 粗长划下划线。
	RtfTag_ululdbwave, // 双波浪下划线。
	RtfTag_ulw, // 字下加下划线。
	RtfTag_ulwave, // 波浪下划线。

	// ------------------------------------------------------ 特殊
	RtfTag_outl, // 边框。\outl0关闭之。
	RtfTag_scaps, // 小体大写字母。\scaps 0关闭之。
	RtfTag_shad, // 阴影。\ shad0关闭之。
	RtfTag_strike, // 删除线。\strike0关闭之。
	RtfTag_striked1, // 双删除线。\strike0关闭之。
	RtfTag_sub, // 按照字体信息的下标文本和缩小点的尺寸。
	RtfTag_super, // 按照字体信息的上标文本和缩小点的尺寸。

	// ------------------------------------------------------ 对齐方式
	RtfTag_qc, // 居中对齐。
	RtfTag_qj, // 两端对齐。
	RtfTag_ql, // 左对齐（默认）。
	RtfTag_qr, // 右对齐。
	RtfTag_qd, // 分散对齐。
	RtfTag_qk, // 使用Kashida规则调整行百分比（0－低、10－中、20－高）。(N)
	RtfTag_qt, // 用于泰文的分散对齐。

	// ------------------------------------------------------ 缩进
	RtfTag_fi, // 首行缩进（默认为0）。(N)
	RtfTag_cufi, // 采用字符单位的百分比的首行缩进值，用以覆盖\fiN的设置，虽然它们可以设为相同值。(N)
	RtfTag_li, // 左端缩进（默认为0）。(N)
	RtfTag_lin, // 从左至右段落的左端缩进值；如果在从右至左段落则表示右端缩进值（默认为0）。\linN定义了段前空格数。(N)
	RtfTag_culi, // 采用字符单位的百分比的左端缩进值，与\linN一样，它用以覆盖\liN和\linN的设置，虽然它们可以设为相同值。(N)
	RtfTag_ri, // 右缩进（默认为0）。(N)
	RtfTag_rin, // 从左至右段落的右端缩进值；如果在从右至左段落则表示左端缩进值（默认为0）。\rinN定义了段前空格数。(N)
	RtfTag_curi, // 采用字符单位的百分比的右端缩进值，与\rinN一样，它用以覆盖\riN和\rinN的设置，虽然它们可以设为相同值。(N)
	RtfTag_adjustright, // 当文档网格被定义时自动调整右缩进。

	// ------------------------------------------------------ 文本间距
	RtfTag_sb, // 段后间隔（默认为0）。(N)
	RtfTag_sa, // 段前间隔（默认为0）。(N)
	RtfTag_sbauto, // 自动段前间隔： 0——段前间距取决于\sb。1——自动段前间距（忽略\sb）。默认为0。(N)
	RtfTag_saauto, // 自动段后间隔： 0——段后间距取决于\sa。1——自动段后间距（忽略\sa）。默认为0。(N)
	RtfTag_lisb, // 采用字符单位的百分比的段前间隔值，用以覆盖\sbN的设置，虽然它们可以设为相同值。(N)
	RtfTag_lisa, // 采用字符单位的百分比的段后间隔值，用以覆盖\saN的设置，虽然它们可以设为相同值。(N)
	RtfTag_sl, // 行间距。如果没有使用该控制字或者使用\sl0，则行间距将根据行间字符最高值自动取值。若N为一个正值，
				// 则该值将仅仅在该值大于行间字符最高值时才使用（否则，使用字符最高值）；分N是一个负值，即使在其小
				// 于行间字符最高值时，总是使用N的绝对值。(N)
	RtfTag_slmult, // 多倍行间距。指出当前行间距是单倍行距的倍数。该控制字只能跟在\sl后，联合作用。
					// 0 “最小”或者是“精确”的行距
					// 1 多倍行距，相对于“单倍”行距。(N)
	RtfTag_nosnaplinegrid, // 取消对齐网格线。
};

RtfTagE GetRtfTag(const char_8 * pText, const char_8 * pEnd);
