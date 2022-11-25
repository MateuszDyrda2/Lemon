use std::collections::HashMap;

use serde::Serialize;
use serde_json::{json, Value};

use super::{
    error_codes::ProjectErrorCode,
    models::{FieldType, Scene, Types},
};

pub fn hash_string(val: &str) -> Option<u32> {
    if val.len() == 0 {
        return None;
    }

    let mut hash = 5381u32;

    for c in val.as_bytes() {
        hash = hash
            .wrapping_shl(5)
            .wrapping_add(hash)
            .wrapping_add(u32::from(*c));
    }

    Some(hash)
}

pub fn field_to_value(ftype: &FieldType) -> Value {
    match ftype {
        FieldType::Bool => json!(false),
        FieldType::I8
        | FieldType::I16
        | FieldType::I32
        | FieldType::I64
        | FieldType::U8
        | FieldType::U16
        | FieldType::U32
        | FieldType::U64
        | FieldType::SizeT
        | FieldType::Asset
        | FieldType::Entity
        | FieldType::Unknown => json!(0),
        FieldType::F32 | FieldType::F64 => json!(0.0f64),
        FieldType::IVec2 | FieldType::UVec2 | FieldType::Vec2 => json!([0, 0]),
        FieldType::IVec3 | FieldType::UVec3 | FieldType::Vec3 => json!([0, 0, 0]),
        FieldType::IVec4 | FieldType::UVec4 | FieldType::Vec4 => json!([0, 0, 0, 0]),
        FieldType::Mat2 => json!([[0, 0], [0, 0]]),
        FieldType::Mat3 => json!([[0, 0, 0], [0, 0, 0], [0, 0, 0]]),
        FieldType::Mat4 => json!([[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]]),
        FieldType::Str | FieldType::HashStr => json!(""),
    }
}
