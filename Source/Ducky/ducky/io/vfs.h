#pragma once

#include "basicio.h"

namespace ducky { namespace io {

	enum VFSItemType : uint8
	{
		ITEM_UNKNOWN,
		ITEM_BYTE,
		ITEM_CHAR,
		ITEM_STRING,
		ITEM_INT16,
		ITEM_UINT16,
		ITEM_INT32,
		ITEM_UINT32,
		ITEM_INT64,
		ITEM_UINT64,
		ITEM_FLOAT,
		ITEM_DOUBLE,
		ITEM_LONG_DOUBLE,
		ITEM_IMAGE,
		ITEM_MODEL,
		ITEM_FONT_DATA
	};

	struct VFSItemInfo
	{
		string name;
		uint8 flags;
		uint64 size;
		uint8 type;
		vector<uint64> elements;
	};

	struct VFSDescriptor
	{
		uint64 flags;
		uint32 numItems;
		vector<VFSItemInfo> items;
		uint64 descriptorStart;
	};

	struct VFSResult
	{
		IStream* stream;
		VFSItemInfo* info;
	};

	class IVirtualFileSystem
	{
	public:
		IVirtualFileSystem(IStream*);
		uint64 GetFlags() const;
		VFSResult GetItem(const string& path, uint index = 0);
		uint GetNumItems() const;
		VFSResult GetItemAt(uint i);
		IStream* GetStream();
		void Close();
	private:
		VFSDescriptor m_Desc;
		IStream* m_Stream;
		uint64 m_Size;
	};
	
	class OVirtualFileSystem
	{
	public:
		OVirtualFileSystem(OStream*);
		uint64 GetFlags() const;
		void SetFlags(uint64);
		OStream* StartAddItem(const string& path, uint8 type);
		void FinishAddArrayItem();
		void FinishAddItem();
		void Close();
	private:
		VFSItemInfo m_ToAdd;
		VFSDescriptor m_Desc;
		OStream* m_Stream;
	};

} }