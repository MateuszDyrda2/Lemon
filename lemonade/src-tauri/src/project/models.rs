use serde::{Deserialize, Serialize};
use serde_json::Value;
use std::collections::HashMap;

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct Stage {
    pub stage: u32,
    pub systems: Vec<u32>,
}

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct Entities {
    pub count: u32,
    pub ids: Vec<u32>,
}

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct Asset {
    pub name: String,
    pub path: String,
}

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct Assets {
    pub textures: Vec<Assets>,
    pub sounds: Vec<Asset>,
    pub shaders: Vec<Asset>,
}

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct Component {
    pub name: String,
    pub count: u32,
    pub entities: HashMap<u32, HashMap<String, Value>>,
}

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct Scene {
    pub systems: Vec<Stage>,
    pub entities: Entities,
    pub components: Vec<Component>,
    pub assets: Assets,
}

#[derive(Serialize, Deserialize)]
pub struct Project {
    pub project_name: String,
    pub assets_path: String,
    pub scene_path: String,
    pub src_path: String,
    pub build_dir: String,
    pub scenes: Vec<String>,
    #[serde(skip)]
    pub current_scene: Option<Scene>,
    #[serde(skip)]
    pub data_set: Option<Types>,
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
