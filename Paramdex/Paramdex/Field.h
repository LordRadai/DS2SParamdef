#pragma once
#include <string>
#include <stdexcept>

#include "tinyxml2/tinyxml2.h"

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
		std::string m_type;
		int m_bitSize;
		int m_arraySize;
		std::string m_enumName;
		std::wstring m_displayName;
		std::wstring m_description;
		std::string m_displayFormat;
		EditFlags m_editFlags;
		float m_defaultValue;
		float m_minValue;
		float m_maxValue;
		float m_increment;
		int m_sortID;
		std::wstring m_bonusStringA;
		std::wstring m_bonusStringB;
		std::wstring m_bonusStringC;
	public:
		Field(const std::string& name, const std::string& type, const std::string& enumName = "", const std::wstring& displayName = L"", const std::wstring& description = L"", const std::string& displayFormat = "%d", EditFlags editFlags = EDIT_OPTIONS_WRAP, float defaultValue = 0.0f, float minValue = 0.0f, float maxValue = 0.0f, float increment = 0.0f, int sortID = 0);

		~Field() {}

		const std::wstring& getName() const { return m_name; }
		void setName(const std::wstring& name) { m_name = name; }

		const std::string& getType() const { return m_type; }
		void setType(const std::string& type) { m_type = type; }

		int getBitSize() const { return m_bitSize; }
		void setBitSize(int bitSize) { m_bitSize = bitSize; }

		int getArraySize() const { return m_arraySize; }
		void setArraySize(int arraySize) { m_arraySize = arraySize; }

		const std::string& getEnumName() const { return m_enumName; }
		void setEnumName(const std::string& enumName) { m_enumName = enumName; }

		const std::wstring& getDisplayName() const { return m_displayName; }
		void setDisplayName(const std::wstring& displayName) { m_displayName = displayName; }

		const std::wstring& getDescription() const { return m_description; }
		void setDescription(const std::wstring& description) { m_description = description; }

		const std::string& getDisplayFormat() const { return m_displayFormat; }
		void setDisplayFormat(const std::string& displayFormat) { m_displayFormat = displayFormat; }

		EditFlags getEditFlags() const { return m_editFlags; }
		void setEditFlags(EditFlags editFlags) { m_editFlags = editFlags; }

		int getSortID() const { return m_sortID; }
		void setSortID(int sortID) { m_sortID = sortID; }

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

		void setDefaultValue(float defaultValue) { m_defaultValue = defaultValue; }

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

		void setMinValue(float value) { m_minValue = value; }

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

		void setMaxValue(float value) { m_maxValue = value; }

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

		void setIncrement(float value) { m_increment = value; }

		std::wstring getBonusStringA() const { return m_bonusStringA; }
		void setBonusStringA(const std::wstring& bonusStringA) { m_bonusStringA = bonusStringA; }

		std::wstring getBonusStringB() const { return m_bonusStringB; }
		void setBonusStringB(const std::wstring& bonusStringB) { m_bonusStringB = bonusStringB; }

		std::wstring getBonusStringC() const { return m_bonusStringC; }
		void setBonusStringC(const std::wstring& bonusStringC) { m_bonusStringC = bonusStringC; }

		bool loadFromXML(const tinyxml2::XMLElement* element);
		bool writeXML(tinyxml2::XMLElement* parent) const;

		static const char* getEditFlagString(EditFlags flags);
		static EditFlags getEditFlagFromString(const std::wstring& str);
	};
}