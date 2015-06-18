#include "stdafx.h"
#include "pe_reader.hh"

void PEReader::ParsePE(void)
{
    if ((pe_file = CreateFileA(pe_path->c_str(),
        GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
    {
        std::cerr << "CreateFile failed." << std::endl;
        return;
    }

    if ((pe_file_map = CreateFileMapping(pe_file, NULL, PAGE_WRITECOPY, 0, 0, NULL)) == INVALID_HANDLE_VALUE)
    {
        std::cerr << "CreateFileMapping failed." << std::endl;
        CloseHandle(pe_file);
        return;
    }

    pe_file_size = GetFileSize(pe_file, NULL);
    pe = reinterpret_cast<uint8_t *>(MapViewOfFile(pe_file_map, FILE_MAP_COPY, 0, 0, pe_file_size));

    if (pe == NULL)
    {
        std::cout << GetLastError() << std::endl;
        CloseHandle(pe_file_map);
        CloseHandle(pe_file);
        return;
    }

    dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(pe);
    pe_header_32 = reinterpret_cast<PIMAGE_NT_HEADERS32>(pe + dos_header->e_lfanew);
    pe_header_64 = reinterpret_cast<PIMAGE_NT_HEADERS64>(pe + dos_header->e_lfanew);

    uint16_t arch = *(reinterpret_cast<uint16_t *>(pe + dos_header->e_lfanew + 0x18));

    is_32_bits_pe = arch == IMAGE_NT_OPTIONAL_HDR32_MAGIC;

    uint32_t sections_table_offset = 0;
    uint32_t number_of_sections = 0;

    if (is_32_bits_pe)
    {
        pe_header_64 = nullptr;
        sections_table_offset = pe_header_32->FileHeader.SizeOfOptionalHeader;
        number_of_sections = pe_header_32->FileHeader.NumberOfSections;
    }

    else
    {
        pe_header_32 = nullptr;
        sections_table_offset = pe_header_64->FileHeader.SizeOfOptionalHeader;
        number_of_sections = pe_header_64->FileHeader.NumberOfSections;
    }

    sections_table_offset += dos_header->e_lfanew + 0x18;
    for (unsigned int i = 0; i < number_of_sections; ++i)
    {
        sections->push_back(reinterpret_cast<PIMAGE_SECTION_HEADER>
            (pe + i * sizeof(IMAGE_SECTION_HEADER) + sections_table_offset));
    }
}
