#ifndef CORE_ENDINGE_H
#define CORE_ENDINGE_H
#include "utility.h"
#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;

namespace ShellCodeEngine {
  struct Section;
  struct Symbol;
  struct Reloc;
  class ObjectFileInfo;
  class DLLParse;
  class Builder;

  struct Section {
  public:
    vector<Reloc>& relocs() { return reloc_list_; };
  public:
    enum Align {
      UNKNOW = 0,
      BYTES_1,
      BYTES_2,
      BYTES_4,
      BYTES_8,
      BYTES_16,
      BYTES_32,
      BYTES_64,
      BYTES_128,
      BYTES_256,
      BYTES_512,
      BYTES_1024,
      BYTES_2048,
      BYTES_4096,
      BYTES_8192,
    };
    int discard;
    std::string name;
    int rva;
    int virtual_size;
    char* rawdata;
    int raw_size;
    int readable;
    int writeable;
    int code_contain;
    int executeable;
    int initalize_data;
    int uninitalize_data;
    int comdat;
    Align align;
  private:
    vector<Reloc> reloc_list_;
  };

  // 暂时先不考虑comdat
  struct Symbol {
    int isaux;
    int isdll;
    int ref;
    string name;
    uint32_t value;
    uint16_t section_number;
    uint16_t type;
    uint8_t storage_class;
    uint8_t aux_count;
    void addref() { ref++; };
    void reset_ref() { ref = 0; };
  };

  struct Reloc {
    int rva;
    int type;
    Symbol* symbol;
  };

  class ObjectFileInfo {
  public:
    enum Machine { UNKNOW = 0, X64, X86 };
    ObjectFileInfo(string& path);
    ~ObjectFileInfo();
    inline int loaded() { 
      return loader_->loaded();
    };
    inline int processed() {
      return processed_;
    }
    inline Machine type() {
      return machine_;
    };
    inline string name() { 
      return name_; 
    };
    inline size_t size() {
      return loader_->size();
    }
    inline vector<Section>& section_list() {
      return section_list_;
    }
    inline vector<Symbol>& symbol_list() {
      return symbol_list_;
    }
  private:
    int process();
    template <typename T>
    T cast(uint32_t offset);
  private:
    FileLoader* loader_;
    Machine machine_;
    vector<Section> section_list_;
    vector<Symbol> symbol_list_;
    string name_;
    int processed_;
  };

  class DLLPares {
  public:
    DLLPares(string& path);
    inline vector<string> exports() {
      return export_;
    }
    inline int parsed() {
      return parsed_;
    }
    inline string name() {
      return name_;
    }
    int find_symbol(string& name);
  private:
    int loadfile(string& path);
    string name_;
    vector<string> export_;
    int parsed_;
  };

  class TemplateBuffer {
  public:
    TemplateBuffer(size_t capcity = 1024);
    ~TemplateBuffer();
    int write(std::string& str);
    int write(const char* buffer, size_t size);
    template <typename T>
    int write(T buffer);
    void clear() {
      memset(buffer_base_, 0, total_size_);
      pos_ = 0;
      current_size_ = 0;
    };
    void seek(int pos) {
      if (pos < 0) {
        pos_ = current_size_;
      }
      else {
        pos_ = pos;
      }
    };
    inline char* base() {
      return buffer_base_;
    };
    inline size_t size() {
      return current_size_;
    };
  private:
    void check(size_t size);
    char* buffer_base_;
    size_t total_size_;
    size_t current_size_;
    size_t pos_;
  };

  template <typename T>
  int ShellCodeEngine::TemplateBuffer::write(T buffer) {
    return write((const char *)&buffer, sizeof(T));
  }

  class Builder {
  public:
    Builder();
    ~Builder();
    int add_module(string& path);
    int add_object_file(string& path);
    void get_function_list(vector<string>& o);
    int build(string& path, string& entry);
  private:
    struct DataBuffer {
      string name;
      Section* section;
      uint32_t offset;  // offset in file
    };
    // 如果是Import类型的话 dllname有效 section有效
    struct FunctionBuffer {
      bool isimport;
      string name;
      string dllname;  // if is import this is available
      uint32_t offset;
      Section* section;     // if is function this is available
    };
    enum RelocType {
      RelocType_REL32,
      RelocType_DIR32
    };
    struct FunctionReloc {
      bool isdata;
      bool processed;
      uint32_t offset;
      RelocType type;
      DataBuffer* data;
      FunctionBuffer* func;
    };
    struct Import {
      string funcname;
      uint32_t offset;              // jmp [0x00000000]
      vector<FunctionReloc*> relocs;
    };

    struct ImportReloc {
      string dllname;
      vector<Import> imports;
    };
  private:
    TemplateBuffer shellcode_;
    TemplateBuffer extract_data_;
    TemplateBuffer reloc_data_;
    void reset_symbol_ref();
    int preprocess(string& entry);
    int process_symbol(Symbol& symbol, ObjectFileInfo& objinfo);
    int process_external_symbol(Symbol& symbol);
    vector<DLLPares*> module_list_;
    vector<ObjectFileInfo*> object_list_;
  };
  
  template<typename T>
  inline T ObjectFileInfo::cast(uint32_t offset) {
    ASSERT(loader_ != nullptr);
    ASSERT(offset != 0 && offset < loader_->size());
    return (T)(loader_->base() + offset);
  }

};  // namespace ShellCodeEngine


#endif  // CORE_ENDINGE_H