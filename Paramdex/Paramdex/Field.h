#pragma once
#include <tinyxml2/tinyxml2.h>
#include <string>
#include <stdexcept>

namespace Paramdex
{
	enum EditFlags
	{
		EDIT_OPTIONS_NONE = 0,
		EDIT_OPTIONS_WRAP = 1,
		EDIT_OPTIONS_LOCK = 4
	};

	class Field
	{
		std::wstring m_name;
		std::wstring m_type;
		int m_bitSize;
		int m_arraySize;
		std::wstring m_enumName;
		std::wstring m_displayName;
		std::wstring m_description;
		std::wstring m_displayFormat;
		EditFlags m_editFlags;
		float m_defaultValue;
		float m_minValue;
		float m_maxValue;
		float m_increment;
		int m_sortID;
	public:
		Field() : m_name(L""), m_type(L""), m_bitSize(-1), m_arraySize(0), m_editFlags(EDIT_OPTIONS_NONE), m_defaultValue(0.0f), m_minValue(0.0f), m_maxValue(0.0f), m_increment(0.0f), m_sortID(0) {}
		Field(const std::wstring& name, const std::wstring& type, int bitSize = -1, int arraySize = 1, const std::wstring& enumName = L"", const std::wstring& displayName = L"", const std::wstring& description = L"", const std::wstring& displayFormat = L"", EditFlags editFlags = EDIT_OPTIONS_WRAP, float defaultValue = 0.0f, float minValue = 0.0f, float maxValue = 0.0f, float increment = 0.0f, int sortID = 0)
			: m_name(name), m_type(type), m_bitSize(bitSize), m_arraySize(arraySize), m_enumName(enumName), m_displayName(displayName), m_description(description), m_displayFormat(displayFormat), m_editFlags(editFlags), m_defaultValue(defaultValue), m_minValue(minValue), m_maxValue(maxValue), m_increment(increment), m_sortID(sortID) 
		{
		}

		~Field() {}

		const std::wstring& getName() const { return m_name; }
		const std::wstring& getType() const { return m_type; }
		int getBitSize() const { return m_bitSize; }
		int getArraySize() const { return m_arraySize; }
		const std::wstring& getEnumName() const { return m_enumName; }
		const std::wstring& getDisplayName() const { return m_displayName; }
		const std::wstring& getDescription() const { return m_description; }
		const std::wstring& getDisplayFormat() const { return m_displayFormat; }
		EditFlags getEditFlags() const { return m_editFlags; }
		int getSortID() const { return m_sortID; }

		int getMemoryRequirements() const;

		template<typename T>
		T getDefaultValue() const 
		{
			if constexpr (std::is_same_v<T, std::wstring>) 
			{
				wchar_t buffer[64];
				swprintf(buffer, 64, L"%g", m_defaultValue);
				return std::wstring(buffer);
			}
			else if constexpr (std::is_arithmetic_v<T>) 
			{
				return static_cast<T>(m_defaultValue);
			}
			else 
			{
				throw std::runtime_error("Unsupported type for default value");
			}
		}

		template<typename T>
		T getMinValue() const
		{
			if constexpr (std::is_same_v<T, std::wstring>)
			{
				wchar_t buffer[64];
				swprintf(buffer, 64, L"%g", m_minValue);
				return std::wstring(buffer);
			}
			else if constexpr (std::is_arithmetic_v<T>)
			{
				return static_cast<T>(m_minValue);
			}
			else
			{
				throw std::runtime_error("Unsupported type for minimum value");
			}
		}

		template<typename T>
		T getMaxValue() const
		{
			if constexpr (std::is_same_v<T, std::wstring>)
			{
				wchar_t buffer[64];
				swprintf(buffer, 64, L"%g", m_maxValue);
				return std::wstring(buffer);
			}
			else if constexpr (std::is_arithmetic_v<T>)
			{
				return static_cast<T>(m_maxValue);
			}
			else
			{
				throw std::runtime_error("Unsupported type for maximum value");
			}
		}

		template<typename T>
		T getIncrement() const
		{
			if constexpr (std::is_same_v<T, std::wstring>)
			{
				wchar_t buffer[64];
				swprintf(buffer, 64, L"%g", m_increment);
				return std::wstring(buffer);
			}
			else if constexpr (std::is_arithmetic_v<T>)
			{
				return static_cast<T>(m_increment);
			}
			else
			{
				throw std::runtime_error("Unsupported type for increment value");
			}
		}

		bool loadFromXML(const tinyxml2::XMLElement* element);
		bool writeXML(tinyxml2::XMLElement* parent) const;

		static const char* getEditFlagString(EditFlags flags);
		static EditFlags getEditFlagFromString(const std::wstring& str);
	};
}