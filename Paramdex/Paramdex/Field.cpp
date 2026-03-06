#include <regex>
#include <sstream>

#include "Field.h"
#include "TiXmlHelpers/TiXmlHelpers.h"

namespace Paramdex
{
	int Field::getMemoryRequirements() const
	{
		if (m_type == "u8" || m_type == "s8" || m_type == "dummy8")
			return 1 * std::fmax(1, m_arraySize);
		else if (m_type == "u16" || m_type == "s16")
			return 2 * std::fmax(1, m_arraySize);
		else if (m_type == "u32" || m_type == "s32" || m_type == "f32" || m_type == "angle32" || m_type == "b32")
			return 4 * std::fmax(1, m_arraySize);
		else if (m_type == "u64" || m_type == "s64" || m_type == "f64")
			return 8 * std::fmax(1, m_arraySize);
		else if (m_type == "fixstr")
			return std::fmax(1, m_arraySize);
		else if (m_type == "fixstrW")
			return 2 * std::fmax(1, m_arraySize);
		else
			return 0;
	}

    bool Field::loadFromXML(const tinyxml2::XMLElement* element) 
    {
        if (!element) return false;

        if (const char* defAttr = element->Attribute("Def")) 
        {
            std::string def(defAttr);

            // Regex handles: Type Name[:Bits] or [Array] [= Default]
            static const std::regex re(R"((\w+)\s+([^:\[\s=]+)(?::(\d+))?(?:\[(\d+)\])?(?:\s*=\s*([^]+))?)");
            std::smatch match;

            if (std::regex_search(def, match, re)) 
            {
                m_type = match[1].str().c_str();
                m_name = match[2].str().c_str();
                m_bitSize = match[3].matched ? std::stoi(match[3]) : -1;
                m_arraySize = match[4].matched ? std::stoi(match[4]) : 1;
                m_defaultValue = match[5].matched ? std::stof(match[5].str()) : 0.0f;
            }
        }

        auto GetW = [&](const char* tag) 
            {
            auto* e = element->FirstChildElement(tag);
            return (e && e->GetText()) ? TiXmlHelpers::SToW(e->GetText()) : L"";
            };

        auto GetS = [&](const char* tag) 
            {
            auto* e = element->FirstChildElement(tag);
            return (e && e->GetText()) ? e->GetText() : "";
			};

        m_displayName = GetW("DisplayName");
        m_enumName = GetS("Enum");
        m_description = GetW("Description");
        m_displayFormat = GetS("DisplayFormat");

        m_editFlags = getEditFlagFromString(GetW("EditFlags"));

        auto GetFloat = [&](const char* tag) -> float 
            {
            auto* e = element->FirstChildElement(tag);
            return (e && e->GetText()) ? std::stof(e->GetText()) : 0.0f;
            };

        m_minValue = GetFloat("Minimum");
        m_maxValue = GetFloat("Maximum");
        m_increment = GetFloat("Increment");

        if (auto* sID = element->FirstChildElement("SortID"))
            m_sortID = sID->IntText();

        return true;
    }

    bool Field::writeXML(tinyxml2::XMLElement* parent) const 
    {
        if (!parent) return false;
        auto* doc = parent->GetDocument();
        auto* e = doc->NewElement("Field");

        // 1. Build "Def" string
        std::stringstream ss;
        ss << m_type << " " << m_name;
        if (m_bitSize != -1) ss << ":" << m_bitSize;
        if (m_arraySize > 1) ss << "[" << m_arraySize << "]";

        if (m_defaultValue != 0)
            ss << " = " << m_defaultValue;

        e->SetAttribute("Def", ss.str().c_str());

        // 2. Add children
        auto AddChildW = [&](const char* tag, const std::wstring& val) 
            {
            if (!val.empty()) e->InsertNewChildElement(tag)->SetText(TiXmlHelpers::ShiftJisToS(TiXmlHelpers::WToShiftJIS(val).c_str()).c_str());
            };

        auto AddChildS = [&](const char* tag, const std::string& val) 
            {
            if (!val.empty()) e->InsertNewChildElement(tag)->SetText(val.c_str());
			};

        AddChildW("DisplayName", m_displayName);

        if (!m_enumName.empty() && (m_enumName != m_type))
            AddChildS("Enum", m_enumName);

        AddChildW("Description", m_description);
        AddChildS("DisplayFormat", m_displayFormat);

        std::string flagStr = getEditFlagString(m_editFlags);
        if (m_editFlags && !flagStr.empty()) e->InsertNewChildElement("EditFlags")->SetText(flagStr.c_str());

        if (m_minValue != 0.0f) e->InsertNewChildElement("Minimum")->SetText(m_minValue);
        if (m_maxValue != 0.0f) e->InsertNewChildElement("Maximum")->SetText(m_maxValue);
        if (m_increment != 0.0f) e->InsertNewChildElement("Increment")->SetText(m_increment);

        e->InsertNewChildElement("SortID")->SetText(m_sortID);

        parent->InsertEndChild(e);

        return true;
    }

    const char* Field::getEditFlagString(EditFlags flags) 
    {
        switch (flags)
        {
		case EDIT_OPTIONS_NONE: return "NONE";
        case EDIT_OPTIONS_WRAP: return "WRAP";
        case EDIT_OPTIONS_LOCK: return "LOCK";
        default: return "";
        }
    }

    EditFlags Field::getEditFlagFromString(const std::wstring& str) 
    {
		if (str == L"WRAP") return EDIT_OPTIONS_WRAP;
        else if (str == L"LOCK") return EDIT_OPTIONS_LOCK;
		else return EDIT_OPTIONS_NONE;
	}
}