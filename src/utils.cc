#include "stdafx.h"
#include "pe_reader.hh"

PIMAGE_SECTION_HEADER PEReader::GetSectionFromRVA(uint64_t rva)
{
    for (auto it = sections->begin(); it != sections->end(); ++it)
    {
        uint64_t section_rva = (*it)->VirtualAddress;
        if (rva >= section_rva && rva <= section_rva + (*it)->SizeOfRawData)
            return *it;
    }

    return nullptr;
}
