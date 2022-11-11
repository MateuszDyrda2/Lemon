import sys
import os
import json

file_template = '''
#include <serialization/serializer.h>
#include <serialization/basic_types_serializer.h>
{}

namespace lemon {{
void serializer::register_all([[maybe_unused]] registry& _registry) {{
{}
}}
}}
'''

include_template = '#include <{}>\n'

component_template = '''
    [[maybe_unused]] auto&& {0}_storage = _registry.storage<{0}>();
    [[maybe_unused]] auto {0}_id = entt::type_hash<{0}>::value();
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
            s->basicTypesSerializer.deserialize(component->{}, (iter++)->value);
'''

def main(argv):
    output_path = argv[1]
    input_path = argv[2]
    include_path = argv[3]
    components = {}
    print(argv)

    include_strings = include_path.split(' ')

    with open(input_path, 'r') as f:
        components = json.load(f)['components']

    serialization_components = [component_template.format(k, ''.join(field_template.format(kk) for kk,val in v.items()), ''.join(field_out_template.format(kk) for kk,val in v.items())) for k,v in components.items()]

    serialization_file = file_template.format(''.join(include_template.format(i) for i in include_strings),''.join(c for c in serialization_components))

    with open(output_path, 'w+') as f:
        f.write(serialization_file)


if __name__ == '__main__':
    if len(sys.argv) < 4:
        print('Not enough arguments')
    else:
        main(sys.argv)

