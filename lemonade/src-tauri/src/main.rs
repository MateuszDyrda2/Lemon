#![cfg_attr(
    all(not(debug_assertions), target_os = "windows"),
    windows_subsystem = "windows"
)]

use assets::{read_assets, Assets};
use project::read_project;
use tauri::Manager;

mod assets;
mod project;

#[derive(Clone, serde::Serialize)]
struct Payload {
    message: String,
}
#[tauri::command]
fn get_assets() -> Assets {
    read_assets("D:/custom/Lemon/Sandbox/assets/assets.json").unwrap()
}

fn main() {
    tauri::Builder::default()
        .setup(|app| {
            let window = app.get_window("main").unwrap();
            let copy = window.clone();
            window.listen("open-project", move |event| {
                read_project(&copy, event.payload().unwrap());
            });
            Ok(())
        })
        .invoke_handler(tauri::generate_handler![get_assets])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
