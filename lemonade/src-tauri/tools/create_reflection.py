import sys
import os
import json

file_template = '''
#include <serialization/serializer.h>
#include <serialization/basic_types_serializer.h>
#include <scripting/scripting_engine.h>
#include <world/scene.h>

extern "C"
{{
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <LuaBridge/LuaBridge.h>
#pragma GCC diagnostic pop
{}

namespace lemon {{
void serializer::register_all([[maybe_unused]] registry& _registry) {{
{}
}}

void scripting_engine::register_types() {{
    luabridge::getGlobalNamespace(L)
        .beginNamespace("lemon")
        {}
        .endNamespace();
}}
}}
'''

include_template = '#include <{}>\n'

component_template = '''
    [[maybe_unused]] auto&& {0}_storage = _registry.storage<{0}>();
    [[maybe_unused]] auto {0}_id = entt::type_hash<{0}>::value();
    [[maybe_unused]] auto {0}_hashid = hashstr::runtime_hash(\"{0}\").value;
    componentIds[{0}_hashid] = {0}_id;
    serializationData[{0}_id] = serialization_data {{
        .name = \"{0}\",
        .id = {0}_id,
        .serialize_f = +[]([[maybe_unused]] serializer* s,[[maybe_unused]] const void* data, [[maybe_unused]] rapidjson::Writer<rapidjson::StringBuffer>& writer) {{
            [[maybe_unused]] auto component = static_cast<const {0}*>(data);
            {1}
        }},
        .deserialize_f = +[]([[maybe_unused]] serializer* s,[[maybe_unused]] void* data,[[maybe_unused]] const rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstObject& obj) {{
            [[maybe_unused]] auto component = static_cast<{0}*>(data); 
            [[maybe_unused]] auto iter = obj.MemberBegin();
            {2}
        }}
    }};
'''

field_template = '''
            writer.Key(\"{0}\");
            s->basicTypesSerializer.serialize(component->{0}, writer);
'''
field_out_template = '''
            s->basicTypesSerializer.deserialize(component->{0}, obj.FindMember(\"{0}\")->value);
'''

scripting_template = '''
    '''
scripting_component_template = '''
        .beginClass<{0}>("{0}")
        {1}
        .endClass()
        .addFunction("get_{0}", +[](script_entity* ent) {{ return ent->_scene->get<{0}>(entity_t(ent->handle)); }})
'''
scripting_field_template = '''
        .addProperty("{1}", &{0}::{1})
'''


def main(argv):
    output_path = argv[1]
    input_path = argv[2]
    include_path = argv[3]
    components = {}

    include_strings = include_path.split(' ')

    with open(input_path, 'r') as f:
        components = json.load(f)['components']

    serialization_components = [component_template.format(k, ''.join(field_template.format(val["name"]) for val in v), ''.join(
        field_out_template.format(val["name"]) for val in v)) for k, v in components.items()]
    scripting_components = [scripting_component_template.format(k, ''.join(
        scripting_field_template.format(k, val["name"]) for val in v)) for k, v in components.items()]

    serialization_file = file_template.format(''.join(include_template.format(i) for i in include_strings), ''.join(
        c for c in serialization_components), ''.join(c for c in scripting_components))

    with open(output_path, 'w+') as f:
        f.write(serialization_file)


if __name__ == '__main__':
    if len(sys.argv) < 4:
        print('Not enough arguments')
    else:
        main(sys.argv)
