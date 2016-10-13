#pragma once

VENUS_BEG

struct SHFileInfoT
{
	int_64 size;
	int_64 tCreate;
	int_64 tRead;
	int_64 tWrite;
	chbufferw<64> chbType;
};
const oid_t OID_CSHFileInfoBuffer = {L"CSHFileInfoBuffer"};
class WIN32_API CSHFileInfoBuffer : public ObjectT<IService>
{
public:
	CSHFileInfoBuffer();
	~CSHFileInfoBuffer();

	// 接收者负责 AddRef 和 Release。
	IWin32ShellItemImage * GetShellIcon(const char_x * szPathOrExt);
	SHFileInfoT GetInfo(const char_x * szFile);

private:
	dictionary<int_x, IWin32ShellItemImage *> m_images;
	void * m_pbctx;
	void * m_pbctx_folder;
};

VENUS_END
