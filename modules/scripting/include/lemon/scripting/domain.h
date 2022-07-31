#pragma once

#include <lemon/core/lemon_types.h>

#include <mono-2.0/mono/jit/jit.h>
#include <mono-2.0/mono/metadata/assembly.h>

#include <string>
#include <vector>

namespace lemon {
class domain
{
  public:
    domain(/* args */);
    ~domain();

    char* load_file(const std::string& path, std::size_t& outSize);
    MonoAssembly* load_assembly(const std::string& path);
    void print_assembly_types(MonoAssembly* assembly);
    MonoClass* get_class_in_assembly(MonoAssembly* assembly, const char* namespaceName, const char* className);
    MonoObject* instantiate_class(MonoAssembly* assembly, const char* namespaceName, const char* className);
    void call_print_float_var_method(MonoObject* objectInstance);
    void call_increment_float_var_method(MonoObject* objectInstance, float value);
    std::vector<MonoClass*> get_systems(MonoAssembly* assembly);

  private:
    MonoDomain* rootDomain;
    MonoDomain* appDomain;
};
} // namespace lemon
