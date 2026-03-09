#pragma once
#include <vector>
#include <unordered_map>

#include "tinyxml2/tinyxml2.h"
#include "Field.h"
#include "Enum.h"

namespace Paramdex
{
	class Paramdex
	{
		std::string m_paramType;
		int m_dataVersion;
		bool m_bBigEndian;
		bool m_bUnicode;
		int m_formatVersion;
		std::vector<Field> m_fields;
		std::unordered_map<std::wstring, Enum> m_enums;

	public:
		Paramdex() : m_dataVersion(0), m_bBigEndian(false), m_bUnicode(false), m_formatVersion(0) {}
		Paramdex(const std::string& paramType, int dataVersion, bool bigEndian, bool unicode, int formatVersion, const std::vector<Field>& fields)
			: m_paramType(paramType), m_dataVersion(dataVersion), m_bBigEndian(bigEndian), m_bUnicode(unicode), m_formatVersion(formatVersion), m_fields(fields) 
		{
		}

		~Paramdex() {}

		const std::string& getParamType() const { return m_paramType; }
		void setParamType(const std::string& paramType) { m_paramType = paramType; }

		int getDataVersion() const { return m_dataVersion; }
		void setDataVersion(int dataVersion) { m_dataVersion = dataVersion; }

		bool isBigEndian() const { return m_bBigEndian; }
		void setBigEndian(bool bigEndian) { m_bBigEndian = bigEndian; }

		bool isUnicode() const { return m_bUnicode; }
		void setUnicode(bool unicode) { m_bUnicode = unicode; }

		int getFormatVersion() const { return m_formatVersion; }
		void setFormatVersion(int formatVersion) { m_formatVersion = formatVersion; }

		const std::vector<Field>& getFields() const { return m_fields; }
		const std::unordered_map<std::wstring, Enum>& getEnums() const { return m_enums; }

		void addField(const Field& field) { m_fields.push_back(field); }
		void clearFields() { m_fields.clear(); }

		void removeField(const std::wstring& fieldName) 
		{
			m_fields.erase(std::remove_if(m_fields.begin(), m_fields.end(), [&](const Field& f) { return f.getName() == fieldName; }), m_fields.end());
		}

		void addEnum(const std::wstring& name, const Enum& e) { m_enums.emplace(name, e); }
		void removeEnum(const std::wstring& enumName) { m_enums.erase(enumName); }
		void clearEnums() { m_enums.clear(); }

		Enum* getEnum(const std::wstring& name) 
		{
			auto it = m_enums.find(name);
			return (it != m_enums.end()) ? &it->second : nullptr;
		}

		int getMemoryRequirements() const;

		bool loadFromXML(const std::wstring& filePath);
		bool saveToXML(const std::wstring& filePath) const;

		bool loadEnumsFromDirectory(const std::wstring& directoryPath);
		bool saveEnumsToDirectory(const std::wstring& directoryPath) const;
	};
}