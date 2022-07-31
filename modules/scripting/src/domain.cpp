#include <lemon/scripting/domain.h>

#include <lemon/core/assert.h>
#include <lemon/core/logger.h>

#include <fstream>

namespace lemon {
using namespace std;
domain::domain()
{
    mono_set_assemblies_path("/lib64/mono/4.5");
    rootDomain = mono_jit_init("LemonJITRuntime");
    lemon_assert(rootDomain != nullptr);

    appDomain = mono_domain_create_appdomain("LemonScriptDomain", nullptr);
    lemon_assert(appDomain != nullptr);
    mono_domain_set(appDomain, true);

    auto test = load_assembly("/home/matzix/shared/Lemon_alt/ScriptCore/bin/Debug/net45/Lemon.dll");
    get_systems(test);
}
domain::~domain()
{
    mono_domain_unload(appDomain);
    mono_jit_cleanup(rootDomain);
}
char* domain::load_file(const std::string& path, std::size_t& outSize)
{
    ifstream file;
    file.open(path, ios::ate | ios::binary);
    size_t size = file.tellg();
    file.seekg(0, file.beg);

    char* buffer = new char[size];
    file.read((char*)buffer, size);
    file.close();

    outSize = size;
    return buffer;
}
MonoAssembly* domain::load_assembly(const std::string& path)
{
    size_t fileSize = 0;
    auto fileData   = load_file(path, fileSize);

    MonoImageOpenStatus status;
    MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

    if(MONO_IMAGE_OK != status)
    {
        const char* errorMessage = mono_image_strerror(status);
        logger::error("Loading assembly from {} failed with: {}", path, errorMessage);
        return nullptr;
    }
    MonoAssembly* assembly = mono_assembly_load_from_full(image, path.c_str(), &status, 0);
    if(MONO_IMAGE_OK != status)
    {
        const char* errorMessage = mono_image_strerror(status);
        logger::error("Loading assembly from {} failed with: {}", path, errorMessage);
        return nullptr;
    }
    mono_image_close(image);
    delete[] fileData;

    return assembly;
}
void domain::print_assembly_types(MonoAssembly* assembly)
{
    MonoImage* image                          = mono_assembly_get_image(assembly);
    const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
    i32 numTypes                              = mono_table_info_get_rows(typeDefinitionsTable);

    for(i32 i = 0; i < numTypes; ++i)
    {
        u32 cols[MONO_TYPEDEF_SIZE];
        mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

        const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
        const char* name      = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

        logger::info("{}.{}", nameSpace, name);
    }
}
std::vector<MonoClass*> domain::get_systems(MonoAssembly* assembly)
{
    std::vector<MonoClass*> systems;
    MonoImage* image               = mono_assembly_get_image(assembly);
    const MonoTableInfo* tableInfo = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
    int rows                       = mono_table_info_get_rows(tableInfo);
    auto system                    = mono_class_from_name(image, "Lemon.World", "LemonSystem");

    for(int i = 0; i < rows; ++i)
    {
        MonoClass* _class = nullptr;
        u32 cols[MONO_TYPEDEF_SIZE];
        mono_metadata_decode_row(tableInfo, i, cols, MONO_TYPEDEF_SIZE);
        const char* name      = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
        const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
        _class                = mono_class_from_name(image, nameSpace, name);
        bool isSystem         = mono_class_is_subclass_of(_class, system, false);
        if(isSystem && _class != system)
        {
            logger::info("{}.{}", nameSpace, name);
            systems.push_back(_class);
        }
    }
    return systems;
}
MonoClass* domain::get_class_in_assembly(MonoAssembly* assembly, const char* namespaceName, const char* className)
{
    MonoImage* image  = mono_assembly_get_image(assembly);
    MonoClass* _class = mono_class_from_name(image, namespaceName, className);
    if(_class == nullptr)
    {
        logger::error("Loading class {} in {} failed", className, namespaceName);
        return nullptr;
    }
    return _class;
}
MonoObject* domain::instantiate_class(MonoAssembly* assembly, const char* namespaceName, const char* className)
{
    // Get a reference to the class we want to instantiate
    MonoClass* testingClass = get_class_in_assembly(assembly, namespaceName, className);

    // Allocate an instance of our class
    MonoObject* classInstance = mono_object_new(appDomain, testingClass);

    if(classInstance == nullptr)
    {
        logger::error("Failed to instantiate class {}.{}", namespaceName, className);
    }

    // Call the parameterless (default) constructor
    mono_runtime_object_init(classInstance);
    return classInstance;
}
void domain::call_print_float_var_method(MonoObject* objectInstance)
{
    // Get the MonoClass pointer from the instance
    MonoClass* instanceClass = mono_object_get_class(objectInstance);

    // Get a reference to the method in the class
    MonoMethod* method = mono_class_get_method_from_name(instanceClass, "GetFloat", 0);

    if(method == nullptr)
    {
        logger::error("error");
        return;
    }

    // Call the C# method on the objectInstance instance, and get any potential exceptions
    MonoObject* exception = nullptr;
    auto ret              = mono_runtime_invoke(method, objectInstance, nullptr, &exception);
    logger::info("val = {:f}", *((f32*)mono_object_unbox(ret)));
}
void domain::call_increment_float_var_method(MonoObject* objectInstance, float value)
{
    // Get the MonoClass pointer from the instance
    MonoClass* instanceClass = mono_object_get_class(objectInstance);

    // Get a reference to the method in the class
    MonoMethod* method = mono_class_get_method_from_name(instanceClass, "IncrementFloatVar", 1);

    if(method == nullptr)
    {
        logger::error("Failed");
        // No method called "IncrementFloatVar" with 1 parameter in the class, log error or something
        return;
    }

    // Call the C# method on the objectInstance instance, and get any potential exceptions
    MonoObject* exception = nullptr;
    void* param           = &value;
    mono_runtime_invoke(method, objectInstance, &param, &exception);
}
} // namespace lemon
