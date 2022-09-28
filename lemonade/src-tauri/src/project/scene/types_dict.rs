use serde::{Deserialize, Serialize};
use serde_json::{from_str, Result};
use std::{collections::HashMap, fs};

#[derive(Deserialize, Debug)]
#[serde(untagged, rename_all = "lowercase")]
pub enum FieldType {
    I8,
    I16,
    I32,
    I64,
    U8,
    U16,
    U32,
    U64,
    #[serde(rename = "std::size_t")]
    SizeT,
    F32,
    F64,
    IVec2,
    IVec3,
    IVec4,
    UVec2,
    UVec3,
    UVec4,
    Vec2,
    Vec3,
    Vec4,
    Mat2,
    Mat3,
    Mat4,
    #[serde(rename = "std::string", alias = "string")]
    Str,
    #[serde(rename = "hash_str")]
    HashStr,
    Asset,
    Entity,
}

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct StageModel {
    pub id: u32,
    pub name: String,
}

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct SystemModel {
    pub id: u32,
    pub name: String,
}

#[derive(Deserialize, Debug)]
pub struct FieldModel {
    pub name: String,
    #[serde(rename = "type")]
    pub _type: FieldType,
}

#[derive(Deserialize, Debug)]
pub struct ComponentModel {
    pub name: String,
    pub fields: Vec<FieldModel>,
}

#[derive(Deserialize, Debug)]
pub struct Types {
    pub stages: HashMap<u32, String>,
    pub systems: HashMap<u32, String>,
    pub components: HashMap<String, HashMap<String, String>>,
}

fn read_types_file(path: &str) -> Result<Types> {
    let contents = fs::read_to_string(path).expect("Failed to read types file");
    let types: Types = from_str(&contents)?;
    Ok(types)
}

pub fn read_types(path: &str) -> Result<Types> {
    read_types_file(path)
}
