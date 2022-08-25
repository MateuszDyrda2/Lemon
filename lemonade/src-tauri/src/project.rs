use serde::{Deserialize, Serialize};
use serde_json::{from_str, Result};
use std::{fs, sync::Mutex};
use tauri::Window;

#[derive(Serialize, Deserialize)]
pub struct Project {
    project_name: String,
    assets_path: String,
    scene_path: String,
    src_path: String,
    build_dir: String,
    scenes: Vec<String>,
}
pub struct ProjectState(pub Mutex<Option<Project>>);

fn read_file(path: &str) -> Result<Project> {
    let contents = fs::read_to_string(path).expect("Failed to read the project file");

    let project: Project = from_str(&contents)?;
    Ok(project)
}
fn read_scene(path: &str) {}

#[tauri::command]
pub fn open_project(window: tauri::Window, path: &str, state: tauri::State<ProjectState>) {
    let mut state_guard = state.0.lock().unwrap();

    match read_file(path) {
        Ok(p) => *state_guard = Some(p),
        Err(e) => println!("error {} parsing the project file", e.to_string()),
    }

    window.emit("project-opened", &(*state_guard).as_ref().unwrap());
}

//fn read_file(path: &str) -> Result<Project> {
//    let contents = fs::read_to_string(path).expect("Failed to read the project file");
//
//    let project: Project = from_str(&contents)?;
//    Ok(project)
//}
//
//pub fn read_project(window: &Window, path: &str) {
//    let project = read_file(&path);
//
//    match project {
//        Ok(p) => {
//            window.emit("project-opened", &p);
//        }
//        Err(e) => println!("error {} parsing the project file", e.to_string()),
//    }
//}
//
