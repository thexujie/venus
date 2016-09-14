#pragma once

#include "BaseInc.h"

enum RtfTagE
{
	RtfTag_unknown = 0,
	RtfTag_invalid,
	RtfTag_trunk_beg, // �ַ� {
	RtfTag_trunk_end, // �ַ� }
	RtfTag_semicolon, // �ֺ� ;
	RtfTag_char, // һ��MultiByte�ַ�������һ���֣�
	RtfTag_u, // һ��Unicode�ַ���Ҳ����һ���֣�
	RtfTag_ref, // ����
	// --------------------------------------------------------- ��������(����������)
	RtfTag_b, // ����Ӵ֡�
	RtfTag_ul, // �������»��ߡ�\ul0�ر������»��ߡ�
	RtfTag_ulnone, // ֹͣ�����»��ߡ�
	RtfTag_rtf, // RTF�汾�š�(N)
	RtfTag_tab, // һ��ˮƽ�Ʊ��(\t)��
	RtfTag_fcharset, // ������ַ�����(N)
	RtfTag_fs, // ����Ĵ�С(��λ����)��(N)
	RtfTag_f, // ���壬����������������0��ʼ��(N)
	RtfTag_fnil, // �����(FunT)
	RtfTag_ansi, // ����ʹ��Ansi�ַ�����
	RtfTag_ansicpg, // ʹ�õ�Ansi�ַ����Ĵ���ҳ��(N)
	RtfTag_deff, // �ĵ���Ĭ�����壬����������������0��ʼ��(N)
	RtfTag_nouicompat,
	RtfTag_fonttbl, // �����
	RtfTag_colortbl, // ��ɫ��
	RtfTag_red, // ��ɫ��ĺ�ɫ����[0,255](N)
	RtfTag_green, // ��ɫ�����ɫ����[0,255](N)
	RtfTag_blue, // ��ɫ�����ɫ����[0,255](N)
	RtfTag_viewkind, // 
	RtfTag_uc, // ����ʹ��Unicode�ַ����ֽ�����(N)
	RtfTag_pard, // ����ΪĬ�϶������ԡ�
	RtfTag_par, // ������
	RtfTag_cf, // ��ɫ����ɫ�������������1��ʼ��0��ʾʹ��Ĭ����ɫ��
	RtfTag_lang, // ���ݵ����ԡ�
	RtfTag_deflangfe, // 
	RtfTag_deflang, // ���ݵ�Ĭ�����ԡ�

	// --------------------------------------------------------- �������
	RtfTag_chbrdr, // �ַ��߿�(ÿ�߾��б߿�)��
	RtfTag_chshdng, // �ַ���Ӱ������N��ֵ������Ӱ�İٷֱȡ�(N)
	RtfTag_chcfpat, // N�Ǳ���ͼ������ɫ��ָ���ĵ���ɫ���һ��������(N)
	RtfTag_chcbpat, // N�����ɫ��ָ���ĵ���ɫ���һ��������(N)
	RtfTag_chbghoriz, // ָ��ˮƽ���ı�����ͼ����
	RtfTag_chbgvert, // ָ����ֱ���ı�����ͼ����
	RtfTag_chbgfdiag, // ָ������Խ����ı�����ͼ��(\\\\)��
	RtfTag_chbgbdiag, // ָ������Խ����ı�����ͼ��(\\\\)��
	RtfTag_chbgcross, // ָ��ʮ�����ı�����ͼ����
	RtfTag_chbgdcross, // ָ���Խ�ʮ�����ı�����ͼ����
	RtfTag_chbgdkhoriz, // ָ����ˮƽ���ı�����ͼ����
	RtfTag_chbgdkvert, // ָ���ִ�ֱ���ı�����ͼ����
	RtfTag_chbgdkfdiag, // ָ����ǰб���ı�����ͼ��(\\\\)��
	RtfTag_chbgdkbdiag, // ָ���ֺ�б���ı�����ͼ��(////)��
	RtfTag_chbgdkcross, // ָ����ʮ�����ı�����ͼ����
	RtfTag_chbgdkdcross, // ָ���ֶԽ�ʮ�����ı�����ͼ����

	// ------------------------------------------------------ �»������
	RtfTag_ulc, // �»�����ɫ��(N)
	RtfTag_uld, // ���»��ߡ�
	RtfTag_uldash, // �̻��»��ߡ�
	RtfTag_uldashd, // �㻮�»��ߡ�
	RtfTag_uldashdd, // ˫�㻮�»��ߡ�
	RtfTag_uldb, // ˫�»��ߡ�
	RtfTag_ulhwave, // ���ز����»��ߡ�
	RtfTag_ulldash, // �����»��ߡ�
	RtfTag_ulth, // ���»��ߡ�
	RtfTag_ulthd, // �ֵ��»��ߡ�
	RtfTag_ulthdash, // �ֶ̻��»��ߡ�
	RtfTag_ulthdashd, // �ֵ㻮�»��ߡ�
	RtfTag_ulthdashdd, // ��˫�㻮�»��ߡ�
	RtfTag_ulthldash, // �ֳ����»��ߡ�
	RtfTag_ululdbwave, // ˫�����»��ߡ�
	RtfTag_ulw, // ���¼��»��ߡ�
	RtfTag_ulwave, // �����»��ߡ�

