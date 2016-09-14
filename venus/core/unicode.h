#pragma once

VENUS_BEG

const char_32 LEAD_SURROGATE_MIN = 0xD800;
const char_32 LEAD_SURROGATE_MAX = 0xDBFF;
const char_32 TRAIL_SURROGATE_MIN = 0xDC00;
const char_32 TRAIL_SURROGATE_MAX = 0xDFFF;
const int_x MAX_UNICODE_PLANE_NAME = 64;

enum unicodeplane_e
{
	unicodeplane_invalid = -1,
	unicodeplane_control = 0,

	unicodeplane_arabic = 14,
	unicodeplane_oriya = 26,
	unicodeplane_sinhala = 32,
	unicodeplane_general_punctuation = 71,
	// 谚文音节，汉字化的朝鲜文.
	unicodeplane_hangul_syllables = 143,
	unicodeplane_utf16_high = 145,
	unicodeplane_utf16_low = 146,

	unicodeplane_cjk_radicals_supplement = 102,
	unicodeplane_cjk_kangxi_radicals = 103,
	unicodeplane_cjk_symbols_and_punctuation = 105,
	unicodeplane_cjk_strokes = 112,
	unicodeplane_cjk_enclosed_letters_and_months = 114,
	unicodeplane_cjk_compatibility = 115,
	unicodeplane_cjk_unified_ideographs_ext_a = 116,
	unicodeplane_cjk_unified_ideographs = 118,
	unicodeplane_cjk_compatibility_ideographs = 148,
	unicodeplane_cjk_compatibility_forms = 154,
	unicodeplane_cjk_unified_ideographs_ext_b = 179,
	unicodeplane_cjk_compatibility_supplement = 180
	,
	unicodeplane_count = 200,
};

struct unicodeplane_t
{
	int_x plane;
	char_32 beg;
	char_32 end;
	char_x name[MAX_UNICODE_PLANE_NAME];
};

CORE_API const unicodeplane_t & unicodeplane_get(int_x plane);
CORE_API const unicodeplane_t & unicodeplane_find(char_32 ch);
CORE_API const unicodeplane_t & unicodeplane_find(const char_16 * text, int_x length);

CORE_API int_x unicode_to_utf16(char_32 ch, char_16 * text, int_x size);
CORE_API int_x utf16_to_unicode(const char_16 * text, int_x size, char_32 & ch);

VENUS_END
