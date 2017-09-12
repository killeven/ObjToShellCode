#include "engine.h"
#include <fstream>

// 使用delphi生成
const int kLoaderSize = 372;
const unsigned char kLoaderBuf[] = {
  0x60, 0xE8, 0x92, 0x00, 0x00, 0x00, 0x89, 0x44, 0x24, 0x1C, 0x61, 0xFF, 0xE0, 0xC3, 0x8B, 0xC0,
  0xE8, 0x00, 0x00, 0x00, 0x00, 0x59, 0x81, 0xE9, 0xE1, 0xE6, 0x65, 0x00, 0x01, 0xC8, 0xC3, 0x90,
  0x31, 0xC0, 0x64, 0x8B, 0x40, 0x30, 0x8B, 0x40, 0x0C, 0x8B, 0x40, 0x14, 0x8B, 0x00, 0x8B, 0x00,
  0x8B, 0x40, 0x10, 0xC3, 0x60, 0x8B, 0x6C, 0x24, 0x18, 0x8B, 0x45, 0x3C, 0x8B, 0x44, 0x28, 0x78,
  0x8D, 0x74, 0x28, 0x18, 0xAD, 0x91, 0xAD, 0x50, 0xAD, 0x92, 0xAD, 0x01, 0xE8, 0x93, 0x8D, 0x34,
  0x2A, 0xAD, 0x01, 0xE8, 0x31, 0xFF, 0x97, 0x31, 0xD2, 0xC1, 0xC2, 0x03, 0x32, 0x17, 0x47, 0x80,
  0x3F, 0x00, 0x75, 0xF5, 0x39, 0x54, 0x24, 0x20, 0x75, 0x23, 0x8B, 0x54, 0x24, 0x18, 0x09, 0xD2,
  0x74, 0x0B, 0x8B, 0x46, 0xFC, 0x01, 0xE8, 0x50, 0x55, 0xFF, 0xD2, 0xEB, 0x14, 0x0F, 0xB7, 0x03,
  0xC1, 0xE0, 0x02, 0x03, 0x04, 0x24, 0x8B, 0x04, 0x28, 0x01, 0xE8, 0xEB, 0x04, 0x43, 0x43, 0xE2,
  0xC0, 0x59, 0x89, 0x44, 0x24, 0x1C, 0x61, 0xC3, 0x53, 0x56, 0x57, 0x55, 0x83, 0xC4, 0xF4, 0xB8,
  0x40, 0xE8, 0x65, 0x00, 0xE8, 0x67, 0xFF, 0xFF, 0xFF, 0x8B, 0xE8, 0x8B, 0xC5, 0x83, 0x38, 0x00,
  0x74, 0x07, 0x8B, 0x00, 0xE9, 0xB2, 0x00, 0x00, 0x00, 0xE8, 0x62, 0xFF, 0xFF, 0xFF, 0x8B, 0xD8,
  0x8B, 0xCB, 0x33, 0xD2, 0xB8, 0x84, 0x9B, 0x50, 0xF2, 0xE8, 0x66, 0xFF, 0xFF, 0xFF, 0x8B, 0xF0,
  0x89, 0x74, 0x24, 0x08, 0x8B, 0xCB, 0x8B, 0xD6, 0xB8, 0x89, 0xFD, 0x12, 0xA4, 0xE8, 0x52, 0xFF,
  0xFF, 0xFF, 0x89, 0x44, 0x24, 0x04, 0x8B, 0xCB, 0x8B, 0xD6, 0xB8, 0xAE, 0xD0, 0x16, 0xAB, 0xE8,
  0x40, 0xFF, 0xFF, 0xFF, 0x89, 0xC7, 0x8B, 0xC5, 0x8B, 0xD0, 0x83, 0xC2, 0x04, 0x8B, 0x1A, 0x03,
  0xD8, 0x81, 0x3B, 0x1A, 0x2B, 0x3C, 0x4D, 0x74, 0x3E, 0x53, 0xFF, 0x54, 0x24, 0x08, 0x89, 0x04,
  0x24, 0xEB, 0x01, 0x43, 0x80, 0x3B, 0x00, 0x75, 0xFA, 0x43, 0xEB, 0x1C, 0x8B, 0xC7, 0x8B, 0x0C,
  0x24, 0x8B, 0x54, 0x24, 0x08, 0xE8, 0x0A, 0xFF, 0xFF, 0xFF, 0x8B, 0xF0, 0x83, 0xC3, 0x04, 0x8B,
  0x03, 0x03, 0xC5, 0x89, 0x30, 0x83, 0xC3, 0x04, 0x8B, 0x3B, 0x81, 0xFF, 0x4D, 0x3C, 0x2B, 0x1A,
  0x75, 0xDA, 0x83, 0xC3, 0x04, 0xEB, 0xBA, 0x83, 0xC3, 0x04, 0x8B, 0xC3, 0xEB, 0x0E, 0x8B, 0x58,
  0x04, 0x8B, 0xD5, 0x03, 0xDA, 0x03, 0xCA, 0x89, 0x19, 0x83, 0xC0, 0x08, 0x8B, 0x08, 0x83, 0xF9,
  0xFF, 0x75, 0xEB, 0x8B, 0x40, 0x04, 0x03, 0xC5, 0x89, 0x45, 0x00, 0x83, 0xC4, 0x0C, 0x5D, 0x5F,
  0x5E, 0x5B, 0xC3, 0x90 };

