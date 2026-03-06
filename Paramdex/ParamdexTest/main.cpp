#include "Paramdex.h"

#define EXPECTED_MEMORY_REQUIREMENTS 108

int main()
{
	std::wstring paramFile = L"example.xml";

	Paramdex::Paramdex paramdex;
	if (!paramdex.loadFromXML(paramFile))
	{
		fprintf(stderr, "Failed to load PARAMDEF file: %ls\n", paramFile.c_str());
		return 1;
	}

	printf_s("Loaded PARAMDEF %ls (memReqs=%d):\n", paramdex.getParamType().c_str(), paramdex.getMemoryRequirements());

	if (paramdex.getMemoryRequirements() != EXPECTED_MEMORY_REQUIREMENTS)
	{
		fprintf(stderr, "Memory requirements mismatch! Expected: %d, Got: %d\n", EXPECTED_MEMORY_REQUIREMENTS, paramdex.getMemoryRequirements());
		return 1;
	}

	printf_s("Data Version: %d\n", paramdex.getDataVersion());
	printf_s("Big Endian: %d\n", paramdex.isBigEndian());
	printf_s("Unicode: %d\n", paramdex.isUnicode());
	printf_s("Format Version: %d\n", paramdex.getFormatVersion());

	for (const auto& field : paramdex.getFields())
		printf_s("\tField: %ls, Type: %ls (size=%d)\n", field.getName().c_str(), field.getType().c_str(), field.getMemoryRequirements());

	if (!paramdex.loadEnumsFromDirectory(L"Enums"))
	{
		fprintf(stderr, "Failed to load enums from directory: Enums\n");
		return 1;
	}

	for	(const auto& [enumName, e] : paramdex.getEnums())
	{
		printf_s("Enum: %s, Type: %s\n", e.getName().c_str(), e.getType().c_str());
		for (const auto& [key, value] : e.getEnumValues())
			printf_s("\t%s = %d\n", key.c_str(), value);
	}

	return 0;
}