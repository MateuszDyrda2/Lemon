#![cfg_attr(
    all(not(debug_assertions), target_os = "windows"),
    windows_subsystem = "windows"
)]
mod project;
mod viewport;
use std::sync::Mutex;

use tauri::{Manager, WindowEvent};

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
        .setup(|app| {
            let main_window = app.get_window("main").unwrap();
            //Some(thread::spawn(move || {
            //    let vp = Viewport::new(main_window, 720, 480);
            //    vp.run(rx);
            //}));
            Ok(())
        })
        .on_window_event(move |e| match e.event() {
            WindowEvent::CloseRequested { api, .. } => {}
            _ => (),
        })
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