ShellCodeEngine::ObjectFileInfo::ObjectFileInfo(string& path): machine_(UNKNOW) {
  name_ = extract_filename(path);
  loader_ = new FileLoader(path);
  if (loaded()) 
    processed_ = process();
}

ShellCodeEngine::ObjectFileInfo::~ObjectFileInfo() {
  SAFE_RELEASE(loader_);
}

int ShellCodeEngine::ObjectFileInfo::process() {
  ASSERT(loaded());
  if (size() < IMAGE_SIZEOF_FILE_HEADER) return 0;
  auto file_header = (IMAGE_FILE_HEADER*)(loader_->base());
  switch (file_header->Machine) {
  case IMAGE_FILE_MACHINE_I386:
    machine_ = X86;
    break;
  case IMAGE_FILE_MACHINE_AMD64:
    machine_ = X64;
    break;
  default:
    DBGOUT("invaild file format, prase error!");
    return 0;
  }
  auto section_offset = IMAGE_SIZEOF_FILE_HEADER;
  auto section_count = file_header->NumberOfSections;
  if (file_header->SizeOfOptionalHeader != 0) {
    DBGOUT("pass optional header.");
    section_offset += file_header->SizeOfOptionalHeader;
  }
  auto symbol_offset = file_header->PointerToSymbolTable;
  int symbol_count = file_header->NumberOfSymbols;
  auto string_table_offset = symbol_offset + symbol_count * IMAGE_SIZEOF_SYMBOL;
  uint32_t string_table_size = 0;
  if (string_table_offset != 0) {
    string_table_size = *cast<uint32_t*>(string_table_offset);
    if (string_table_size == 0) string_table_size = 4;
    if (string_table_size < 4) {
      DBGOUT("invaild file format!");
      return 0;
    }
    string_table_offset += 4;
    string_table_size -= 4;
  }
  auto sections = cast<IMAGE_SECTION_HEADER*>(section_offset);
  auto symbols = cast<IMAGE_SYMBOL*>(symbol_offset);
  char* string_table = nullptr;
  if (string_table_size != 0) {
    string_table = cast<char*>(string_table_offset);
  }
  unsigned long name_offset = 0;
  // enum symbol
  for (auto i = 0; i < symbol_count; i++) {
    Symbol symbol = { 0, 0, 0, "", 0, 0, 0, 0, 0 };
    if (symbols[i].N.Name.Short == 0) {
      name_offset = symbols[i].N.Name.Long;
      if (name_offset < 4) {
        DBGOUT("invaild file format!");
        return 0;
      }
      name_offset -= 4;
      if (string_table == nullptr || name_offset > string_table_size) {
        DBGOUT("invaild file format!");
        return 0;
      }
      symbol.name.assign(string_table + name_offset);
    } else {
      if (symbols[i].N.ShortName[7] != 0) {
        symbol.name.assign(reinterpret_cast<char*>(symbols[i].N.ShortName), 8);
      } else {
        symbol.name.assign(reinterpret_cast<char*>(symbols[i].N.ShortName));
      }
    }
    symbol.value = symbols[i].Value;
    symbol.section_number = symbols[i].SectionNumber;
    symbol.type = symbols[i].Type;
    symbol.storage_class = symbols[i].StorageClass;
    symbol_list_.push_back(symbol);
    if (symbols[i].NumberOfAuxSymbols > 0) {
      Symbol temp = { 1, 0, 0, "auxsymbol", 0, 0, 0, 0, 0 };;
      for (auto j = 0; j < symbols[i].NumberOfAuxSymbols; j++) {
        symbol_list_.push_back(temp);
      }
    }
    i += symbols[i].NumberOfAuxSymbols;
  }
  // enum section
  for (auto i = 0; i < section_count; i++) {
    Section section;
    memset(&section, 0, sizeof(section));
    section.discard = 0;
    if (sections[i].Name[0] == '/') {
      char* endc;
      name_offset = strtoul(reinterpret_cast<char*>(&sections[i].Name[1]), &endc, 10);
      if (*endc != 0) {
        DBGOUT("invaild file format!");
        return 0;
      }
      if (name_offset < 4) {
        DBGOUT("invaild file format!");
        return 0;
      }
      name_offset -= 4;
      if (string_table == nullptr || name_offset > string_table_size) {
        DBGOUT("invaild file format!");
        return 0;
      }
      section.name.assign(string_table + name_offset);
    }
    else {
      if (sections[i].Name[7] != 0) {
        section.name.assign(reinterpret_cast<char*>(sections[i].Name), 8);
      }
      else {
        section.name.assign(reinterpret_cast<char*>(sections[i].Name));
      }
    }
    if ((section.name.find(".debug", 0) == string::npos) && ((sections[i].Characteristics & IMAGE_SCN_LNK_REMOVE) == 0)) {
      // get align
      switch (sections[i].Characteristics & IMAGE_SCN_ALIGN_MASK) {
      case IMAGE_SCN_ALIGN_1BYTES:
        section.align = Section::BYTES_1;
        break;
      case IMAGE_SCN_ALIGN_2BYTES:
        section.align = Section::BYTES_2;
        break;
      case IMAGE_SCN_ALIGN_4BYTES:
        section.align = Section::BYTES_4;
        break;
      case IMAGE_SCN_ALIGN_8BYTES:
        section.align = Section::BYTES_8;
        break;
      case IMAGE_SCN_ALIGN_16BYTES:
        section.align = Section::BYTES_16;
        break;
      case IMAGE_SCN_ALIGN_32BYTES:
        section.align = Section::BYTES_32;
        break;
      case IMAGE_SCN_ALIGN_64BYTES:
        section.align = Section::BYTES_64;
        break;
      case IMAGE_SCN_ALIGN_128BYTES:
        section.align = Section::BYTES_128;
        break;
      case IMAGE_SCN_ALIGN_256BYTES:
        section.align = Section::BYTES_256;
        break;
      case IMAGE_SCN_ALIGN_512BYTES:
        section.align = Section::BYTES_512;
        break;
      case IMAGE_SCN_ALIGN_1024BYTES:
        section.align = Section::BYTES_1024;
        break;
      case IMAGE_SCN_ALIGN_2048BYTES:
        section.align = Section::BYTES_2048;
        break;
      case IMAGE_SCN_ALIGN_4096BYTES:
        section.align = Section::BYTES_4096;
        break;
      case IMAGE_SCN_ALIGN_8192BYTES:
        section.align = Section::BYTES_8192;
        break;
      default:
        if ((sections[i].Characteristics & IMAGE_SCN_TYPE_NO_PAD) > 0) {
          section.align = Section::BYTES_1;
        } else {
          DBGOUT("unknow section align");
          return 0;
        }
      }
      if ((sections[i].Characteristics & IMAGE_SCN_CNT_INITIALIZED_DATA) > 0) {
        section.initalize_data = 1;
      }
      if ((sections[i].Characteristics & IMAGE_SCN_CNT_UNINITIALIZED_DATA) > 0) {
        section.uninitalize_data = 1;
      }
      if ((sections[i].Characteristics & IMAGE_SCN_MEM_READ) > 0) {
        section.readable = 1;
      }
      if ((sections[i].Characteristics & IMAGE_SCN_MEM_WRITE) > 0) {
        section.writeable = 1;
      }
      if ((sections[i].Characteristics & IMAGE_SCN_CNT_CODE) > 0) {
        section.code_contain = 1;
      }
      if ((sections[i].Characteristics & IMAGE_SCN_MEM_EXECUTE) > 0) {
        section.executeable = 1;
      }
      if ((sections[i].Characteristics & IMAGE_SCN_LNK_COMDAT) > 0) {
        section.comdat = 1;
      }
      section.rva = sections[i].VirtualAddress;
      section.virtual_size = sections[i].Misc.VirtualSize;
      section.rawdata = nullptr;
      // .bss会有大小但是没有具体数据指向
      if (sections[i].PointerToRawData != 0) {
        section.rawdata = cast<char*>(sections[i].PointerToRawData);
      }
      section.raw_size = sections[i].SizeOfRawData;
      // enum relocs
      if (sections[i].NumberOfRelocations != 0) {
        auto relocs = cast<IMAGE_RELOCATION*>(sections[i].PointerToRelocations);
        for (auto j = 0; j < sections[i].NumberOfRelocations; j++) {
          Reloc reloc;
          reloc.rva = relocs[j].VirtualAddress;
          reloc.symbol = nullptr;
          reloc.symbol = &symbol_list_.at(relocs[j].SymbolTableIndex);
          reloc.type = relocs[j].Type;
          section.relocs().push_back(reloc);
        }
      }
    } else {
      DBGOUT("found debug section");
      section.discard = 1;
    }
    section_list_.push_back(section);
  }
  return 1;
}

