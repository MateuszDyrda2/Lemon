#![cfg_attr(
    all(not(debug_assertions), target_os = "windows"),
    windows_subsystem = "windows"
)]

mod project;
use std::sync::Mutex;
use tauri::{
    CustomMenuItem, Manager, SystemTray, SystemTrayEvent, SystemTrayMenu, SystemTrayMenuItem,
};

use project::{
    add_component_to_entity, get_asset_list, get_components, get_components_for_entity,
    get_entities, get_rendering_data, open_project, ProjectState,
};

#[tauri::command]
async fn close_splashscreen(window: tauri::Window) {
    if let Some(splashscreen) = window.get_window("splashscreen") {
        splashscreen.close().unwrap();
    }
    window.get_window("main").unwrap().show().unwrap();
}

fn main() {
    let quit = CustomMenuItem::new("quit".to_string(), "Quit");
    let hide = CustomMenuItem::new("hide".to_string(), "Hide");
    let tray_menu = SystemTrayMenu::new()
        .add_item(quit)
        .add_native_item(SystemTrayMenuItem::Separator)
        .add_item(hide);

    let system_tray = SystemTray::new().with_menu(tray_menu);

    tauri::Builder::default()
        .system_tray(system_tray)
        .on_system_tray_event(|app, event| match event {
            SystemTrayEvent::MenuItemClick { id, .. } => match id.as_str() {
                "quit" => std::process::exit(0),
                "hide" => {
                    let window = app.get_window("main").unwrap();
                    window.hide().unwrap()
                }
                _ => {}
            },
            _ => {}
        })
        .manage(ProjectState(Mutex::new(None)))
        .invoke_handler(tauri::generate_handler![
            close_splashscreen,
            open_project,
            get_components,
            get_components_for_entity,
            get_entities,
            get_rendering_data,
            get_asset_list,
            add_component_to_entity,
        ])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
