#include "ducky_pch.h"
#include "xml.h"
#include "../utils/stringUtils.h"
#include "../utils/debug.h"

namespace ducky { namespace io {

	string XMLFile::s_Alignment = "  ";

	XMLFile::XMLFile()
	{

	}

	XMLFile::~XMLFile()
	{

	}

	void XMLFile::Read(const string& c)
	{
		if (m_Root)
			delete m_Root;
		if (m_Prolog)
			delete m_Prolog;
		string code = RemoveComments(c);
		// Looking for a prolog
		const char* left = code.c_str();
		int i = 0;
		m_Prolog = nullptr;
		for (i = 0; i < code.size(); i++)
		{
			if (code[i] == '<')
			{
				if (code[i + 1] == '?')
				{
					m_Prolog = Process(left, &left);
					m_Prolog->m_Parent = nullptr;
				}
				else break;
			}
		}
		m_Root = Process(left, &left);
		m_Root->m_Parent = nullptr;
	}

	XMLNode* XMLFile::Process(const char* str, const char** left)
	{
		bool prolog = false;
		XMLNode* node = new XMLNode();

		while (*str == ' ' || *str == '\t' || *str == '\n') str++;
		if (*str != '<')
		{
			Debug::Printer << ERR << "XML: Expected a '<'!\n" << ENDL;
			return nullptr;
		}
		str++;
		if (*str == '?')
		{
			prolog = true;
			str++;
		}
		const char* iname = str;
		while (*str != ' ' && *str != '\t' && *str != '\n' && *str != '>' && *str != '?' && *str != '/' && *str != '\\') str++;

		node->m_Name = string(iname, str - iname);
		
		while (true)
		{
			while (*str == ' ' || *str == '\t' || *str == '\n') str++;
			if (*str == '\\' || *str == '/')
			{
				str++;
				while (*str == ' ' || *str == '\t' || *str == '\n') str++;
				if (*str != '>')
				{
					Debug::Printer << ERR << "XML: Expected a '>'!" << ENDL;
					return nullptr;
				}
				str++;
				*left = str;
				return node;
			}
			else if (*str == '?')
			{
				if (!prolog)
				{
					Debug::Printer << ERR << "XML: Unexpected token: '?'!" << ENDL;
					return nullptr;
				}
				str++;
				while (*str == ' ' || *str == '\t' || *str == '\n') str++;
				if (*str != '>')
				{
					Debug::Printer << ERR << "XML: Expected a '>'!" << ENDL;
					return nullptr;
				}
				str++;
				*left = str;
				return node;
			}
			else if (*str == '>')
			{
				str++;
				break;
			}

			XMLAttribute atr;

			while (*str == ' ' || *str == '\t' || *str == '\n') str++;
			const char* atrnamebeg = str;
			while (*str != '=') str++;
			string atrname = string(atrnamebeg, str - atrnamebeg);
			atr.name = atrname;
			str++;
			if (*str != '"')
			{
				Debug::Printer << ERR << "XML: Expected a '\"' when reading an attribute!" << ENDL;
				return nullptr;
			}
			str++;
			const char* atrvalbeg = str;
			while (*str != '\"') str++;
			string atrval = string(atrvalbeg, str - atrvalbeg);
			atr.value = atrval;
			node->m_Attributes.push_back(atr);
			str++;
		}
		while (true)
		{
			while (*str == ' ' || *str == '\t' || *str == '\n') str++;
			const char* beg = str;
			if (*beg != '<')
			{
				const char* vbeg = str;
				while (*str != '<') str++;
				XMLAttribute atrr;
				atrr.name = "value";
				atrr.value = string(vbeg, str - vbeg);
				node->AddAttribute(atrr);
				continue;
			}
			beg++;
			while (*beg == ' ' || *beg == '\t' || *beg == '\n') beg++;
			if (*beg == '\\' || *beg == '/')
			{
				beg++;
				while (*beg == ' ' || *beg == '\t' || *beg == '\n') beg++;
				const char* nbeg = beg;
				while (*beg != ' ' && *beg != '\t' && *beg != '\n' && *beg != '>' && *beg != '?' && *beg != '/' && *beg != '\\') beg++;
				string cname = string(nbeg, beg - nbeg);
				beg++;
				if (cname != node->m_Name)
				{
					Debug::Printer << ERR << "XML: Expected the tag \"" << node->m_Name.c_str() << "\" to be closing, not \"" << cname.c_str() << "\"!" << ENDL;
					return nullptr;
				}
				*left = beg;
				return node;
			}
			else
			{
				XMLNode* child = Process(str, &str);
				child->m_Parent = node;
				node->m_Children.push_back(child);
			}
		}
		return node;

	}

	string XMLFile::RemoveComments(string s)
	{
		for (int i = 0; i < (int)s.size() - 2; i++)
		{
			if (utils::StringUtils::StartsWith(s.c_str() + i, "<!--"))
			{
				uint bi = i;
				for (; i < s.size() - 2; i++)
				{
					if (utils::StringUtils::StartsWith(s.c_str() + i, "-->"))
					{
						i += 3;
						s.erase(s.begin() + bi, s.begin() + i);
						i = bi - 1;
						break;
					}
				}
			}
		}
		return s;
	}

	string XMLFile::Write()
	{
		string out;
		if (m_Prolog)
		{
			out += "<?";
			out += m_Prolog->GetName();
			for (uint i = 0; i < m_Prolog->GetNumAttributes(); i++)
			{
				//if (m_Prolog->GetAttributeAt(i)->name == "value")
				//	continue;
				out += " ";
				out += m_Prolog->GetAttributeAt(i)->name;
				out += "=\"";
				out += m_Prolog->ProvessRevAttributeValue(m_Prolog->GetAttributeAt(i)->value);
				out += "\"";
			}
			out += "?>\n";
		}
		m_Root->Write(out);
		return out;
	}

