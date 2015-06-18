#include "stdafx.h"

#include "pe_reader.hh"
#include "dos_header.hh"
#include "import_descriptor.hh"

PEReader::PEReader(const std::string &pe_path, uint64_t new_image_base)
{
    is_32_bits_pe = false;
    pe_header_32 = nullptr;
    pe_header_64 = nullptr;
    pe_file = nullptr;
    pe_file_map = nullptr;
    sections = new std::vector < PIMAGE_SECTION_HEADER > ;
    this->pe_path = new std::string(pe_path);
    this->new_image_base = new_image_base;

    ParsePE();
}

PEReader::PEReader(const char *pe_path, uint64_t new_image_base)
{
    is_32_bits_pe = false;
    pe_header_32 = nullptr;
    pe_header_64 = nullptr;
    pe_file = nullptr;
    pe_file_map = nullptr;
    sections = new std::vector < PIMAGE_SECTION_HEADER > ;
    this->pe_path = new std::string(pe_path);
    this->new_image_base = new_image_base;

    ParsePE();
}

PEReader::~PEReader()
{
    if (pe_file_map)
        CloseHandle(pe_file_map);

    if (pe_file)
        CloseHandle(pe_file);

    if (pe_path)
        delete pe_path;

    if (sections)
        delete sections;
}

void PEReader::LoadPE(const char *pe_path)
{
    *this->pe_path = pe_path;
    ParsePE();
}

void PEReader::LoadPE(const std::string &pe_path)
{
    *this->pe_path = pe_path;
    ParsePE();
}

void PEReader::SetImageBase(uint64_t new_image_base)
{
    this->new_image_base = new_image_base;
}
