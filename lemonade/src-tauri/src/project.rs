use serde::{Deserialize, Serialize};
use serde_json::{from_str, Value};
use std::{fs, path::Path, result::Result, sync::Mutex};
use tauri::Window;

mod scene;
use scene::{read_scene, Assets, Project, Scene, Stage};

use scene::types_dict::read_types;

use self::scene::types_dict::{StageModel, SystemModel};

pub struct ProjectState(pub Mutex<Option<Project>>);

#[derive(Serialize, Deserialize)]
pub enum ProjectErrorCode {
    NoProjectLoaded,
    NoSceneOpened,
    NoDatasetLoaded,
    NoMatchedStage,
    NoMatchedSystem,
}

fn read_file(path: &str) -> Result<Project, serde_json::error::Error> {
    let contents = fs::read_to_string(path).expect("Failed to read the project file");

    let project: Project = from_str(&contents)?;
    Ok(project)
}

#[tauri::command]
pub fn open_project(
    window: Window,
    path: &str,
    state: tauri::State<ProjectState>,
) -> Result<String, &'static str> {
    let mut state_guard = state.0.lock().unwrap();

    match read_file(path) {
        Ok(p) => {
            *state_guard = Some(p);
        }
        Err(e) => println!("error {} parsing the project file", e.to_string()),
    }
    match &mut (*state_guard) {
        Some(p) => {
            let parent = Path::new(path).parent();
            let scenes_path = parent.unwrap().join(&p.scene_path);
            let scene_name = p.scenes.first().unwrap().to_owned();
            let scene_path = scenes_path.join(scene_name + ".lemon");

            match read_scene(scene_path.to_str().unwrap()) {
                Ok(s) => p.current_scene = Some(s),
                Err(e) => println!("Failed to read a scene with: {}", e.to_string()),
            }
            let types = parent.unwrap().join("types.json");

            match read_types(types.to_str().unwrap()) {
                Ok(t) => p.data_set = Some(t),
                Err(e) => println!(
                    "Failed to read type definition file, Err: {}",
                    e.to_string()
                ),
            }

            _ = window.emit("project-opened", "");
            println!("{:?}", &p.current_scene.as_ref().unwrap().components);
            Ok(p.project_name.clone())
        }
        None => Err("Could not find a project"),
    }
}

#[tauri::command]
pub fn get_assets(
    _window: Window,
    state: tauri::State<ProjectState>,
) -> Result<Assets, &'static str> {
    let state_guard = state.0.lock().unwrap();

    match &(*state_guard) {
        Some(p) => match &(p.current_scene) {
            Some(s) => Result::Ok(s.assets.clone()),
            None => Result::Err("No active scene"),
        },
        None => Result::Err("Failed to get assets / no active project"),
    }
}

#[derive(Serialize, Deserialize, Debug)]
pub struct NamedStages {
    stage: StageModel,
    systems: Vec<SystemModel>,
}

#[tauri::command]
pub fn get_project_name(
    _window: Window,
    state: tauri::State<ProjectState>,
) -> Result<String, &'static str> {
    let state_guard = state.0.lock().unwrap();

    if let Some(proj) = &(*state_guard) {
        Ok(proj.project_name.clone())
    } else {
        Err("project not loaded")
    }
}

#[tauri::command]
pub fn get_systems(
    _window: Window,
    state: tauri::State<ProjectState>,
) -> Result<Vec<NamedStages>, ProjectErrorCode> {
    let state_guard = state.0.lock().unwrap();

    let mut named_stages: Vec<NamedStages> = Vec::new();
    let proj = (&(*state_guard)).ok_or(ProjectErrorCode::NoProjectLoaded)?;
    let data_set = (&proj.data_set).ok_or(ProjectErrorCode::NoDatasetLoaded)?;

    let stg = &data_set.stages;
    let sys = &data_set.systems;

    let scene = (&proj.current_scene).ok_or(ProjectErrorCode::NoProjectLoaded)?;
    for stage in scene.systems.iter() {
        let mut named_systems: Vec<SystemModel> = Vec::new();
        for system in stage.systems.iter() {
            match sys.into_iter().find(|&x| x.id == *system) {
                Some(item) => named_systems.push(item.clone()),
                None => return Err(ProjectErrorCode::NoMatchedSystem),
            }
            match stg.into_iter().find(|&x| x.id == stage.stage) {
                Some(item) => named_stages.push(NamedStages {
                    stage: item.clone(),
                    systems: named_systems,
                }),
                None => return Err(ProjectErrorCode::NoMatchedStage),
            }
        }
    }
    Ok(named_stages)
}

#[tauri::command]
pub fn set_systems(
    _window: Window,
    state: tauri::State<ProjectState>,
    systemlist: Vec<NamedStages>,
) {
    let mut state_guard = state.0.lock().unwrap();

    if let Some(proj) = &mut (*state_guard) {
        if let Some(scene) = &mut proj.current_scene {
            let mut new_stages: Vec<Stage> = Vec::new();
            for stage in &systemlist {
                let mut new_systems: Vec<u32> = Vec::new();
                for sys in &stage.systems {
                    new_systems.push(sys.id);
                }
                new_stages.push(Stage {
                    stage: stage.stage.id,
                    systems: new_systems,
                });
            }
            scene.systems = new_stages;
        }
    }
}

#[tauri::command]
pub fn get_system_definitions(
    _window: Window,
    state: tauri::State<ProjectState>,
) -> Result<Vec<SystemModel>, ProjectErrorCode> {
    let state_guard = state.0.lock().unwrap();

    match &(*state_guard) {
        Some(proj) => match &proj.data_set {
            Some(data_set) => Ok(data_set.systems.clone()),
            None => Err(ProjectErrorCode::NoDatasetLoaded),
        },
        None => Err(ProjectErrorCode::NoProjectLoaded),
    }
}

#[tauri::command]
pub fn get_components(
    _window: Window,
    state: tauri::State<ProjectState>,
) -> Result<Vec<String>, ProjectErrorCode> {
    let state_guard = state.0.lock().unwrap();

    let proj = (&(*state_guard)).ok_or(ProjectErrorCode::NoProjectLoaded)?;
    let types = &proj.data_set.ok_or(ProjectErrorCode::NoSceneOpened)?;
    Ok(types.components.iter().map(|&c| c.name).collect())
}
