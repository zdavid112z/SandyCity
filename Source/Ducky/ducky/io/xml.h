#pragma once

#include "ducky_pch.h"
#include "basicio.h"

namespace ducky { namespace io {

	class XMLFile;

	struct XMLAttribute
	{
		string name;
		string value;
	};

	class XMLNode
	{
		friend class XMLFile;
	public:
		XMLNode(const string& name, XMLNode* parent);
		~XMLNode();

		string GetName() const;
		XMLNode* GetParent();

		void AddAttribute(XMLAttribute a);
		void AddChild(XMLNode* c);

		void RemoveAttribute(const string& name);

		XMLAttribute* GetAttribute(const string& name);
		uint GetNumAttributes() const;
		XMLAttribute* GetAttributeAt(uint index);

		vector<XMLNode*> GetChild(const string& name);
		uint GetNumChildren() const;
		XMLNode* GetChildAt(uint index);
		void Write(string&);

		static string ProcessAttributeValue(string);
		static string ProvessRevAttributeValue(string);
		string GetAlignment();
	private:

		XMLNode();
	private:

		string m_Name;
		XMLNode* m_Parent;
		vector<XMLNode*> m_Children;
		vector<XMLAttribute> m_Attributes;
	};

	class XMLFile
	{
	public:
		static string s_Alignment;
	public:
		XMLFile();
		~XMLFile();

		void Read(const string& code);
		string Write();

		XMLNode* GetProlog();
		XMLNode* GetRoot();
	private:
		XMLNode* Process(const char* beg, const char** left);
		string RemoveComments(string s);
	private:
		XMLNode* m_Root = nullptr;
		XMLNode* m_Prolog = nullptr;
	};

} } 