	XMLNode* XMLFile::GetProlog()
	{
		return m_Prolog;
	}

	XMLNode* XMLFile::GetRoot()
	{
		return m_Root;
	}

	XMLNode::XMLNode(const string& name, XMLNode* parent) :
		m_Name(name),
		m_Parent(parent) {}

	XMLNode::XMLNode() {}
	XMLNode::~XMLNode() {}

	string XMLNode::GetName() const
	{
		return m_Name;
	}

	XMLNode* XMLNode::GetParent()
	{
		return m_Parent;
	}

	void XMLNode::AddAttribute(XMLAttribute a)
	{
		a.value = ProcessAttributeValue(a.value);
		m_Attributes.push_back(a);
	}

	void XMLNode::Write(string& s)
	{
		s += GetAlignment();
		s += "<";
		s += m_Name;
		bool hv = false;
		for (uint i = 0; i < m_Attributes.size(); i++)
		{
			if (m_Attributes[i].name == "value")
			{
				hv = true;
				if(m_Children.size() == 0)
					continue;
			}
			s += " ";
			s += m_Attributes[i].name;
			s += "=\"";
			s += ProvessRevAttributeValue(m_Attributes[i].value);
			s += "\"";
		}
		if (hv && m_Children.size() == 0)
		{
			s += ">";
			s += GetAttribute("value")->value;
			s += "</";
			s += m_Name;
			s += ">\n";
		}
		else if(m_Children.size() != 0)
		{
			s += ">\n";
			for (uint i = 0; i < m_Children.size(); i++)
				m_Children[i]->Write(s);
			s += GetAlignment();
			s += "</";
			s += m_Name;
			s += ">\n";
		}
		else
		{
			s += "/>\n";
		}
	}

	string XMLNode::ProcessAttributeValue(string s)
	{
		for(int i = 0; i < (int)s.size() - 5; i++)
		{
			if (utils::StringUtils::StartsWith(s.c_str() + i, "&lt;"))
			{
				s.erase(s.begin() + i, s.begin() + i + 4);
				s.insert(s.begin() + i, '<');
				continue;
			}
			else if (utils::StringUtils::StartsWith(s.c_str() + i, "&gt;"))
			{
				s.erase(s.begin() + i, s.begin() + i + 4);
				s.insert(s.begin() + i, '>');
				continue;
			}
			else if (utils::StringUtils::StartsWith(s.c_str() + i, "&amp;"))
			{
				s.erase(s.begin() + i, s.begin() + i + 5);
				s.insert(s.begin() + i, '&');
				continue;
			}
			else if (utils::StringUtils::StartsWith(s.c_str() + i, "&apos;"))
			{
				s.erase(s.begin() + i, s.begin() + i + 6);
				s.insert(s.begin() + i, '\'');
				continue;
			}
			else if (utils::StringUtils::StartsWith(s.c_str() + i, "&quot;"))
			{
				s.erase(s.begin() + i, s.begin() + i + 6);
				s.insert(s.begin() + i, '\"');
				continue;
			}
		}
		return s;
	}

	string XMLNode::ProvessRevAttributeValue(string s)
	{
		for (uint i = 0; i < s.size(); i++)
		{
			if (s[i] == '<')
			{
				s.erase(s.begin() + i);
				s.insert(i, "&lt;");
			}
			else if (s[i] == '>')
			{
				s.erase(s.begin() + i);
				s.insert(i, "&gt;");
			}
			else if (s[i] == '&')
			{
				s.erase(s.begin() + i);
				s.insert(i, "&amp;");
			}
			else if (s[i] == '\'')
			{
				s.erase(s.begin() + i);
				s.insert(i, "&apos;");
			}
			else if (s[i] == '\"')
			{
				s.erase(s.begin() + i);
				s.insert(i, "&quot;");
			}
		}
		return s;
	}

	string XMLNode::GetAlignment()
	{
		string al = "";
		XMLNode* parent = m_Parent;
		while (parent != NULL)
		{
			al += XMLFile::s_Alignment;
			parent = parent->m_Parent;
		}
		return al;
	}

	void XMLNode::AddChild(XMLNode* c)
	{
		m_Children.push_back(c);
	}

	void XMLNode::RemoveAttribute(const string& name)
	{
		for (uint i = 0; i < m_Attributes.size(); i++)
		{
			if (m_Attributes[i].name == name)
			{
				m_Attributes.erase(m_Attributes.begin() + i);
				return;
			}
		}
	}

	XMLAttribute* XMLNode::GetAttribute(const string& name)
	{
		for (uint i = 0; i < m_Attributes.size(); i++)
		{
			if (m_Attributes[i].name == name)
			{
				return &m_Attributes[i];
			}
		}
		return nullptr;
	}

	uint XMLNode::GetNumAttributes() const
	{
		return m_Attributes.size();
	}

	XMLAttribute* XMLNode::GetAttributeAt(uint index)
	{
		if (index >= m_Attributes.size())
			return nullptr;
		return &m_Attributes[index];
	}

	vector<XMLNode*> XMLNode::GetChild(const string& name)
	{
		vector<XMLNode*> nodes;
		for (uint i = 0; i < m_Children.size(); i++)
		{
			if (m_Children[i]->GetName() == name)
			{
				nodes.push_back(m_Children[i]);
			}
		}
		return nodes;
	}

	uint XMLNode::GetNumChildren() const
	{
		return m_Children.size();
	}

	XMLNode* XMLNode::GetChildAt(uint index)
	{
		return m_Children[index];
	}

} }