#pragma once
#include <vector>
#include <unordered_map>
#include <tinyxml2/tinyxml2.h>

#include "Field.h"
#include "Enum.h"

namespace Paramdex
{
	class Paramdex
	{
		std::wstring m_paramType;
		int m_dataVersion;
		bool m_bBigEndian;
		bool m_bUnicode;
		int m_formatVersion;
		std::vector<Field> m_fields;
		std::unordered_map<std::wstring, Enum> m_enums;

	public:
		Paramdex() : m_dataVersion(0), m_bBigEndian(false), m_bUnicode(false), m_formatVersion(0) {}
		Paramdex(const std::wstring& paramType, int dataVersion, bool bigEndian, bool unicode, int formatVersion, const std::vector<Field>& fields)
			: m_paramType(paramType), m_dataVersion(dataVersion), m_bBigEndian(bigEndian), m_bUnicode(unicode), m_formatVersion(formatVersion), m_fields(fields) 
		{
		}

		~Paramdex() {}

		const std::wstring& getParamType() const { return m_paramType; }
		int getDataVersion() const { return m_dataVersion; }
		bool isBigEndian() const { return m_bBigEndian; }
		bool isUnicode() const { return m_bUnicode; }
		int getFormatVersion() const { return m_formatVersion; }
		const std::vector<Field>& getFields() const { return m_fields; }

		int getMemoryRequirements() const;

		bool loadFromXML(const std::wstring& filePath);
		bool saveToXML(const std::wstring& filePath) const;

		bool loadEnumsFromDirectory(const std::wstring& directoryPath);
		bool saveEnumsToDirectory(const std::wstring& directoryPath) const;
	};
}