#![cfg_attr(
    all(not(debug_assertions), target_os = "windows"),
    windows_subsystem = "windows"
)]

use serde::{Deserialize, Serialize};
use serde_json::{Result, Value};
use std::env;
use std::fs;

#[derive(Serialize, Deserialize)]
struct Path {
    name: String,
    path: String,
}

#[derive(Serialize, Deserialize)]
struct Assets {
    textures: Vec<Path>,
    sounds: Vec<Path>,
    shaders: Vec<Path>,
}

fn read_assets(path: &str) -> Result<Assets> {
    let contents = fs::read_to_string(path).expect("Failed to read the asset file");

    let assets: Assets = serde_json::from_str(&contents)?;
    Ok(assets)
}

#[tauri::command]
fn get_assets() -> Assets {
    read_assets("D:/custom/Lemon/Sandbox/assets/assets.json").unwrap()
}

fn main() {
    tauri::Builder::default()
        .invoke_handler(tauri::generate_handler![get_assets])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
