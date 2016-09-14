#pragma once

VENUS_BEG

struct PE_STRING32
{
	uint_16 Length;
	uint_16 MaxLength;
	uint_32 Buffer;
};

struct PE_STRING64
{
	uint_16 Length;
	uint_16 MaxLength;
	uint_32 Unused;
	uint_64 Buffer;
};

struct PROCESS_PARAMETERS_32
{
	uint_32     AllocationSize;
	uint_32     ActualSize;
	uint_32     Flags;
	uint_32     DebugFlags;

	uint_32     ConsoleHandle;
	uint_32     ConsoleFlags;

	uint_32     InputHandle;
	uint_32     OutputHandle;
	uint_32     ErrorHandle;

	PE_STRING32 CurrentDirectory;
	uint_32     CurrentDirectoryHandle;

	PE_STRING32 DllPath;
	PE_STRING32 ImagePathName;
	PE_STRING32 CommandLine;
};

struct PROCESS_PARAMETERS_64
{
	uint_32     AllocationSize;
	uint_32     ActualSize;
	uint_32     Flags;
	uint_32     DebugFlags;

	uint_64     ConsoleHandle;
	uint_32     ConsoleFlags;

	uint_64     InputHandle;
	uint_64     OutputHandle;
	uint_64     ErrorHandle;

	PE_STRING64 CurrentDirectory;
	uint_64      CurrentDirectoryHandle;

	PE_STRING64 DllPath;
	PE_STRING64 ImagePathName;
	PE_STRING64 CommandLine;
};

struct PEB_32
{
	uint_32  AllocationSize;
	uint_32  Mutant;
	uint_32  ProcessHinstance;
	uint_32  ListDlls;
	uint_32	 ProcessParameters;
};

struct PEB_64
{
	uint_64  AllocationSize;
	uint_64  Mutant;
	uint_64  ProcessHinstance;
	uint_64  ListDlls;
	uint_64	 ProcessParameters;
};

struct PROCESS_BASIC_INFORMATION_32
{
	uint_32  ExitStatus;
	uint_32  PebBaseAddress;
	uint_32 AffinityMask;
	uint_32 BasePriority;
	uint_32 UniqueProcessId;
	uint_32 InheritedFromUniqueProcessId;
};

struct PROCESS_BASIC_INFORMATION_64
{
	uint_64 ExitStatus;
	uint_64 PebBaseAddress;
	uint_64 AffinityMask;
	uint_64 BasePriority;
	uint_64 UniqueProcessId;
	uint_64 InheritedFromUniqueProcessId;
};

#ifdef BIT_64
typedef PEB_64 PEB_X;
typedef PROCESS_BASIC_INFORMATION_64 PROCESS_BASIC_INFORMATION_X;
typedef PROCESS_PARAMETERS_64 PROCESS_PARAMETERS_X;
#else
typedef PEB_32 PEB_X;
typedef PROCESS_BASIC_INFORMATION_32 PROCESS_BASIC_INFORMATION_X;
typedef PROCESS_PARAMETERS_32 PROCESS_PARAMETERS_X;
#endif // BIT_64

VENUS_END
