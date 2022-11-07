mod error_codes;
mod models;

use serde::{Deserialize, Serialize};
use serde_json::{from_str, Value};
use std::collections::HashMap;
use std::{fs, path::Path, result::Result, sync::Mutex};
use tauri::Window;

use self::models::*;
use error_codes::ProjectErrorCode;

pub struct ProjectState(pub Mutex<Option<Project>>);

macro_rules! unwrap_or_return {
    ( $e:expr ) => {
        match $e {
            Ok(x) => x,
            Err(code) => return Err(code),
        }
    };
}

macro_rules! lock_state {
    ( $e:expr ) => {{
        $e.0.lock().unwrap()
    }};
}

fn read_project_file(path: &str) -> Result<Project, serde_json::error::Error> {
    let contents = fs::read_to_string(path).expect("Failed to read the project file");

    let project: Project = from_str(&contents)?;
    Ok(project)
}

fn read_scene_file(path: &str) -> Result<Scene, serde_json::error::Error> {
    let contents = fs::read_to_string(path).expect("Failed to read scene file");

    let scene: Scene = from_str(&contents)?;
    Ok(scene)
}

fn read_types_file(path: &str) -> Result<Types, serde_json::error::Error> {
    let contents = fs::read_to_string(path).expect("Failed to read types file");

    let types: Types = from_str(&contents)?;
    Ok(types)
}

fn get_scene(project: &Project) -> Result<&Scene, ProjectErrorCode> {
    project
        .current_scene
        .as_ref()
        .ok_or(ProjectErrorCode::NoSceneOpened)
}

fn get_project(project: &Option<Project>) -> Result<&Project, ProjectErrorCode> {
    project.as_ref().ok_or(ProjectErrorCode::NoProjectLoaded)
}

fn get_dataset(project: &Project) -> Result<&Types, ProjectErrorCode> {
    project
        .data_set
        .as_ref()
        .ok_or(ProjectErrorCode::NoDatasetLoaded)
}

#[tauri::command]
pub fn open_project(
    window: Window,
    state: tauri::State<ProjectState>,
    path: &str,
) -> Result<String, &'static str> {
    let mut state_guard = lock_state!(state);

    match read_project_file(path) {
        Ok(p) => {
            *state_guard = Some(p);
        }
        Err(e) => println!("Error parsing the project file: {}", e.to_string()),
    }

    if let Some(project) = &mut (*state_guard) {
        let parent = Path::new(path).parent();
        let scenes_path = parent.unwrap().join(&project.scene_path);
        let scene_name = project.scenes.first().unwrap().to_owned();
        let scene_path = scenes_path.join(scene_name + ".json");

        match read_scene_file(scene_path.to_str().unwrap()) {
            Ok(s) => project.current_scene = Some(s),
            Err(e) => println!("{}", e.to_string()),
        }

        let types = parent
            .unwrap()
            .join(project.build_dir.to_string() + "/_generated/types.json");

        match read_types_file(types.to_str().unwrap()) {
            Ok(t) => project.data_set = Some(t),
            Err(e) => println!("{}", e.to_string()),
        }

        _ = window.emit("project-opened", "");

        Ok(project.project_name.clone())
    } else {
        Err("Error occured while reading the file")
    }
}

#[tauri::command]
pub fn get_assets(
    _window: Window,
    state: tauri::State<ProjectState>,
) -> Result<Assets, ProjectErrorCode> {
    let state_guard = state.0.lock().unwrap();

    let project = unwrap_or_return!(get_project(&(*state_guard)));
    let scene = unwrap_or_return!(get_scene(project));
    Ok(scene.assets.clone())
}

#[derive(Serialize, Deserialize, Debug)]
pub struct EntityDTO {
    id: u32,
    name: String,
}

#[tauri::command]
pub fn get_components(
    _window: Window,
    state: tauri::State<ProjectState>,
) -> Result<Vec<String>, ProjectErrorCode> {
    let state_guard = state.0.lock().unwrap();

    let project = unwrap_or_return!(get_project(&(*state_guard)));
    let dataset = unwrap_or_return!(get_dataset(project));

    Ok(dataset.components.keys().cloned().collect::<Vec<String>>())
}

#[tauri::command]
pub fn get_components_for_entity(
    _window: Window,
    state: tauri::State<ProjectState>,
    entitid: u32,
) -> Result<HashMap<String, HashMap<String, Value>>, ProjectErrorCode> {
    let state_guard = state.0.lock().unwrap();

    let project = unwrap_or_return!(get_project(&(*state_guard)));
    let scene = unwrap_or_return!(get_scene(project));

    Ok((&scene.components)
        .iter()
        .filter(|c| c.entities.contains_key(&entitid))
        .map(|c| (c.name.clone(), c.entities.get(&entitid).unwrap().clone()))
        .collect::<HashMap<String, HashMap<String, Value>>>())
}

#[tauri::command]
pub fn get_entities(
    _window: Window,
    state: tauri::State<ProjectState>,
) -> Result<Vec<EntityDTO>, ProjectErrorCode> {
    let state_guard = lock_state!(state);

    let project = unwrap_or_return!(get_project(&(*state_guard)));
    let scene = unwrap_or_return!(get_scene(project));

    let names = (&scene.components).iter().find(|x| x.name == "tag");
    let entities = &names.as_ref().unwrap().entities;

    let ret_ent = entities
        .iter()
        .map(|(key, val)| EntityDTO {
            id: *key,
            name: val.get("name").unwrap().as_str().unwrap().to_string(),
        })
        .collect::<Vec<EntityDTO>>();

    Ok(ret_ent)
}
