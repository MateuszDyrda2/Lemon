#![cfg_attr(
    all(not(debug_assertions), target_os = "windows"),
    windows_subsystem = "windows"
)]

use std::sync::Mutex;

use assets::{read_assets, Assets};
use project::{open_project, Project, ProjectState};
use tauri::Manager;

mod assets;
mod project;

#[tauri::command]
fn get_assets() -> Assets {
    read_assets("D:/custom/Lemon/Sandbox/assets/assets.json").unwrap()
}

fn main() {
    tauri::Builder::default()
        .manage(ProjectState(Mutex::new(None)))
        .invoke_handler(tauri::generate_handler![open_project])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
