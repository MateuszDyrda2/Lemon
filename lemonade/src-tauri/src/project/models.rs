use std::collections::HashMap;

use serde::{Deserialize, Serialize};
use serde_json::Value;

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
    pub textures: Vec<Asset>,
    pub sounds: Vec<Asset>,
    pub shaders: Vec<Asset>,
    pub scripts: Vec<Asset>,
    pub animations: Vec<Asset>,
}

#[derive(Serialize, Debug, Clone)]
pub struct AssetLookup {
    pub assets: HashMap<u32, String>,
}

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct Component {
    pub name: String,
    pub count: u32,
    pub entities: HashMap<u32, HashMap<String, Value>>,
}

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct Scene {
    pub entities: Entities,
    pub components: Vec<Component>,
}

#[derive(Serialize, Deserialize, Debug, Clone)]
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
    #[serde(skip)]
    pub assets: Option<Assets>,
    #[serde(skip)]
    pub asset_lookup: Option<AssetLookup>,
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
    #[serde(rename = "std::size_t", alias = "size_t")]
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

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct Types {
    pub components: HashMap<String, HashMap<String, String>>,
}

#[derive(Serialize, Deserialize, Debug)]
pub struct RenderingData {
    pub nameid: u32,
    pub model: Vec<f32>,
    pub textureid: u32,
    pub texCoords: Vec<f32>,
}
