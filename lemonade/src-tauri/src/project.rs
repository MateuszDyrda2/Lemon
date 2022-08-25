use serde::{Deserialize, Serialize};
use serde_json::from_str;
use std::{fs, path::Path, result::Result, sync::Mutex};
use tauri::Window;

mod scene;
use scene::{read_scene, Assets, Scene, Stage};

mod types_dict;
use types_dict::{read_types, Types};

#[derive(Serialize, Deserialize)]
pub struct Project {
    project_name: String,
    assets_path: String,
    scene_path: String,
    src_path: String,
    build_dir: String,
    scenes: Vec<String>,
    #[serde(skip)]
    current_scene: Option<Scene>,
    #[serde(skip)]
    data_set: Option<Types>,
}
pub struct ProjectState(pub Mutex<Option<Project>>);

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
                Ok(s) => {
                    p.current_scene = Some(s);
                }
                Err(e) => println!("Failed to read a scene with: {}", e.to_string()),
            }
            let types = parent.unwrap().join("types.json");

            match read_types(types.to_str().unwrap()) {
                Ok(t) => {
                    p.data_set = Some(t);
                }
                Err(e) => println!(
                    "Failed to read type definition file, Err: {}",
                    e.to_string()
                ),
            }

            window.emit("project-opened", "");
            Ok(p.project_name.clone())
        }
        None => Err("Could not find a project"),
    }
}

#[tauri::command]
pub fn get_assets(
    window: Window,
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
    stage: String,
    systems: Vec<String>,
}

#[tauri::command]
pub fn get_project_name(
    window: Window,
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
    window: Window,
    state: tauri::State<ProjectState>,
) -> Result<Vec<NamedStages>, &'static str> {
    let mut state_guard = state.0.lock().unwrap();

    let mut named_stages: Vec<NamedStages> = Vec::new();
    if let Some(proj) = &(*state_guard) {
        let types = proj.data_set.as_ref().unwrap();
        let stg = &types.stages;
        let sys = &types.systems;
        if let Some(scene) = &proj.current_scene {
            for stage in scene.systems.iter() {
                let mut named_systems: Vec<String> = Vec::new();
                for system in stage.systems.iter() {
                    match sys.get(system) {
                        Some(val) => named_systems.push(val.clone()),
                        None => panic!("System definition not found!"),
                    }
                }
                match stg.get(&stage.stage) {
                    Some(val) => named_stages.push(NamedStages {
                        stage: val.clone(),
                        systems: named_systems,
                    }),
                    None => panic!("Stage definition not found!"),
                }
            }
            Ok(named_stages)
        } else {
            Result::Err("No active scene")
        }
    } else {
        Result::Err("No active project")
    }
}
