#ifdef PE_READER_EXPORTS
# define PE_READER_API __declspec(dllexport)
#else
# define PE_READER_API
#endif

#include "stdafx.h"

#include "dos_header.hh"

class PE_READER_API PEReader
{
public:
    PEReader();
    PEReader(const std::string &, uint64_t new_image_base = 0);
    PEReader(const char *, uint64_t new_image_base = 0);
    ~PEReader();

    void LoadPE(const char *path);
    void LoadPE(const std::string &);
    void SetImageBase(uint64_t new_image_base);
    PIMAGE_SECTION_HEADER GetSectionFromRVA(uint64_t rva);

    void PatchRelocations();
    void PatchImports();

private:
    void ParsePE(void);

    std::string *pe_path;
    uint8_t *pe;
    HANDLE pe_file;
    HANDLE pe_file_map;
    uint32_t pe_file_size;

    uint64_t new_image_base;

    PIMAGE_DOS_HEADER dos_header;

    PIMAGE_NT_HEADERS32 pe_header_32;
    PIMAGE_NT_HEADERS64 pe_header_64;

    std::vector<PIMAGE_SECTION_HEADER> *sections;

    bool is_32_bits_pe;
};
