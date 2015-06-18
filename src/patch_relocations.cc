#include "stdafx.h"
#include "pe_reader.hh"

void PEReader::PatchRelocations(void)
{
    PIMAGE_DATA_DIRECTORY reloc_entry;
    uint64_t image_base;

    if (is_32_bits_pe)
    {
        reloc_entry = &pe_header_32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
        image_base = pe_header_32->OptionalHeader.ImageBase;
    }

    else
    {
        reloc_entry = &pe_header_64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
        image_base = pe_header_64->OptionalHeader.ImageBase;
    }

    if (reloc_entry->Size == 0)
    {
        std::cout << "[Relocations] Nothing to be done." << std::endl;
        return;
    }

    uint64_t delta = new_image_base - image_base;
    PIMAGE_SECTION_HEADER reloc_section_hdr = GetSectionFromRVA(reloc_entry->VirtualAddress);

    if (!reloc_section_hdr)
    {
        std::cerr << "[Relocations] No relocation section found." << std::endl;
        return;
    }

    unsigned int n = 0;
    while (n < reloc_entry->Size)
    {
        unsigned int offset_to_relocations = n + reloc_section_hdr->PointerToRawData;
        PIMAGE_BASE_RELOCATION reloc = reinterpret_cast<PIMAGE_BASE_RELOCATION>(pe + offset_to_relocations);
        int num_relocations = (reloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
        PIMAGE_SECTION_HEADER section_hdr = GetSectionFromRVA(reloc->VirtualAddress);

        for (int i = 0; i < num_relocations; ++i)
        {
            unsigned int offset = offset_to_relocations + sizeof(IMAGE_BASE_RELOCATION) + i * sizeof(WORD);
            uint16_t word = *reinterpret_cast<uint16_t *>(pe + offset);
            uint32_t reloc_section_offset = word & 0xFFF;

            uint32_t *addr_to_patch = reinterpret_cast<uint32_t *>(pe + section_hdr->PointerToRawData + reloc_section_offset);
            *addr_to_patch += delta & 0xFFFFFFFF;
        }

        n += reloc->SizeOfBlock;
    }
}
