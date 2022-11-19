use std::fs::read_to_string;

use super::models::{Assets, Project, Scene, Types};
use serde_json::from_str;

pub fn read_project_file(path: &str) -> Result<Project, serde_json::error::Error> {
    let contents = read_to_string(path).expect("Failed to read the project file");

    let project: Project = from_str(&contents)?;
    Ok(project)
}

pub fn read_scene_file(path: &str) -> Result<Scene, serde_json::error::Error> {
    let contents = read_to_string(path).expect("Failed to read scene file");

    let scene: Scene = from_str(&contents)?;
    Ok(scene)
}

pub fn read_types_file(path: &str) -> Result<Types, serde_json::error::Error> {
    let contents = read_to_string(path).expect("Failed to read types file");

    let types: Types = from_str(&contents)?;
    Ok(types)
}

pub fn read_assets_file(path: &str) -> Result<Assets, serde_json::error::Error> {
    let contents = read_to_string(path).expect("Failed to read assets file");

    let assets: Assets = from_str(&contents)?;
    Ok(assets)
}
