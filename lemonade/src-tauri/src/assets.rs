use serde::{Deserialize, Serialize};
use serde_json::Result;
use std::fs;

#[derive(Serialize, Deserialize)]
struct Path {
    name: String,
    path: String,
}

#[derive(Serialize, Deserialize)]
pub struct Assets {
    textures: Vec<Path>,
    sounds: Vec<Path>,
    shaders: Vec<Path>,
}

pub fn read_assets(path: &str) -> Result<Assets> {
    let contents = fs::read_to_string(path).expect("Failed to read the asset file");

    let assets: Assets = serde_json::from_str(&contents)?;
    Ok(assets)
}
