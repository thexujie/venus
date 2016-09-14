#pragma once

VENUS_BEG

//	���ݿ����	���ݿ�����			�����ݿ�	��ѡ��	λ������
//	IHDR		�ļ�ͷ���ݿ�			��		��		��һ��
//	cHRM		��ɫ�Ͱ�ɫ�����ݿ�		��		��		��PLTE��IDAT֮ǰ
//	gAMA		ͼ������ݿ�			��		��		��PLTE��IDAT֮ǰ
//	sBIT		������Чλ���ݿ�		��		��		��PLTE��IDAT֮ǰ
//	PLTE		��ɫ�����ݿ�			��		��		��IDAT֮ǰ
//	bKGD		������ɫ���ݿ�		��		��		��PLTE֮��IDAT֮ǰ
//	hIST		ͼ��ֱ��ͼ���ݿ�		��		��		��PLTE֮��IDAT֮ǰ
//	tRNS		ͼ��͸�����ݿ�		��		��		��PLTE֮��IDAT֮ǰ
//	oFFs		(ר�ù������ݿ�)		��		��		��IDAT֮ǰ
//	pHYs		�������سߴ����ݿ�		��		��		��IDAT֮ǰ
//	sCAL		(ר�ù������ݿ�)		��		��		��IDAT֮ǰ
//	IDAT		ͼ�����ݿ�			��		��		������IDAT����
//	tIME		ͼ������޸�ʱ�����ݿ�	��		��		������
//	tEXt		�ı���Ϣ���ݿ�		��		��		������
//	zTXt		ѹ���ı����ݿ�		��		��		������
//	fRAc		(ר�ù������ݿ�)		��		��		������
//	gIFg		(ר�ù������ݿ�)		��		��		������
//	gIFt		(ר�ù������ݿ�)		��		��		������
//	gIFx		(ר�ù������ݿ�)		��		��		������
//	IEND		ͼ���������			��		��		���һ�����ݿ�

#define PNG_MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
	((unsigned int)(unsigned char)(ch0) | ((unsigned int)(unsigned char)(ch1) << 8) | \
	((unsigned int)(unsigned char)(ch2) << 16) | ((unsigned int)(unsigned char)(ch3) << 24))

const img_u8 PNG_HEADER[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
const img_u8 PNG_TAIL[12] = {0x0, 0x0, 0x0, 0x0, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82};
#pragma pack(push, 1)
typedef enum tag_png_color_type
{
	png_color_type_invalid = -1,
	png_color_type_gray = 0,
	//png_color_type_palette,
	png_color_type_rgb = 2,
	png_color_type_index_rgb = 3,
	png_color_type_gray_alpha = 4,
	png_color_type_argb = 6,
}png_color_type_e;

typedef struct tag_png_header
{
	unsigned int width;
	unsigned int height;
	unsigned char bits;
	unsigned char color_type;
	unsigned char format;
	unsigned char filter;
	unsigned char interlace;
}png_header_t;

typedef enum tag_png_chunk_type
{
	png_chunk_type_ihdr = PNG_MAKEFOURCC('I', 'H', 'D', 'R'), // �ļ�ͷ --------
	png_chunk_type_chrm = PNG_MAKEFOURCC('c', 'H', 'R', 'M'), // ��ɫ�Ͱ�ɫ�����ݿ�
	png_chunk_type_gama = PNG_MAKEFOURCC('g', 'A', 'M', 'A'), // ͼ�� gama ���ݿ�
	png_chunk_type_sbits = PNG_MAKEFOURCC('s', 'B', 'I', 'T'), // ������Чλ���ݿ�
	png_chunk_type_plte = PNG_MAKEFOURCC('P', 'L', 'T', 'E'), // ��ɫ�����ݿ� ---------
	png_chunk_type_bkgd = PNG_MAKEFOURCC('b', 'K', 'G', 'D'), // ������ɫ���ݿ�
	png_chunk_type_hist = PNG_MAKEFOURCC('h', 'I', 'S', 'T'), // ͼ��ֱ��ͼ���ݿ�
	png_chunk_type_trns = PNG_MAKEFOURCC('t', 'R', 'N', 'S'), // ͼ��͸�����ݿ�
	png_chunk_type_offs = PNG_MAKEFOURCC('o', 'F', 'F', 's'), // (ר�ù������ݿ�)
	png_chunk_type_phys = PNG_MAKEFOURCC('p', 'H', 'Y', 's'), // �������سߴ����ݿ�
	png_chunk_type_scal = PNG_MAKEFOURCC('s', 'C', 'A', 'L'), // (ר�ù������ݿ�)
	png_chunk_type_idata = PNG_MAKEFOURCC('I', 'D', 'A', 'T'), // ͼ�����ݿ�
	png_chunk_type_time = PNG_MAKEFOURCC('t', 'I', 'M', 'E'), // ͼ������޸�ʱ�����ݿ�
	png_chunk_type_text = PNG_MAKEFOURCC('t', 'E', 'X', 't'), // �ı���Ϣ���ݿ�
	png_chunk_type_ztxt = PNG_MAKEFOURCC('z', 'T', 'X', 't'), // ѹ���ı����ݿ�
	png_chunk_type_frac = PNG_MAKEFOURCC('f', 'R', 'A', 'c'), // (ר�ù������ݿ�)
	png_chunk_type_gift = PNG_MAKEFOURCC('g', 'I', 'F', 't'), // (ר�ù������ݿ�)
	png_chunk_type_gifx = PNG_MAKEFOURCC('g', 'I', 'F', 'x'), // (ר�ù������ݿ�)
	png_chunk_type_iend = PNG_MAKEFOURCC('I', 'E', 'N', 'D'), // ͼ���������
}png_chunk_type_e;

typedef struct tag_png_chunk
{
	union
	{
		unsigned int size;
		struct
		{
			unsigned char size_4;
			unsigned char size_3;
			unsigned char size_2;
			unsigned char size_1;
		};
	};

	union
	{
		png_chunk_type_e type;
		unsigned char type_str[4];
	};
}png_chunk_t;

typedef struct tag_png_chunk_sbit
{
	unsigned int x1;
	unsigned int s_bit;
	unsigned int x2;
}png_chunk_sbit_t;

typedef struct tag_png_chunk_phys
{
	unsigned int ppux;
	unsigned int ppuy;
	unsigned char unit; // 0(δ֪), 1(��)
}png_chunk_phys_t;

#pragma pack(pop)

IMAGE_API bool is_png_data(const img_u8 * pBuffer, img_ix length);
IMAGE_API img_ix png_create(const img_u8 * buffer, img_ix length, image_data_t * img,
						  image_convert_rule_fun_t pfn_match = nullptr, void * user_data = nullptr);
IMAGE_API bool png_rule_default(image_convert_rule_t * rule);

const image_convert_fun_t image_convert_png_use_src = (image_convert_fun_t)-1;

VENUS_END
