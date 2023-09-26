/* KallistiOS ##version##

   kos/elf.h
   Copyright (C) 2000, 2001, 2003 Megan Potter
   Copyright (C) 2023 Falco Girgis

*/

/** \file   kos/elf.h
    \brief  ELF binary loading support.
    \ingroup elf

    This file contains the support functionality for loading ELF binaries in
    KOS. This includes the various header structures and whatnot that are used
    in ELF files to store code/data/relocations/etc. This isn't necessarily
    meant for running multiple processes, but more for loadable library support
    within KOS.

    \author Megan Potter
    \author Falco Girgis
*/

#ifndef __KOS_ELF_H
#define __KOS_ELF_H

#include <sys/cdefs.h>
__BEGIN_DECLS

#include <arch/types.h>
#include <sys/queue.h>

/** \defgroup elf                       ELF Format
    \brief Structures and API for working with the ELF format
*/

/** \defgroup elf_ident                 ELF Identification Bytes
    \ingroup elf

    Initial bytes of the ELF file, specifying how it should be
    interpreted.

    @{
*/
#define EI_MAG0         0   /**< \brief File identification: 0x7f */
#define EI_MAG1         1   /**< \brief File identification: 'E' */
#define EI_MAG2         2   /**< \brief File identification: 'L' */
#define EI_MAG3         3   /**< \brief File identificaiton: 'F" */
#define EI_CLASS        4   /**< \brief File class */
#define EI_DATA         5   /**< \brief Data encoding */ 
#define EI_VERSION      6   /**< \brief File version */
#define EI_OSABI        7   /**< \brief Operating System/ABI identification */
#define EI_ABIVERSION   8   /**< \brief ABI version */
#define EI_PAD          9   /**< \brief Start of padding bytes */
#define EI_NIDENT       16  /**< \brief Size of elf_hdr::ident */
/** @} */

/** \defgroup elf_types                 ELF object file types
    \ingroup elf

    Identifies the object file type.

    @{
*/
#define ET_NONE     0       /**< \brief No file type */
#define ET_REL      1       /**< \brief Relocatable file */
#define ET_EXEC     2       /**< \brief Executable file */
#define ET_DYN      3       /**< \brief Shared object file */
#define ET_CORE     4       /**< \brief Core file */
#define ET_LOOS     0xfe00  /**< \brief OS-specific */
#define ET_HIOS     0xfeff  /**< \brief OS-specific */
#define ET_LOPROC   0xff00  /**< \brief Processor-specific */
#define ET_HIPROC   0xffff  /**< \brief Processor-specific */
/** @} */

/** \defgroup elf_archs                 ELF architecture types
    \ingroup elf

    These are the various architectures that we might care about for ELF files.

    @{
*/
#define EM_386  3   /**< \brief x86 (IA32) */
#define EM_ARM  40  /**< \brief ARM */
#define EM_SH   42  /**< \brief SuperH */
/** @} */

/** \brief ELF file header.
    \ingroup elf

    This header is at the beginning of any valid ELF binary and serves to
    identify the architecture of the binary and various data about it.
*/
typedef struct elf_hdr {
    uint8 ident[EI_NIDENT]; /**< \brief ELF identifier */
    uint16 type;            /**< \brief ELF file type */
    uint16 machine;         /**< \brief ELF file architecture */
    uint32 version;         /**< \brief Object file version */
    uint32 entry;           /**< \brief Entry point */
    uint32 phoff;           /**< \brief Program header offset */
    uint32 shoff;           /**< \brief Section header offset */
    uint32 flags;           /**< \brief Processor flags */
    uint16 ehsize;          /**< \brief ELF header size in bytes */
    uint16 phentsize;       /**< \brief Program header entry size */
    uint16 phnum;           /**< \brief Program header entry count */
    uint16 shentsize;       /**< \brief Section header entry size */
    uint16 shnum;           /**< \brief Section header entry count */
    uint16 shstrndx;        /**< \brief String table section index */
} elf_hdr_t;

/** \defgroup elf_segments              Segment types
    \ingroup elf

    Identifies an ELF segment type.

    @{
*/
#define PT_NULL     0           /**< \brief Unused */
#define PT_LOAD     1           /**< \brief Loadable segment */
#define PT_DYNAMIC  2           /**< \brief Dynamic linking information */
#define PT_INTERP   3           /**< \brief Interpreter path */
#define PT_NOTE     4           /**< \brief Auxiliary information */
#define PT_SHLIB    5           /**< \brief Unspecified semantics */
#define PT_PHDR     6           /**< \brief Program header */
#define PT_LOPROC   0x70000000  /**< \brief Processor-specific start */
#define PT_HIPROC   0x7fffffff  /**< \brief Processor-specific end */
/** @} */

/** \defgroup phdr_flags                Segment flags
    \ingroup elf

    Flags for permissions given to a particular segment.

    @{
*/
#define PF_X        0x1         /**< \brief Execute */
#define PF_W        0x2         /**< \brief Write */
#define PF_R        0x4         /**< \brief Read */
#define PF_MASKOS   0x0ff00000  /**< \brief Reserved for opperating system */
#define PF_MASKPROC 0xf0000000  /**< \brief Reserved for processor */
/** @} */

