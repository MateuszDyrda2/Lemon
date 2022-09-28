use serde::{Deserialize, Serialize};
use serde_json::{from_str, Value};
use std::{collections::HashMap, fs};

pub mod types_dict;
use types_dict::Types;

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
    pub textures: Vec<Asset>,
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

fn read_scene_file(path: &str) -> Result<Scene, serde_json::error::Error> {
    let contents = fs::read_to_string(path).expect("Failed to read scene file");
    let scene: Scene = from_str(&contents)?;
    Ok(scene)
}

pub fn read_scene(path: &str) -> Result<Scene, serde_json::error::Error> {
    read_scene_file(path)
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
