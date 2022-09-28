use serde::{Deserialize, Serialize};
use serde_json::{from_str, Value};
use std::borrow::Borrow;
use std::collections::HashMap;
use std::result;
use std::{fs, path::Path, result::Result, sync::Mutex};
use tauri::http::header::STRICT_TRANSPORT_SECURITY;
use tauri::Window;

mod scene;
use scene::{read_scene, Assets, Project, Stage};

use scene::types_dict::read_types;

use self::scene::types_dict::{StageModel, SystemModel};
use self::scene::Scene;

pub struct ProjectState(pub Mutex<Option<Project>>);

#[derive(Serialize, Deserialize, Debug)]
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

fn get_scene(project: &Project) -> Result<&Scene, ProjectErrorCode> {
    project
        .current_scene
        .as_ref()
        .ok_or(ProjectErrorCode::NoSceneOpened)
}

fn get_project(project: &Option<Project>) -> Result<&Project, ProjectErrorCode> {
    project.as_ref().ok_or(ProjectErrorCode::NoProjectLoaded)
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
            let scene_path = scenes_path.join(scene_name + ".json");

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
) -> Result<String, ProjectErrorCode> {
    let state_guard = state.0.lock().unwrap();

    match &(*state_guard) {
        Some(proj) => Ok(proj.project_name.clone()),
        None => Err(ProjectErrorCode::NoProjectLoaded),
    }
}

#[tauri::command]
pub fn get_systems(
    _window: Window,
    state: tauri::State<ProjectState>,
) -> Result<Vec<NamedStages>, ProjectErrorCode> {
    let state_guard = state.0.lock().unwrap();

    let mut named_stages: Vec<NamedStages> = Vec::new();
    match &(*state_guard) {
        Some(proj) => {
            let data_set = proj.data_set.as_ref().unwrap();
            let scene = proj.current_scene.as_ref().unwrap();

            let stg = &data_set.stages;
            let sys = &data_set.systems;

            for stage in scene.systems.iter() {
                let mut named_systems = Vec::new();
                for system in stage.systems.iter() {
                    match sys.get(system) {
                        Some(item) => named_systems.push(SystemModel {
                            id: *system,
                            name: item.clone(),
                        }),
                        None => return Err(ProjectErrorCode::NoMatchedSystem),
                    };
                }
                match stg.get(&stage.stage) {
                    Some(item) => named_stages.push(NamedStages {
                        stage: StageModel {
                            id: stage.stage,
                            name: item.clone(),
                        },
                        systems: named_systems,
                    }),
                    None => return Err(ProjectErrorCode::NoMatchedStage),
                }
            }
            Ok(named_stages)
        }
        None => Err(ProjectErrorCode::NoProjectLoaded),
    }
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
            Some(data_set) => Ok(data_set
                .systems
                .clone()
                .into_iter()
                .map(|(id, name)| SystemModel { id: id, name: name })
                .collect()),
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

    match &(*state_guard) {
        Some(proj) => match &proj.data_set {
            Some(data_set) => Ok(data_set.components.keys().cloned().collect::<Vec<String>>()),
            None => Err(ProjectErrorCode::NoDatasetLoaded),
        },
        None => Err(ProjectErrorCode::NoProjectLoaded),
    }
}

#[tauri::command]
pub fn get_entity_components(
    _window: Window,
    state: tauri::State<ProjectState>,
    entityid: u32,
) -> Result<HashMap<String, HashMap<String, Value>>, ProjectErrorCode> {
    let state_guard = state.0.lock().unwrap();
    let project = get_project(&(*state_guard)).unwrap();
    let scene = get_scene(project).unwrap();

    Ok((&scene.components)
        .iter()
        .filter(|c| c.entities.contains_key(&entityid))
        .map(|c| (c.name.clone(), c.entities.get(&entityid).unwrap().clone()))
        .collect::<HashMap<String, HashMap<String, Value>>>())
}

#[derive(Serialize, Deserialize, Debug)]
pub struct NamedEntity {
    id: u32,
    name: String,
}

#[tauri::command]
pub fn get_entities(
    _window: Window,
    state: tauri::State<ProjectState>,
) -> Result<Vec<NamedEntity>, ProjectErrorCode> {
    let state_guard = state.0.lock().unwrap();
    let project = get_project(&(*state_guard)).unwrap();
    let scene = get_scene(project).unwrap();

    let names = (&scene.components).iter().find(|x| x.name == "tag");
    let entities = &names.as_ref().unwrap().entities;
    let ret_ent = entities
        .iter()
        .map(|(key, val)| NamedEntity {
            id: *key,
            name: val.get("name").unwrap().as_str().unwrap().to_string(),
        })
        .collect::<Vec<NamedEntity>>();

    Ok(ret_ent)
}
