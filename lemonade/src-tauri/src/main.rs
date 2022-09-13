#![cfg_attr(
    all(not(debug_assertions), target_os = "windows"),
    windows_subsystem = "windows"
)]
mod project;

use project::{
    get_assets, get_components, get_project_name, get_system_definitions, get_systems,
    open_project, set_systems, ProjectState,
};
use std::sync::Mutex;

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
            get_components
        ])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
