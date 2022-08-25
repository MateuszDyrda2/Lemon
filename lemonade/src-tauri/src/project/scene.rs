use serde::{Deserialize, Serialize};
use serde_json::{from_str, Result, Value};
use std::fs;

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct Stage {
    pub stage: u32,
    pub systems: Vec<u32>,
}

#[derive(Deserialize, Debug)]
pub struct Component {}

#[derive(Deserialize, Debug)]
pub struct Entities {
    pub count: u32,
    pub entities: Vec<u32>,
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

#[derive(Deserialize, Debug)]
pub struct Scene {
    pub systems: Vec<Stage>,
    pub entities: Entities,
    pub components: Vec<Value>,
    pub assets: Assets,
}

fn read_scene_file(path: &str) -> Result<Scene> {
    let contents = fs::read_to_string(path).expect("Failed to read scene file");
    let scene: Scene = from_str(&contents)?;
    Ok(scene)
}

pub fn read_scene(path: &str) -> Result<Scene> {
    read_scene_file(path)
}