/** \brief ELF program header.
    \ingroup elf
    
    Describes a segment or other information the system needs to 
    prepare a program for execution. 
*/
typedef struct elf_phdr {
    uint32 type;    /**< \brief Type of segment */
    uint32 offset;  /**< \brief File offset */
    uint32 vaddr;   /**< \brief Virtual address for segment */
    uint32 paddr;   /**< \brief Physical address for segment */
    uint32 filesz;  /**< \brief Byte size of file image */
    uint32 memsz;   /**< \brief Byte size of memory image */
    uint32 flags;   /**< \brief Flags describing segment */
    uint32 align;   /**< \brief Alignment in memory and file */
} elf_phdr_t;

/** \defgroup elf_sections              Section header types
    \ingroup elf

    These are the various types of section headers that can exist in an ELF
    file.

    @{
*/
#define SHT_NULL        0       /**< \brief Inactive section */
#define SHT_PROGBITS    1       /**< \brief Program code/data */
#define SHT_SYMTAB      2       /**< \brief Full symbol table */
#define SHT_STRTAB      3       /**< \brief String table */
#define SHT_RELA        4       /**< \brief Relocation table, with addends */
#define SHT_HASH        5       /**< \brief Symbol hash table */
#define SHT_DYNAMIC     6       /**< \brief Dynamic linking info */
#define SHT_NOTE        7       /**< \brief Notes section */
#define SHT_NOBITS      8       /**< \brief Occupies no file space */
#define SHT_REL         9       /**< \brief Relocation table, no addends */
#define SHT_SHLIB       10      /**< \brief Reserved */
#define SHT_DYNSYM      11      /**< \brief Dynamic-only sym tab */
#define SHT_LOPROC  0x70000000  /**< \brief Start of processor specific types */
#define SHT_HIPROC  0x7fffffff  /**< \brief End of processor specific types */
#define SHT_LOUSER  0x80000000  /**< \brief Start of program specific types */
#define SHT_HIUSER  0xffffffff  /**< \brief End of program specific types */
/** @} */

/** \defgroup elf_hdrflags              Section header flags
    \ingroup elf

    These are the flags that can be set on a section header. These are related
    to whether the section should reside in memory and permissions on it.

    @{
*/
#define SHF_WRITE       1           /**< \brief Writable data */
#define SHF_ALLOC       2           /**< \brief Resident */
#define SHF_EXECINSTR   4           /**< \brief Executable instructions */
#define SHF_MASKPROC    0xf0000000  /**< \brief Processor specific mask */
/** @} */

/** \defgroup elf_specsec               Special section indeces
    \ingroup elf

    These are the indices to be used in special situations in the section array.

    @{
*/
#define SHN_UNDEF   0       /**< \brief Undefined, missing, irrelevant */
#define SHN_ABS     0xfff1  /**< \brief Absolute values */
/** @} */

/** \brief  ELF Section header.
    \ingroup elf

    This structure represents the header on each ELF section.

    \note
    Link and info fields:
    switch (sh_type) {
        case SHT_DYNAMIC:
            link = section header index of the string table used by
                the entries in this section
            info = 0
        case SHT_HASH:
            ilnk = section header index of the string table to which
                this info applies
            info = 0
        case SHT_REL, SHT_RELA:
            link = section header index of associated symbol table
            info = section header index of section to which reloc applies
        case SHT_SYMTAB, SHT_DYNSYM:
            link = section header index of associated string table
            info = one greater than the symbol table index of the last
                local symbol (binding STB_LOCAL)
    }
*/
typedef struct elf_shdr {
    uint32 name;        /**< \brief Index into string table */
    uint32 type;        /**< \brief Section type \see elf_sections */
    uint32 flags;       /**< \brief Section flags \see elf_hdrflags */
    uint32 addr;        /**< \brief In-memory offset */
    uint32 offset;      /**< \brief On-disk offset */
    uint32 size;        /**< \brief Size (if SHT_NOBITS, amount of 0s needed) */
    uint32 link;        /**< \brief Section header table index link */
    uint32 info;        /**< \brief Section header extra info */
    uint32 addralign;   /**< \brief Alignment constraints */
    uint32 entsize;     /**< \brief Fixed-size table entry sizes */
} elf_shdr_t;

/** \defgroup elf_binding               Symbol binding types.
    \ingroup elf

    These are the values that can be set to say how a symbol is bound in an ELF
    binary. This is stored in the upper 4 bits of the info field in elf_sym_t.

    @{
*/
#define STB_LOCAL   0       /**< \brief Local (non-exported) symbol */
#define STB_GLOBAL  1       /**< \brief Global (exported) symbol */
#define STB_WEAK    2       /**< \brief Weak-linked symbol */
/** @} */

