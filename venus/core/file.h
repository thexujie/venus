#pragma once

VENUS_BEG

enum driver_e
{
	//! 非法的驱动器路径
	driver_invalid,
	//! 未知的驱动器
	driver_unknown,
	//! 可移动磁盘
	driver_removealbe,
	//! 本地磁盘
	driver_fixed,
	//! 网络位置
	driver_remote,
	//! CD 驱动器
	driver_CDROM,
	//! 虚拟磁盘
	driver_RAM,
};

const uint_32 FILE_ATTR_MASK = 0x3FFF;

typedef chbuffera<MAX_FILE_PATH> FilePathA;
typedef chbufferw<MAX_FILE_PATH> FilePathW;
typedef chbufferw<MAX_FILE_PATH> FilePathX;

/**
 * Name 路径中除了驱动器、父目录以外的部分，即包括扩展名。
 * FileName 只对文件有效，不包括扩展名的文件名。
 */
class CORE_API pathinfo_t
{
public:
	pathinfo_t();
	pathinfo_t(const char_x * szFile);
	pathinfo_t(const char_x * szDirectory, const char_x * szFile);

	void generate(const char_x * szFile, int_x iLength = -1);
	void generate(const char_x * szFolder, const char_x * szFile, int_x iFolderLength = -1, int_x iFileLength = -1);
	void refresh();

	int_x get_driver(char_x * szDriver, int_x iSize) const;
	int_x get_folder(char_x * szFolder, int_x iSize) const;
	int_x get_name(char_x * szName, int_x iSize) const;
	int_x get_ext(char_x * szExt, int_x iSize) const;

	textw get_driver() const;
	textw get_folder() const;
	textw get_name() const;
	textw get_ext() const;

	int_x get_paths(vector<pathinfo_t> & arrPaths, uint_32 uiAttrCare = 0, uint_32 uiAttr = 0, const char_x * szFilter = nullptr) const;
	vector<pathinfo_t> get_paths(uint_32 uiAttrCare = 0, uint_32 uiAttr = 0, const char_x * szFilter = nullptr) const;

	bool operator < (const pathinfo_t & another) const;

	char_x path[MAX_FILE_PATH];
	union
	{
		uint_32 _attributes;
		struct
		{
			//! 只读。
			bool readonly : 1;
			//! 隐藏。
			bool hidden : 1;
			//! 系统文件。
			bool system : 1;
			//! unused
			bool _unused1 : 1;
			//！ 是否是一个文件夹。
			bool folder : 1;
			//! 是否可存档。
			bool archive : 1;
			//! 是否是一个设备。
			bool device : 1;
			//! 是否是普通文件。
			bool normal : 1;
			//！ 是否是临时文件。
			bool temporary : 1;
			bool sparsefile : 1;
			bool reparsepoint : 1;
			//！ 是否被压缩存储。
			bool commpossed : 1;
			//！ 是否离线文件。
			bool offline : 1;
			bool notcontentindexed : 1;
			//！ 是否已经被加密。
			bool encrypted : 1;

			//! 文件是否存在。
			bool exists : 1;
			//! 是否是驱动器。
			bool driver : 1;
		};
	};
};

class CORE_API fileinfo_t : public pathinfo_t
{
public:
	fileinfo_t();
	fileinfo_t(const char_x * szFile);
	fileinfo_t(const char_x * szDirectory, const char_x * szFile);
	
	int_x get_filename(char_x * szFileName, int_x iSize) const;

	bool create(bool bForce = false);
	bool remove();

	bool rename(const char_x * szFileName, int_x iLength = -1, bool bForce = false);

	int_x size() const;
};


class CORE_API folderinfo_t : public pathinfo_t
{
public:
	folderinfo_t();
	folderinfo_t(const char_x * szFile);
	folderinfo_t(const char_x * szDirectory, const char_x * szFile);

	bool has_paths(uint_32 uiAttrCare = 0, uint_32 uiAttr = 0, const char_x * szFilter = nullptr) const;
	bool has_folders(uint_32 uiAttrCare = 0, uint_32 uiAttr = 0, const char_x * szFilter = nullptr) const;
	bool has_files(uint_32 uiAttrCare = 0, uint_32 uiAttr = 0, const char_x * szFilter = nullptr) const;
	int_x get_folders(vector<folderinfo_t> & arrFolders, uint_32 uiAttrCare = 0, uint_32 uiAttr = 0, const char_x * szFilter = nullptr) const;
	vector<folderinfo_t> get_folders(uint_32 uiAttrCare = 0, uint_32 uiAttr = 0, const char_x * szFilter = nullptr) const;
	int_x get_files(vector<fileinfo_t> & arrFiles, uint_32 uiAttrCare = 0, uint_32 uiAttr = 0, const char_x * szFilter = nullptr) const;
	vector<fileinfo_t> get_files(uint_32 uiAttrCare = 0, uint_32 uiAttr = 0, const char_x * szFilter = nullptr) const;
	
	bool create();
	//! 移除目录，不处理子文件。
	bool remove();
	//! 移除所有子文件。
	bool remove_all();
};

class CORE_API driverinfo_t : public folderinfo_t
{
public:
	driverinfo_t();
	driverinfo_t(const char_x * szDriver, int_x iLength = -1);

	driver_e type;
	char_x lable[MAX_FILE_PATH];
};

CORE_API int_x get_drivers(vector<driverinfo_t> & arrDrivers);
CORE_API vector<driverinfo_t> get_drivers();

class CORE_API File
{
public:
	File() = delete;

	static bool Exists(textw filePath);
	static bool Exists(texta filePath);

	static bool Copy(textw srcFilePath, textx dstFilePath, bool overwrite = false);
	static bool Copy(texta srcFilePath, texta dstFilePath, bool overwrite = false);
	static bool Move(textw srcFilePath, textx dstFilePath);
	static bool Move(texta srcFilePath, texta dstFilePath);

	static bool Delete(textw filePath);
	static bool Delete(texta filePath);

	static bool Create(textw filePath);
	static bool Create(texta filePath);

	static int_64 GetSize(textw filePath);
	static int_64 GetSize(texta filePath);

	static daytime_t GetCreationTime(textw filePath);
	static daytime_t GetCreationTime(texta filePath);
	static daytime_t GetAccessTime(textw filePath);
	static daytime_t GetAccessTime(texta filePath);
	static daytime_t GetWriteTime(textw filePath);
	static daytime_t GetWriteTime(texta filePath);
};

VENUS_END
