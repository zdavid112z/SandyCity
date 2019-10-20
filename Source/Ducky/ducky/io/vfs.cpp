#include "ducky_pch.h"
#include "vfs.h"
#include "utils\debug.h"

namespace ducky { namespace io {

	IVirtualFileSystem::IVirtualFileSystem(IStream* stream)
	{
		m_Stream = stream;
		m_Size = stream->GetSize();
		stream->Seek(m_Size - 8);
		uint64 descLocation;
		stream->ReadUInt64(descLocation);
		stream->Seek(descLocation);

		m_Desc.descriptorStart = descLocation;
		stream->ReadUInt64(m_Desc.flags);
		stream->ReadUInt(m_Desc.numItems);
		for (uint i = 0; i < m_Desc.numItems; i++)
		{
			m_Desc.items.push_back(VFSItemInfo());
			VFSItemInfo& info = m_Desc.items[m_Desc.items.size() - 1];
			stream->ReadString(info.name);
			stream->ReadByte(info.flags);
			stream->ReadUInt64(info.size);
			stream->ReadByte(info.type); 
			uint size, ax;
			stream->ReadUInt(size);
			info.elements.reserve(size);
			for (int j = 0; j < size; j++)
			{
				stream->ReadUInt(ax);
				info.elements.push_back(ax);
			}
			//stream->ReadBytes((byte*)&m_Desc.items[m_Desc.items.size() - 1], sizeof(VFSItemInfo));
		}
	}

	VFSResult IVirtualFileSystem::GetItem(const string& path, uint index)
	{
		VFSResult res;
		res.stream = nullptr;
		res.info = nullptr;
		for (int i = 0; i < m_Desc.items.size(); i++)
		{
			if (m_Desc.items[i].name == path)
			{
				res.info = &m_Desc.items[i];
				m_Stream->Seek(res.info->elements[index]);
				res.stream = m_Stream;
				return res;
			}
		}
		return res;
	}

	IStream* IVirtualFileSystem::GetStream()
	{
		return m_Stream;
	}

	uint IVirtualFileSystem::GetNumItems() const
	{
		return m_Desc.items.size();
	}

	VFSResult IVirtualFileSystem::GetItemAt(uint i)
	{
		VFSResult r;
		r.info = &m_Desc.items[i];
		m_Stream->Seek(r.info->elements[0]);
		r.stream = m_Stream;
		return r;
	}

	uint64 IVirtualFileSystem::GetFlags() const
	{
		return m_Desc.flags;
	}

	void IVirtualFileSystem::Close()
	{
		m_Stream->Close();
	}

	OVirtualFileSystem::OVirtualFileSystem(OStream* stream)
	{
		m_Stream = stream;
		m_Desc.flags = 0;
	}

	uint64 OVirtualFileSystem::GetFlags() const
	{
		return m_Desc.flags;
	}

	void OVirtualFileSystem::SetFlags(uint64 f)
	{
		m_Desc.flags = f;
	}

	OStream* OVirtualFileSystem::StartAddItem(const string& path, uint8 type)
	{
		m_ToAdd.name = path;
		m_ToAdd.type = type;
		m_ToAdd.flags = 0;
		m_ToAdd.elements.clear();
		m_ToAdd.elements.push_back(m_Stream->Tell());
		return m_Stream;
	}

	void OVirtualFileSystem::FinishAddArrayItem()
	{
		m_ToAdd.elements.push_back(m_Stream->Tell());
	}

	void OVirtualFileSystem::FinishAddItem()
	{
		uint64 loc = m_Stream->Tell();
		if (m_ToAdd.elements[m_ToAdd.elements.size() - 1] == loc)
			m_ToAdd.elements.pop_back();
		if (m_ToAdd.elements.size() == 0)
		{
			utils::Debug::Printer << WARN << TLOG << "VFS item " << m_ToAdd.name << " is empty when finishing!" << ENDL;
			utils::Debug::Printer << WARN << TLOG << "The item was not added." << ENDL;
		}
		else
		{
			m_ToAdd.size = m_Stream->Tell() - m_ToAdd.elements[0];
			m_Desc.items.push_back(m_ToAdd);
		}
	}

	void OVirtualFileSystem::Close()
	{
		uint64 beg = m_Stream->Tell();
		m_Stream->WriteUInt64(m_Desc.flags);
		m_Stream->WriteUInt(m_Desc.items.size());
		for (uint i = 0; i < m_Desc.items.size(); i++)
		{
			//m_Stream->WriteUShort(m_Desc.items[i].nameSize);
			//m_Stream->WriteBytes((byte*)m_Desc.items[i].name, m_Desc.items[i].nameSize);
			m_Stream->WriteString(m_Desc.items[i].name);
			m_Stream->WriteByte(m_Desc.items[i].flags);
			m_Stream->WriteUInt64(m_Desc.items[i].size);
			m_Stream->WriteByte(m_Desc.items[i].type);
			m_Stream->WriteUInt(m_Desc.items[i].elements.size());
			for(uint j = 0; j < m_Desc.items[i].elements.size(); j++)
				m_Stream->WriteUInt(m_Desc.items[i].elements[j]);
		}

		m_Stream->WriteUInt64(beg);
		m_Stream->Close();
	}

} }
