#![cfg_attr(
    all(not(debug_assertions), target_os = "windows"),
    windows_subsystem = "windows"
)]
mod project;
use std::sync::Mutex;

use tauri::Manager;

use project::{
    get_assets, get_components, get_entities, get_entity_components, get_project_name,
    get_system_definitions, get_systems, open_project, set_systems, ProjectState,
};
#[tauri::command]
async fn close_splashscreen(window: tauri::Window) {
    if let Some(splashscreen) = window.get_window("splashscreen") {
        splashscreen.close().unwrap();
    }
    window.get_window("main").unwrap().show().unwrap();
}

fn main() {
    tauri::Builder::default()
        .manage(ProjectState(Mutex::new(None)))
        .invoke_handler(tauri::generate_handler![
            open_project,
            get_assets,
            get_systems,
            get_project_name,
            get_system_definitions,
            set_systems,
            get_entities,
            get_components,
            get_entity_components,
            close_splashscreen
        ])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