	// ------------------------------------------------------ ����
	RtfTag_outl, // �߿�\outl0�ر�֮��
	RtfTag_scaps, // С���д��ĸ��\scaps 0�ر�֮��
	RtfTag_shad, // ��Ӱ��\ shad0�ر�֮��
	RtfTag_strike, // ɾ���ߡ�\strike0�ر�֮��
	RtfTag_striked1, // ˫ɾ���ߡ�\strike0�ر�֮��
	RtfTag_sub, // ����������Ϣ���±��ı�����С��ĳߴ硣
	RtfTag_super, // ����������Ϣ���ϱ��ı�����С��ĳߴ硣

	// ------------------------------------------------------ ���뷽ʽ
	RtfTag_qc, // ���ж��롣
	RtfTag_qj, // ���˶��롣
	RtfTag_ql, // ����루Ĭ�ϣ���
	RtfTag_qr, // �Ҷ��롣
	RtfTag_qd, // ��ɢ���롣
	RtfTag_qk, // ʹ��Kashida��������аٷֱȣ�0���͡�10���С�20���ߣ���(N)
	RtfTag_qt, // ����̩�ĵķ�ɢ���롣

	// ------------------------------------------------------ ����
	RtfTag_fi, // ����������Ĭ��Ϊ0����(N)
	RtfTag_cufi, // �����ַ���λ�İٷֱȵ���������ֵ�����Ը���\fiN�����ã���Ȼ���ǿ�����Ϊ��ֵͬ��(N)
	RtfTag_li, // ���������Ĭ��Ϊ0����(N)
	RtfTag_lin, // �������Ҷ�����������ֵ������ڴ�������������ʾ�Ҷ�����ֵ��Ĭ��Ϊ0����\linN�����˶�ǰ�ո�����(N)
	RtfTag_culi, // �����ַ���λ�İٷֱȵ��������ֵ����\linNһ���������Ը���\liN��\linN�����ã���Ȼ���ǿ�����Ϊ��ֵͬ��(N)
	RtfTag_ri, // ��������Ĭ��Ϊ0����(N)
	RtfTag_rin, // �������Ҷ�����Ҷ�����ֵ������ڴ�������������ʾ�������ֵ��Ĭ��Ϊ0����\rinN�����˶�ǰ�ո�����(N)
	RtfTag_curi, // �����ַ���λ�İٷֱȵ��Ҷ�����ֵ����\rinNһ���������Ը���\riN��\rinN�����ã���Ȼ���ǿ�����Ϊ��ֵͬ��(N)
	RtfTag_adjustright, // ���ĵ����񱻶���ʱ�Զ�������������

	// ------------------------------------------------------ �ı����
	RtfTag_sb, // �κ�����Ĭ��Ϊ0����(N)
	RtfTag_sa, // ��ǰ�����Ĭ��Ϊ0����(N)
	RtfTag_sbauto, // �Զ���ǰ����� 0������ǰ���ȡ����\sb��1�����Զ���ǰ��ࣨ����\sb����Ĭ��Ϊ0��(N)
	RtfTag_saauto, // �Զ��κ����� 0�����κ���ȡ����\sa��1�����Զ��κ��ࣨ����\sa����Ĭ��Ϊ0��(N)
	RtfTag_lisb, // �����ַ���λ�İٷֱȵĶ�ǰ���ֵ�����Ը���\sbN�����ã���Ȼ���ǿ�����Ϊ��ֵͬ��(N)
	RtfTag_lisa, // �����ַ���λ�İٷֱȵĶκ���ֵ�����Ը���\saN�����ã���Ȼ���ǿ�����Ϊ��ֵͬ��(N)
	RtfTag_sl, // �м�ࡣ���û��ʹ�øÿ����ֻ���ʹ��\sl0�����м�ཫ�����м��ַ����ֵ�Զ�ȡֵ����NΪһ����ֵ��
				// ���ֵ�������ڸ�ֵ�����м��ַ����ֵʱ��ʹ�ã�����ʹ���ַ����ֵ������N��һ����ֵ����ʹ����С
				// ���м��ַ����ֵʱ������ʹ��N�ľ���ֵ��(N)
	RtfTag_slmult, // �౶�м�ࡣָ����ǰ�м���ǵ����о�ı������ÿ�����ֻ�ܸ���\sl���������á�
					// 0 ����С�������ǡ���ȷ�����о�
					// 1 �౶�о࣬����ڡ��������оࡣ(N)
	RtfTag_nosnaplinegrid, // ȡ�����������ߡ�
};

RtfTagE GetRtfTag(const char_8 * pText, const char_8 * pEnd);