ShellCodeEngine::DLLPares::DLLPares(string & path) {
  parsed_ = loadfile(path);
}

int ShellCodeEngine::DLLPares::find_symbol(string & name) {
  for (auto it = export_.cbegin(); it != export_.cend(); ++it) {
    if (*it == name) return static_cast<int>(it - export_.cbegin());
  }
  return -1;
}

int ShellCodeEngine::DLLPares::loadfile(string& path) {
  // extract filename
  name_ = extract_filename(path);
  // extract export
  auto module = LoadLibraryExA(path.c_str(), nullptr, DONT_RESOLVE_DLL_REFERENCES);
  if (module == nullptr) return 0;
  auto base = reinterpret_cast<char*>(module);
  auto dos_header = reinterpret_cast<IMAGE_DOS_HEADER*>(base);
  auto nt_header = reinterpret_cast<IMAGE_NT_HEADERS*>(base + dos_header->e_lfanew);
  auto export_header = reinterpret_cast<IMAGE_EXPORT_DIRECTORY*>(base + nt_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
  auto name_addr = reinterpret_cast<ULONG*>(base + export_header->AddressOfNames);
  for (uint32_t i = 0; i < export_header->NumberOfNames; i++) {
    export_.push_back(base + name_addr[i]);
  }
  return 1;
}

ShellCodeEngine::TemplateBuffer::TemplateBuffer(size_t capcity): total_size_(capcity), current_size_(0), pos_(0) {
  buffer_base_ = new char[capcity];
  memset(buffer_base_, 0, capcity);
}

ShellCodeEngine::TemplateBuffer::~TemplateBuffer() {
  if (buffer_base_ != nullptr) delete[] buffer_base_;
}

int ShellCodeEngine::TemplateBuffer::write(std::string& str) {
  write(str.c_str(), str.length());
  write<byte>(0);
  return str.length() + 1;
}

int ShellCodeEngine::TemplateBuffer::write(const char* buffer, size_t size) {
  check(size);
  if (buffer == nullptr) {
    memset(buffer_base_ + pos_, 0, size);
  }
  else {
    memcpy(buffer_base_ + pos_, buffer, size);
  }
  pos_ += size;
  if (pos_ > current_size_) {
    current_size_ = pos_;
  }
  return size;
}

// 每次动态增加1kb
void ShellCodeEngine::TemplateBuffer::check(size_t size) {
  auto new_size = total_size_;
  if (current_size_ + size > total_size_) {
    if (size / 1024 > 0) {
      new_size += (size / 1024) * 1024;
    }
    new_size += 1024;
    auto temp = new char[new_size];
    memset(temp, 0, new_size);
    memcpy(temp, buffer_base_, current_size_);
    delete[] buffer_base_;
    buffer_base_ = temp;
    total_size_ = new_size;
  }
}

ShellCodeEngine::Builder::Builder() {
}

ShellCodeEngine::Builder::~Builder() {
  for (auto it = object_list_.cbegin(); it != object_list_.cend(); ++it) {
    delete *it;
  }
  object_list_.clear();
  for (auto it = module_list_.cbegin(); it != module_list_.cend(); ++it) {
    delete *it;
  }
  module_list_.clear();
}

int ShellCodeEngine::Builder::add_module(string & path) {
  auto filename = extract_filename(path);
  for (auto it = module_list_.cbegin(); it != module_list_.cend(); ++it) {
    if (filename == (*it)->name()) return 0;
  }
  auto parse = new DLLPares(path);
  if (!parse->parsed()) {
    delete parse;
    return 0;
  }
  module_list_.push_back(parse);
  return 1;
}

int ShellCodeEngine::Builder::add_object_file(string& path) {
  auto filename = extract_filename(path);
  for (auto it = object_list_.cbegin(); it != object_list_.cend(); ++it) {
    if (filename == (*it)->name()) return 0;
  }
  auto info = new ObjectFileInfo(path);
  if (!info->processed()) {
    delete info;
    return 0;
  }
  object_list_.push_back(info);
  return 1;
}

void ShellCodeEngine::Builder::get_function_list(vector<string>& o) {
  for (auto it = object_list_.cbegin(); it != object_list_.cend(); ++it) {
    for (auto j : (*it)->symbol_list()) {
      if (j.isaux) continue;
      // 微软文档中是这么说的，暂时先这么来做
      // A number representing type.
      // Microsoft tools set this field to 0x20
      // (function)or 0x0 (not a function).
      // See Section 5.4.3, “Type
      // Representation, ” for more
      // information.
      if ((j.storage_class == IMAGE_SYM_CLASS_EXTERNAL) && (j.type == 0x0020) && (j.section_number > 0)) {
        o.push_back(j.name);
      }
    }
  }
}


int ShellCodeEngine::Builder::build(string& path, string& entry) {
  // 处理symbol ref
  reset_symbol_ref();
  shellcode_.clear();
  extract_data_.clear();
  reloc_data_.clear();
  if (!preprocess(entry)) return 0;
  vector<DataBuffer> datas;
  vector<FunctionBuffer> functions;
  // 写入标志
  shellcode_.write<uint32_t>(0x00000000);
  // 写入数据大小
  shellcode_.write<uint32_t>(0x00000000);
  // 分类数据object_list_
  for (auto obj = object_list_.begin(); obj != object_list_.end(); ++obj) {
    for (auto sym = (*obj)->symbol_list().begin(); sym != (*obj)->symbol_list().end(); ++sym) {
      if (sym->ref == 0) continue;
      if ((sym->section_number != 0) && (sym->type != 0x0020)) {
        // data 暂时先不处理align
        DataBuffer data;
        data.name = sym->name;
        data.section = &(*obj)->section_list()[sym->section_number - 1];
        data.offset = 0;
        datas.push_back(data);
      }
      else if ((sym->section_number != 0) && (sym->type == 0x0020)) {
        // function    
        FunctionBuffer function;
        function.isimport = 0;
        function.name = sym->name;
        function.section = &(*obj)->section_list()[sym->section_number - 1];
        function.dllname = "";
        function.offset = 0;
        functions.push_back(function);
      } else {
        // external symbol
        if (sym->isdll) {
          FunctionBuffer function;
          function.isimport = 1;
          function.name = sym->name;
          function.section = nullptr;
          auto realname = decode_fun(sym->name);
          for (auto dll = module_list_.begin(); dll != module_list_.end(); ++dll) {
            if ((*dll)->find_symbol(realname) != -1) function.dllname = (*dll)->name();
          }
          function.offset = 0;
          functions.push_back(function);
        }
      }
    }
  }
  for (unsigned int i = 0; i < datas.size(); i++) {
    DBGOUT("data: %s, offset: 0x%X, size: 0x%X\r\n", datas[i].name.c_str(), shellcode_.size(), datas[i].section->raw_size);
    datas[i].offset = shellcode_.size();
    if (datas[i].section->uninitalize_data) {
      shellcode_.write(nullptr, datas[i].section->raw_size);
    } else {
      shellcode_.write(datas[i].section->rawdata, datas[i].section->raw_size);
    }
  }
  shellcode_.write(0xCCCCCCCC);
  for (unsigned int i = 0; i < functions.size(); i++) {
    if (!functions[i].isimport) {
      DBGOUT("function: %s, offset: 0x%X, size: 0x%X\r\n", functions[i].name.c_str(), shellcode_.size(), functions[i].section->raw_size);
      functions[i].offset = shellcode_.size();
      shellcode_.write(functions[i].section->rawdata, functions[i].section->raw_size);
    }
  }
  // process reloc
  vector<FunctionReloc> relocs;
  for (unsigned int i = 0; i < functions.size(); i++) {
    if (!functions[i].isimport) {
      for (auto j = functions[i].section->relocs().begin(); j != functions[i].section->relocs().end(); ++j) {
        FunctionReloc reloc;
        switch (j->type) {
        case IMAGE_REL_I386_DIR32:
          reloc.type = RelocType_DIR32;
          break;
        case IMAGE_REL_I386_REL32:
          reloc.type = RelocType_REL32;
          break;
        default:
          DBGOUT("found unknow reloc type");
          return 0;
        }
        if (j->symbol->type == 0x0020) {
          reloc.isdata = 0;
          reloc.data = nullptr;
          // find target func
          reloc.func = nullptr;
          for (auto k = functions.begin(); k != functions.end(); ++k) {
            if (k->name == j->symbol->name) {
              reloc.func = &(*k);
              break;
            }
          }
          if (reloc.func == nullptr) {
            DBGOUT("can not find target function %s.\r\n", j->symbol->name.c_str());
            return 0;
          }
          reloc.offset = j->rva + static_cast<uint32_t>(functions[i].offset);
          reloc.processed = 0;
        } else {
          reloc.isdata = 1;
          reloc.data = nullptr;
          for (auto k = datas.begin(); k != datas.end(); ++k) {
            if (k->name == j->symbol->name) {
              reloc.data = &(*k);
              break;
            }
          }
          if (reloc.data == nullptr) {
            DBGOUT("can not find target data %s.\r\n", j->symbol->name.c_str());
            return 0;
          }
          reloc.func = nullptr;
          reloc.offset = j->rva + static_cast<uint32_t>(functions[i].offset);
          reloc.processed = 0;
        }
        relocs.push_back(reloc);
      }
    }
  }
  // 先整理dll function offset
  vector<ImportReloc> dlllist;
  for (auto i = relocs.begin(); i != relocs.end(); ++i) {
    if (i->processed || i->isdata) continue;
    if (i->func->isimport) {
      auto funcname = decode_fun(i->func->name);
      auto dllname = i->func->dllname;
      ImportReloc* import_reloc = nullptr;
      for (auto j = dlllist.begin(); j != dlllist.end(); ++j) {
        if ((j->dllname == dllname)) {
          import_reloc = &(*j);
        }
      }
      if (import_reloc == nullptr) {
        ImportReloc temp;
        temp.dllname = dllname;
        dlllist.push_back(temp);
        import_reloc = &dlllist[dlllist.size() - 1];
      }
      auto added = 0;
      for (auto j = import_reloc->imports.begin(); j != import_reloc->imports.end(); ++j) {
        if (j->funcname == funcname) {
          j->relocs.push_back(&(*i));
          added = 1;
        }
      }
      if (!added) {
        Import import;
        import.funcname = funcname;
        import.relocs.push_back(&(*i));
        import_reloc->imports.push_back(import);
      }
      i->processed = 1;
    }
  }
  // 生成dll function offset
  for (auto i = dlllist.begin(); i != dlllist.end(); ++i) {
    extract_data_.write(i->dllname);
    for (auto j = i->imports.begin(); j != i->imports.end(); ++j) {
      // build function
      shellcode_.seek(-1);
      j->offset = shellcode_.size();
      shellcode_.write<uint32_t>(0x00000000);
      shellcode_.write<uint16_t>(0x25FF);
      shellcode_.write<uint32_t>(0x00000000);  // api addr
      // write to function table
      extract_data_.write<uint32_t>(HashStringFromRol3Xor(j->funcname.c_str()));
      extract_data_.write<uint32_t>(j->offset);
      // write to reloc
      reloc_data_.write<uint32_t>(j->offset + 6);
      reloc_data_.write<uint32_t>(j->offset);
    }
    extract_data_.write<uint32_t>(0x1A2B3C4D);
  }
  extract_data_.write<uint32_t>(0x4D3C2B1A);
  uint32_t target, current;
  for (auto i = dlllist.begin(); i != dlllist.end(); ++i) {
    for (auto j = i->imports.begin(); j != i->imports.end(); ++j) {
      for (auto k = j->relocs.begin(); k != j->relocs.end(); ++k) {
        switch ((*k)->type) {
        case RelocType_REL32:
          // (call _api) fix to => (call addr; addr:jmp [0x00000000])
          // gen jmp [0x00000000]
          current = (*k)->offset;
          // fix
          target = j->offset + 4;
          shellcode_.seek(current);
          shellcode_.write<uint32_t>(target - current - 4);
          break;
        case RelocType_DIR32:
          // (call [addr]; addr:function addr)
          current = (*k)->offset;
          target = j->offset;
          reloc_data_.write<uint32_t>(current);
          reloc_data_.write<uint32_t>(target);
          break;
        default:
          return 0;
        }
      }
    }
  }
  // 整理reloc
  for (auto i = relocs.begin(); i != relocs.end(); ++i) {
    if (i->processed) continue;
    if (i->isdata) {
      // process data reloc
      switch (i->type) {
      case RelocType_DIR32:
        reloc_data_.write<uint32_t>(i->offset);
        reloc_data_.write<uint32_t>(i->data->offset);
        i->processed = 1;
        break;
      default:
        return 0;
      }
    }
    else {
      // process func reloc
      if (i->func->isimport) continue;
      switch (i->type) {
      case RelocType_REL32:
        shellcode_.seek(i->offset);
        shellcode_.write<uint32_t>(i->func->offset - i->offset - 4);
        i->processed = 1;
        break;
      case RelocType_DIR32:
        reloc_data_.write<uint32_t>(i->offset);
        reloc_data_.write<uint32_t>(i->func->offset);
        i->processed = 1;
        break;
      default:
        return 0;
      }
    }
  }
  for (auto i = functions.begin(); i != functions.end(); ++i) {
    if (i->name == entry) {
      reloc_data_.write<uint32_t>(0xFFFFFFFF);
      reloc_data_.write<uint32_t>(i->offset);
      break;
    }
  }
  std::ofstream fw;
  fw.open(path, std::ios::binary);
  fw.width(0);
  fw.write((const char*)kLoaderBuf, kLoaderSize);
  shellcode_.seek(4);
  shellcode_.write(shellcode_.size());
  fw.write(shellcode_.base(), shellcode_.size());
  DBGOUT("extdata offset: 0x%X, size: 0x%X\r\n", shellcode_.size(), extract_data_.size());
  fw.write(extract_data_.base(), extract_data_.size());
  DBGOUT("relocdata offset: 0x%X, size: 0x%X\r\n", shellcode_.size() + extract_data_.size(), reloc_data_.size());
  fw.write(reloc_data_.base(), reloc_data_.size());
  fw.close();

  return 1;
}

void ShellCodeEngine::Builder::reset_symbol_ref() {
  for (auto it = object_list_.cbegin(); it != object_list_.cend(); ++it) {
    for (auto t = (*it)->symbol_list().begin(); t != (*it)->symbol_list().end(); ++t) t->reset_ref();
  }
}

int ShellCodeEngine::Builder::preprocess(string & entry) {
  for (auto obj = object_list_.begin(); obj != object_list_.end(); ++obj) {
    for (auto sym = (*obj)->symbol_list().begin(); sym != (*obj)->symbol_list().end(); ++sym) {
      if (sym->isaux) continue;
      // pass auxsymbol or not a function or extrenal symbol
      if (sym->isaux || sym->storage_class != IMAGE_SYM_CLASS_EXTERNAL || sym->type != 0x0020 || sym->section_number == 0) continue;
      if (sym->name == entry) {
        return process_symbol(*sym, **obj);
      }
    }
  }
  return 0;
}

int ShellCodeEngine::Builder::process_symbol(Symbol& symbol, ObjectFileInfo& objinfo) {
  symbol.addref();
  switch (symbol.section_number) {
  case IMAGE_SYM_UNDEFINED:
    return process_external_symbol(symbol);
  case IMAGE_SYM_ABSOLUTE:
    DBGOUT("found absolute symbol, can''t prase now\r\n");
    return 0;
  default:
    break;
  }
  if (symbol.section_number > IMAGE_SYM_SECTION_MAX) {
    DBGOUT("found unknow symbol\r\n");
    return 0;
  }
  auto section = objinfo.section_list()[symbol.section_number - 1];
  for (auto reloc = section.relocs().begin(); reloc != section.relocs().end(); ++reloc) {
    if (reloc->symbol->ref != 0) continue;
    if (!process_symbol(*(reloc->symbol), objinfo)) return 0;
  }
  return 1;
}

int ShellCodeEngine::Builder::process_external_symbol(Symbol& symbol) {
  for (auto obj = object_list_.begin(); obj != object_list_.end(); ++obj) {
    for (auto sym = (*obj)->symbol_list().begin(); sym != (*obj)->symbol_list().end(); ++sym) {
      if (sym->isaux) continue;
      // pass auxsymbol or extrenal symbol define
      if (sym->isaux || (sym->storage_class != IMAGE_SYM_CLASS_EXTERNAL) || (sym->section_number == 0)) continue;
      if (sym->name == symbol.name) {
        return process_symbol(*sym, **obj);
      }
    }
  }
  DBGOUT("cant find external symbol: %s continue search in dll\r\n", symbol.name.c_str());
  auto realname = decode_fun(symbol.name);
  for (auto dll = module_list_.begin(); dll != module_list_.end(); ++dll) {
    auto index = (*dll)->find_symbol(realname);
    if (index != -1) {
      DBGOUT("found dll import: %s, function: %s.\r\n", (*dll)->name().c_str(), realname.c_str());
      symbol.isdll = 1;
      // __imp__形式会为0的情况,把它变成函数
      symbol.type = 0x0020;
      return 1;
    }
  }
  return 0;
}