/** \defgroup elf_symtype               Symbol types.
    \ingroup elf

    These are the values that can be set to say what kind of symbol a given
    symbol in an ELF file is. This is stored in the lower 4 bits of the info
    field in elf_sym_t.

    @{
*/
#define STT_NOTYPE  0       /**< \brief Symbol has no type */
#define STT_OBJECT  1       /**< \brief Symbol is an object */
#define STT_FUNC    2       /**< \brief Symbol is a function */
#define STT_SECTION 3       /**< \brief Symbol is a section */
#define STT_FILE    4       /**< \brief Symbol is a file name */
/** @} */

/** \brief  Symbol table entry
    \ingroup elf

    This structure represents a single entry in a symbol table in an ELF file.
*/
typedef struct elf_sym {
    uint32 name;        /**< \brief Index into file's string table */
    uint32 value;       /**< \brief Value of the symbol */
    uint32 size;        /**< \brief Size of the symbol */
    uint8 info;         /**< \brief Symbol type and binding */
    uint8 other;        /**< \brief 0. Holds no meaning. */
    uint16 shndx;       /**< \brief Section index */
} elf_sym_t;

/** \brief  Retrieve the binding type for a symbol.
    \ingroup elf

    \param  info            The info field of an elf_sym_t.
    \return                 The binding type of the symbol.
    
    \see                    elf_binding
*/
#define ELF32_ST_BIND(info) ((info) >> 4)

/** \brief  Retrieve the symbol type for a symbol.
    \ingroup elf 

    \param  info            The info field of an elf_sym_t.
    \return                 The symbol type of the symbol.
    
    \see                    elf_symtype
*/
#define ELF32_ST_TYPE(info) ((info) & 0xf)

/** \brief  ELF Relocation entry (with explicit addend).
    \ingroup elf

    This structure represents an ELF relocation entry with an explicit addend.
    This structure is used on some architectures, whereas others use the
    elf_rel_t structure instead.
*/
typedef struct elf_rela {
    uint32 offset;      /**< \brief Offset within section */
    uint32 info;        /**< \brief Symbol and type */
    int32 addend;       /**< \brief Constant addend for the symbol */
} elf_rela_t;

/** \brief  ELF Relocation entry (without explicit addend).
    \ingroup elf

    This structure represents an ELF relocation entry without an explicit
    addend. This structure is used on some architectures, whereas others use the
    elf_rela_t structure instead.
*/
typedef struct elf_rel {
    uint32      offset;     /**< \brief Offset within section */
    uint32      info;       /**< \brief Symbol and type */
} elf_rel_t;

/** \defgroup elf_reltypes              ELF relocation types
    \ingroup elf

    These define the types of operations that can be done to calculate
    relocations within ELF files.

    @{
*/
#define R_SH_DIR32  1       /**< \brief SuperH: Rel = Symbol + Addend */
#define R_386_32    1       /**< \brief x86: Rel = Symbol + Addend */
#define R_386_PC32  2       /**< \brief x86: Rel = Symbol + Addend - Value */
/** @} */

/** \brief  Retrieve the symbol index from a relocation entry.
    \ingroup elf 

    \param  i               The info field of an elf_rel_t or elf_rela_t.
    \return                 The symbol table index from that relocation entry.
*/
#define ELF32_R_SYM(i) ((i) >> 8)

/** \brief  Retrieve the relocation type from a relocation entry.
    \ingroup elf

    \param  i               The info field of an elf_rel_t or an elf_rela_t.
    \return                 The relocation type of that relocation.
    \see                    elf_reltypes
*/
#define ELF32_R_TYPE(i) ((uint8)(i))

/** \brief Maximum size for the name of an ELF program
    \ingroup elf
*/
#define ELF_PROG_NAME_SIZE  256

struct klibrary;

/** \brief  Kernel-specific definition of a loaded ELF binary.
    \ingroup elf

    This structure represents the internal representation of a loaded ELF binary
    in KallistiOS (specifically as a dynamically loaded library).
*/
typedef struct elf_prog {
    void *data;                  /**< \brief Pointer to program in memory */
    uint32 size;                 /**< \brief Memory image size (rounded up to page size) */

    /* Library exports */
    ptr_t lib_get_name;          /**< \brief Pointer to get_name() function */
    ptr_t lib_get_version;       /**< \brief Pointer to get_version() function */
    ptr_t lib_open;              /**< \brief Pointer to library's open function */
    ptr_t lib_close;             /**< \brief Pointer to library's close function */

    char fn[ELF_PROG_NAME_SIZE]; /**< \brief Filename of library */
} elf_prog_t;

/** \brief  Load an ELF binary.
    \ingroup elf

    This function loads an ELF binary from the VFS and fills in an elf_prog_t
    for it.

    \param  fn              The filename of the binary on the VFS.
    \param  shell           Unused?
    \param  out             Storage for the binary that will be loaded.
    \return                 0 on success, <0 on failure.
*/
int elf_load(const char *fn, struct klibrary *shell, elf_prog_t *out);

/** \brief  Free a loaded ELF program.
    \inroup elf

    This function cleans up an ELF binary that was loaded with elf_load().

    \param  prog            The loaded binary to clean up.
*/
void elf_free(elf_prog_t *prog);

__END_DECLS

#endif  /* __OS_ELF_H */

