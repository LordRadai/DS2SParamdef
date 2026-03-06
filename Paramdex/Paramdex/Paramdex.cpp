#include <filesystem>
#include <cwctype>

#include "Paramdex.h"
#include "TiXmlHelpers/TiXmlHelpers.h"

namespace
{
	std::wstring SnakeToPascalCase(const std::wstring& snake) 
	{
		std::wstring pascal;
		bool capitalizeNext = true;

		for (wchar_t c : snake) 
		{
			if (c == L'_') 
			{
				capitalizeNext = true;
			}
			else 
			{
				if (capitalizeNext) 
				{
					pascal += static_cast<wchar_t>(std::towupper(c));
					capitalizeNext = false;
				}
				else 
				{
					pascal += static_cast<wchar_t>(std::towlower(c));
				}
			}
		}

		return pascal;
	}
}

namespace Paramdex
{
	int Paramdex::getMemoryRequirements() const
	{
		int total = 0;

		for (const auto& field : m_fields)
			total += field.getMemoryRequirements();

		return total;
	}

	bool Paramdex::loadFromXML(const std::wstring& filePath)
	{
		tinyxml2::XMLDocument doc;

		if (doc.LoadFile(TiXmlHelpers::WToS(filePath).c_str()) != tinyxml2::XML_SUCCESS)
			return false;

		auto* root = doc.RootElement();
		if (!root) return false;

		m_paramType = TiXmlHelpers::SToW(root->Attribute("ParamType"));
		m_dataVersion = root->IntAttribute("DataVersion", 0);
		m_bBigEndian = root->BoolAttribute("BigEndian", false);
		m_bUnicode = root->BoolAttribute("Unicode", false);
		m_formatVersion = root->IntAttribute("FormatVersion", 0);
		m_fields.clear();

		for (auto* e = root->FirstChildElement("Field"); e; e = e->NextSiblingElement("Field"))
		{
			Field field;
			if (field.loadFromXML(e))
				m_fields.push_back(field);
		}

		return true;
	}

	bool Paramdex::saveToXML(const std::wstring& filePath) const
	{
		tinyxml2::XMLDocument doc;
		auto* decl = doc.NewDeclaration();

		doc.InsertFirstChild(decl);
		auto* root = doc.NewElement("PARAMDEF");

		root->SetAttribute("ParamType", TiXmlHelpers::WToS(m_paramType).c_str());
		root->SetAttribute("DataVersion", m_dataVersion);
		root->SetAttribute("BigEndian", m_bBigEndian);
		root->SetAttribute("Unicode", m_bUnicode);
		root->SetAttribute("FormatVersion", m_formatVersion);

		doc.InsertEndChild(root);

		for (const auto& field : m_fields)
			field.writeXML(root);

		return doc.SaveFile(TiXmlHelpers::WToS(filePath).c_str()) == tinyxml2::XML_SUCCESS;
	}

	bool Paramdex::loadEnumsFromDirectory(const std::wstring& directoryPath)
	{
		for (const auto& entry : std::filesystem::directory_iterator(directoryPath))
		{
			if (entry.is_regular_file() && entry.path().extension() == L".tdf") 
			{
				std::wstring filename = entry.path().stem().wstring();
				std::wstring enumName = SnakeToPascalCase(filename);

				Enum e;
				if (e.loadFromTdf(entry.path().wstring()))
					m_enums[enumName] = e;
			}
		}

		return true;
	}

	bool Paramdex::saveEnumsToDirectory(const std::wstring& directoryPath) const
	{
		for (const auto& [enumName, e] : m_enums)
		{
			std::wstring filename = SnakeToPascalCase(enumName) + L".tdf";
			std::filesystem::path filePath = std::filesystem::path(directoryPath) / filename;
			e.saveToTdf(filePath.wstring());
		}

		return true;
	}
}