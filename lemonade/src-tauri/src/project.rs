mod component;
mod entity;
mod error_codes;
mod files;
mod models;
mod utils;

use serde::Serialize;
use serde_json::{json, Value};
use std::collections::HashMap;
use std::io::{BufRead, BufReader};
use std::path::PathBuf;
use std::process::{Child, Command, Stdio};
use std::sync::MutexGuard;
use std::thread::{self, JoinHandle};
use std::{fs, path::Path, result::Result, str, sync::Mutex};
use tauri::Window;

use self::component::{
    add_component, change_component, get_entity_components, remove_entity_component,
    EntityComponents,
};
use self::entity::{add_new_entity, get_all_entities, remove_one_entity, EntityDTO};
use self::files::{read_assets_file, read_project_file, read_scene_file, read_types_file};
use self::models::*;
use error_codes::ProjectErrorCode;
use ndarray::arr2;
use utils::hash_string;

use imagesize::size;

pub struct EngineProcess {
    child: Option<Child>,
    handle: Option<JoinHandle<()>>,
}

pub struct ProjectState(pub Mutex<Option<Project>>);
pub struct Engine(pub Mutex<Option<EngineProcess>>);

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

fn generate_assets(assets: &Assets) -> Result<AssetLookup, ProjectErrorCode> {
    let mut asset_lookup: HashMap<u32, String> = HashMap::new();
    for a in &assets.textures {
        let hashed = hash_string(&a.name);
        if let Some(h) = hashed {
            asset_lookup.insert(h, a.path.clone());
        } else {
            println!("Failed to hash and insert asset: {}", &a.name);
        }
    }

    for a in &assets.sounds {
        let hashed = hash_string(&a.name);
        if let Some(h) = hashed {
            asset_lookup.insert(h, a.path.clone());
        } else {
            println!("Failed to hash and insert asset: {}", &a.name);
        }
    }

    for a in &assets.shaders {
        let hashed = hash_string(&a.name);
        if let Some(h) = hashed {
            asset_lookup.insert(h, a.path.clone());
        } else {
            println!("Failed to hash and insert asset: {}", &a.name);
        }
    }

    for a in &assets.scripts {
        let hashed = hash_string(&a.name);
        if let Some(h) = hashed {
            asset_lookup.insert(h, a.path.clone());
        } else {
            println!("Failed to hash and insert asset: {}", &a.name);
        }
    }

    for a in &assets.animations {
        let hashed = hash_string(&a.name);
        if let Some(h) = hashed {
            asset_lookup.insert(h, a.path.clone());
        } else {
            println!("Failed to hash and insert asset: {}", &a.name);
        }
    }

    Ok(AssetLookup {
        assets: asset_lookup,
    })
}

fn open_project_impl(
    window: Window,
    mut state_guard: MutexGuard<Option<Project>>,
    path: &str,
) -> Result<String, &'static str> {
    match read_project_file(path) {
        Ok(p) => {
            *state_guard = Some(p);
        }
        Err(e) => println!("Error parsing the project file: {}", e.to_string()),
    };

    if let Some(project) = &mut (*state_guard) {
        let parent = Path::new(path).parent();
        project.path = parent.unwrap().to_path_buf();
        let scenes_path = parent.unwrap().join(&project.scene_path);
        let scene_name = project.scenes.first().unwrap().to_owned();

        let scene_path = scenes_path.join(scene_name.clone() + ".json");

        match read_scene_file(scene_path.to_str().unwrap()) {
            Ok(mut s) => {
                s.name = scene_name;
                project.current_scene = Some(s)
            }
            Err(e) => println!("{}", e.to_string()),
        };

        let types = parent.unwrap().join(&project.types_path);
        project.executable = Some(parent.unwrap().join(&project.exec_path));

        match read_types_file(types.to_str().unwrap()) {
            Ok(t) => project.data_set = Some(t),
            Err(e) => println!("{}", e.to_string()),
        };

        let assets = parent
            .unwrap()
            .join(&project.assets_path)
            .join("assets.json");

        match read_assets_file(assets.to_str().unwrap()) {
            Ok(a) => project.assets = Some(a),
            Err(e) => println!("{}", e.to_string()),
        };

        if let Some(a) = &project.assets {
            project.asset_lookup = generate_assets(a).ok();
        }

        _ = window.emit("project-opened", "");

        Ok(project.project_name.clone())
    } else {
        Err("Error occured while reading the file")
    }
}

#[tauri::command]
pub fn open_project(
    window: Window,
    state: tauri::State<ProjectState>,
    path: &str,
) -> Result<String, &'static str> {
    let mut state_guard = lock_state!(state);
    open_project_impl(window, state_guard, path)
}
#[tauri::command]
pub fn get_components(state: tauri::State<ProjectState>) -> Result<Vec<String>, ProjectErrorCode> {
    let state_guard = state.0.lock().unwrap();

    let project = unwrap_or_return!(get_project(&(*state_guard)));
    let dataset = unwrap_or_return!(get_dataset(project));

    Ok(dataset.components.keys().cloned().collect::<Vec<String>>())
}

#[tauri::command]
pub fn get_components_for_entity(
    state: tauri::State<ProjectState>,
    entityid: u32,
) -> Result<EntityComponents, ProjectErrorCode> {
    let state_guard = state.0.lock().unwrap();

    let project = unwrap_or_return!(get_project(&(*state_guard)));
    let scene = unwrap_or_return!(get_scene(project));
    Ok(get_entity_components(scene, entityid))
}

#[tauri::command]
pub fn get_entities(state: tauri::State<ProjectState>) -> Result<Vec<EntityDTO>, ProjectErrorCode> {
    let state_guard = lock_state!(state);

    let project = unwrap_or_return!(get_project(&(*state_guard)));
    let scene = unwrap_or_return!(get_scene(project));

    Ok(get_all_entities(scene))
}

#[tauri::command]
pub fn get_debug_data(
    state: tauri::State<ProjectState>,
) -> Result<Vec<DebugData>, ProjectErrorCode> {
    let state_guard = lock_state!(state);

    let Some(project) = &(*state_guard) else {
        return Err(ProjectErrorCode::NoProjectLoaded);
    };

    let Some(scene) = &project.current_scene else {
        return Err(ProjectErrorCode::NoSceneOpened);
    };

    let transforms = (&scene.components).iter().find(|x| x.name == "transform");
    let Some(transform_entities_m) = transforms.as_ref() else {
        return Err(ProjectErrorCode::NoEntityFound);
    };
    let transform_entities = &transform_entities_m.entities;

    let colliders = (&scene.components)
        .iter()
        .find(|x| x.name == "box_collider");
    let Some(collider_entities_m) = colliders.as_ref() else {
        return Err(ProjectErrorCode::NoEntityFound);
    };
    let collider_entities = &collider_entities_m.entities;

    Ok(collider_entities
        .into_iter()
        .filter_map(|(key, value)| {
            transform_entities.get(key).map(|value_b| {
                let position = value_b["position"]
                    .as_array()
                    .unwrap()
                    .into_iter()
                    .map(|x| x.as_f64().unwrap() as f32)
                    .collect::<Vec<f32>>();

                let h_size = value["hSize"]
                    .as_array()
                    .unwrap()
                    .into_iter()
                    .map(|x| x.as_f64().unwrap() as f32)
                    .collect::<Vec<f32>>();

                let offset = value["offset"]
                    .as_array()
                    .unwrap()
                    .into_iter()
                    .map(|x| x.as_f64().unwrap() as f32)
                    .collect::<Vec<f32>>();

                let center = &[position[0] + offset[0], position[1] + offset[1]];

                DebugData {
                    coords: [
                        center[0] - h_size[0],
                        center[1] - h_size[1],
                        center[0] + h_size[0],
                        center[1] - h_size[1],
                        center[0] + h_size[0],
                        center[1] + h_size[1],
                        center[0] - h_size[0],
                        center[1] + h_size[1],
                        center[0] - h_size[0],
                        center[1] - h_size[1],
                    ]
                    .to_vec(),
                }
            })
        })
        .collect::<Vec<DebugData>>())
}

#[tauri::command]
pub fn get_rendering_data(
    state: tauri::State<ProjectState>,
) -> Result<Vec<RenderingData>, ProjectErrorCode> {
    let state_guard = lock_state!(state);

    let project = unwrap_or_return!(get_project(&(*state_guard)));
    let scene = unwrap_or_return!(get_scene(project));

    let transforms = (&scene.components).iter().find(|x| x.name == "transform");

    let Some(transform_entities_m) = transforms.as_ref() else {
        return Err(ProjectErrorCode::NoEntityFound);
    };
    let transform_entities = &transform_entities_m.entities;

    let sprite_renderers = (&scene.components)
        .iter()
        .find(|x| x.name == "sprite_renderer");

    let Some(rendering_entities_m) = sprite_renderers.as_ref() else {
        return Err(ProjectErrorCode::NoEntityFound);
    };
    let rendering_entities = &rendering_entities_m.entities;

    Ok(rendering_entities
        .into_iter()
        .filter_map(|(key, value)| {
            transform_entities.get(key).map(|value_b| {
                let position = value_b["position"]
                    .as_array()
                    .unwrap()
                    .into_iter()
                    .map(|x| x.as_f64().unwrap() as f32)
                    .collect::<Vec<f32>>();
                let layer = value_b["layer"].as_u64().unwrap();

                let rotation = value_b["rotation"].as_f64().unwrap() as f32;
                let scale = value_b["scale"]
                    .as_array()
                    .unwrap()
                    .into_iter()
                    .map(|x| x.as_f64().unwrap() as f32)
                    .collect::<Vec<f32>>();

                let translation_matrix = arr2(&[
                    [1f32, 0f32, 0f32, 0f32],
                    [0f32, 1f32, 0f32, 0f32],
                    [0f32, 0f32, 1f32, 0f32],
                    [position[0], position[1], layer as f32 * 0.1f32, 1f32],
                ]);

                let scale_matrix = arr2(&[
                    [scale[0], 0f32, 0f32, 0f32],
                    [0f32, scale[1], 0f32, 0f32],
                    [0f32, 0f32, 1f32, 0f32],
                    [0f32, 0f32, 0f32, 1f32],
                ]);

                let rotation_matrix = arr2(&[
                    [rotation.cos(), -rotation.sin(), 0f32, 0f32],
                    [rotation.sin(), rotation.cos(), 0f32, 0f32],
                    [0f32, 0f32, 1f32, 0f32],
                    [0f32, 0f32, 0f32, 1f32],
                ]);

                let model = translation_matrix
                    .dot(&rotation_matrix)
                    .dot(&scale_matrix)
                    .into_iter()
                    .collect::<Vec<f32>>();

                let tex_coords = value["texCoords"]
                    .as_array()
                    .unwrap()
                    .into_iter()
                    .map(|x| x.as_f64().unwrap() as f32)
                    .collect::<Vec<f32>>();

                let hashed = hash_string(value["tex"].as_str().unwrap());

                if let Some(textureid) = hashed {
                    RenderingData {
                        nameid: *key,
                        model,
                        textureid,
                        tex_coords,
                    }
                } else {
                    RenderingData {
                        nameid: *key,
                        model,
                        textureid: 0,
                        tex_coords,
                    }
                }
            })
        })
        .collect::<Vec<RenderingData>>())
}

#[derive(Serialize)]
pub struct TextureDTO {
    pub path: String,
    pub width: usize,
    pub height: usize,
}

#[tauri::command]
pub fn get_asset_list(
    state: tauri::State<ProjectState>,
    ids: Vec<u32>,
) -> Result<HashMap<u32, TextureDTO>, ProjectErrorCode> {
    let state_guard = lock_state!(state);

    let project = unwrap_or_return!(get_project(&(*state_guard)));

    match &project.asset_lookup {
        Some(assets) => Ok(ids
            .into_iter()
            .filter_map(|id| {
                assets.assets.get(&id).map(|val| match size(val.clone()) {
                    Ok(dim) => (
                        id,
                        TextureDTO {
                            path: val.clone(),
                            width: dim.width,
                            height: dim.height,
                        },
                    ),
                    Err(_) => (
                        id,
                        TextureDTO {
                            path: val.clone(),
                            width: 0usize,
                            height: 0usize,
                        },
                    ),
                })
            })
            .collect::<HashMap<u32, TextureDTO>>()),
        None => Err(ProjectErrorCode::NoAssetsLoaded),
    }
}

#[tauri::command]
pub fn add_component_to_entity(
    state: tauri::State<ProjectState>,
    entityid: u32,
    componentname: String,
) -> Result<EntityComponents, ProjectErrorCode> {
    let mut state_guard = state.0.lock().unwrap();

    let Some(project) = &mut (*state_guard) else {
        return Err(ProjectErrorCode::NoProjectLoaded);
    };

    let Some(data) = &project.data_set else {
        return Err(ProjectErrorCode::NoDatasetLoaded);
    };

    let Some(scene) = &mut project.current_scene else {
        return Err(ProjectErrorCode::NoSceneOpened);
    };

    match add_component(scene, data, entityid, componentname) {
        Ok(()) => Ok(get_entity_components(scene, entityid)),
        Err(err) => Err(err),
    }
}

#[tauri::command]
pub fn remove_component_from_entity(
    state: tauri::State<ProjectState>,
    entityid: u32,
    componentname: String,
) -> Result<EntityComponents, ProjectErrorCode> {
    let mut state_guard = state.0.lock().unwrap();

    let Some(project) = &mut (*state_guard) else {
        return Err(ProjectErrorCode::NoProjectLoaded);
    };

    let Some(scene) = &mut project.current_scene else {
        return Err(ProjectErrorCode::NoSceneOpened);
    };
    remove_entity_component(scene, entityid, componentname)?;
    Ok(get_entity_components(scene, entityid))
}

#[tauri::command]
pub fn add_entity(state: tauri::State<ProjectState>) -> Result<Vec<EntityDTO>, ProjectErrorCode> {
    let mut state_guard = state.0.lock().unwrap();

    let Some(project) = &mut (*state_guard) else {
        return Err(ProjectErrorCode::NoProjectLoaded);
    };

    let Some(scene) = &mut project.current_scene else {
        return Err(ProjectErrorCode::NoSceneOpened);
    };
    add_new_entity(scene);
    Ok(get_all_entities(scene))
}

#[tauri::command]
pub fn set_entity_name(
    state: tauri::State<ProjectState>,
    entityid: u32,
    name: String,
) -> Result<String, ProjectErrorCode> {
    let mut state_guard = state.0.lock().unwrap();

    let Some(project) = &mut (*state_guard) else {
        return Err(ProjectErrorCode::NoProjectLoaded);
    };
    let Some(scene) = &mut project.current_scene else {
        return Err(ProjectErrorCode::NoSceneOpened);
    };

    let components = &mut scene.components;
    if let Some(c) = components.iter().position(|x| x.name == "tag") {
        let entity = components[c].entities.get_mut(&entityid).unwrap();
        *entity.get_mut("name").unwrap() = json!(name);
    }

    Ok(name)
}

#[tauri::command]
pub fn change_entity_component(
    state: tauri::State<ProjectState>,
    window: Window,
    entityid: u32,
    componentname: String,
    fieldname: String,
    newvalue: Value,
) -> Result<(), ProjectErrorCode> {
    let mut state_guard = state.0.lock().unwrap();

    let Some(project) = &mut (*state_guard) else {
        return Err(ProjectErrorCode::NoProjectLoaded);
    };

    let Some(scene) = &mut project.current_scene else {
        return Err(ProjectErrorCode::NoSceneOpened);
    };

    change_component(scene, entityid, componentname, fieldname, newvalue)?;
    window
        .emit("scene-changed", ())
        .expect("Failed to emit event on scene changed");

    Ok(())
}

#[tauri::command]
pub fn run_engine(
    state: tauri::State<ProjectState>,
    window: Window,
    engine_state: tauri::State<Engine>,
) -> Result<(), ProjectErrorCode> {
    let state_guard = lock_state!(state);
    let Some(project) = &(*state_guard) else {
        return Err(ProjectErrorCode::NoProjectLoaded);
    };

    let mut engine_state_guard = engine_state.0.lock().unwrap();

    if let Some(engine) = &mut *engine_state_guard {
        if let Some(child) = &mut engine.child {
            match child.try_wait() {
                Ok(Some(status)) => println!("exited with {status}"),
                Ok(None) => match child.kill() {
                    Err(_err) => return Err(ProjectErrorCode::FailedToStartEngine),
                    _ => (),
                },
                Err(_) => {
                    return Err(ProjectErrorCode::FailedToStartEngine);
                }
            }
        }
        if let Some(thread_join_handle) = engine.handle.take() {
            thread_join_handle.join().unwrap();
        }
    }

    let mut child = Command::new(project.executable.as_ref().unwrap())
        .stdout(Stdio::piped())
        .stderr(Stdio::inherit())
        .spawn()
        .expect("failed to execute child");

    let out = BufReader::new(child.stdout.take().unwrap());

    let t = thread::Builder::new()
        .name("engine_stream_to_editor".into())
        .spawn(move || {
            out.lines().for_each(|line| {
                _ = window.emit("received-output", line.unwrap());
                ()
            });
        });
    match t {
        Ok(thread) => {
            *engine_state_guard = Some(EngineProcess {
                child: Some(child),
                handle: Some(thread),
            });
            Ok(())
        }
        Err(_err) => Err(ProjectErrorCode::FailedToStartEngine),
    }
}

#[tauri::command]
pub fn stop_engine(engine_state: tauri::State<Engine>) -> Result<(), ProjectErrorCode> {
    let mut engine_state_guard = engine_state.0.lock().unwrap();

    let Some(engine) = &mut *engine_state_guard else {
        return Err(ProjectErrorCode::EngineNotRunning);
    };

    if let Some(child) = &mut engine.child {
        match child.kill() {
            Ok(_) => (),
            Err(_err) => return Err(ProjectErrorCode::EngineNotRunning),
        }
    }
    if let Some(thread_join_handle) = engine.handle.take() {
        thread_join_handle.join().unwrap();
    }
    Ok(())
}

#[tauri::command]
pub fn get_scenes(state: tauri::State<ProjectState>) -> Result<Vec<String>, ProjectErrorCode> {
    let state_guard = lock_state!(state);

    let Some(project) = &(*state_guard) else {
        return Err(ProjectErrorCode::NoProjectLoaded);
    };

    Ok(project.scenes.clone())
}

#[tauri::command]
pub fn get_textures(state: tauri::State<ProjectState>) -> Result<Vec<Asset>, ProjectErrorCode> {
    let state_guard = lock_state!(state);

    let Some(project) = &(*state_guard) else {
        return Err(ProjectErrorCode::NoProjectLoaded);
    };

    let Some(assets) = &project.assets else {
        return Err(ProjectErrorCode::NoAssetsLoaded);
    };

    Ok(assets.textures.clone())
}

#[tauri::command]
pub fn get_sounds(state: tauri::State<ProjectState>) -> Result<Vec<Asset>, ProjectErrorCode> {
    let state_guard = lock_state!(state);

    let Some(project) = &(*state_guard) else {
        return Err(ProjectErrorCode::NoProjectLoaded);
    };

    let Some(assets) = &project.assets else {
        return Err(ProjectErrorCode::NoAssetsLoaded);
    };

    Ok(assets.sounds.clone())
}
#[tauri::command]
pub fn get_scripts(state: tauri::State<ProjectState>) -> Result<Vec<Asset>, ProjectErrorCode> {
    let state_guard = lock_state!(state);

    let Some(project) = &(*state_guard) else {
        return Err(ProjectErrorCode::NoProjectLoaded);
    };

    let Some(assets) = &project.assets else {
        return Err(ProjectErrorCode::NoAssetsLoaded);
    };

    Ok(assets.scripts.clone())
}

#[tauri::command]
pub fn get_animations(state: tauri::State<ProjectState>) -> Result<Vec<Asset>, ProjectErrorCode> {
    let state_guard = lock_state!(state);

    let Some(project) = &(*state_guard) else {
        return Err(ProjectErrorCode::NoProjectLoaded);
    };

    let Some(assets) = &project.assets else {
        return Err(ProjectErrorCode::NoAssetsLoaded);
    };

    Ok(assets.animations.clone())
}

#[tauri::command]
pub fn get_shaders(state: tauri::State<ProjectState>) -> Result<Vec<Asset>, ProjectErrorCode> {
    let state_guard = lock_state!(state);

    let Some(project) = &(*state_guard) else {
        return Err(ProjectErrorCode::NoProjectLoaded);
    };

    let Some(assets) = &project.assets else {
        return Err(ProjectErrorCode::NoAssetsLoaded);
    };

    Ok(assets.shaders.clone())
}

#[tauri::command]
pub fn save(state: tauri::State<ProjectState>) -> Result<(), ProjectErrorCode> {
    let mut state_guard = lock_state!(state);

    let Some(project) = &mut (*state_guard) else {
        return Err(ProjectErrorCode::NoProjectLoaded);
    };

    let Some(scene) = &mut project.current_scene else {
        return Err(ProjectErrorCode::NoSceneOpened);
    };

    let slice = serde_json::to_vec(&scene);
    let path = project
        .path
        .join(&project.scene_path)
        .join(scene.name.clone() + ".json");

    fs::write(path, slice.unwrap()).expect("Failed to save scene to file");
    Ok(())
}

#[tauri::command]
pub fn recreate_assets(
    state: tauri::State<ProjectState>,
    handle: tauri::AppHandle,
) -> Result<(), ProjectErrorCode> {
    let state_guard = lock_state!(state);

    let Some(project) = &*state_guard else {
        return Err(ProjectErrorCode::NoProjectLoaded);
    };
    let assets = &project.assets_path;
    let path = &project.path;

    let asset_path = path.join(assets);
    let tool_path = handle.path_resolver()
                .resolve_resource("tools/recreate_assets.py")
                .expect("failed to create project");

    //let tool_path = PathBuf::try_from(env!("CARGO_MANIFEST_DIR"))
    //    .unwrap()
    //    .parent()
    //    .unwrap()
    //    .join("tools/recreate_assets.py");

    let python = if cfg!(windows) { "py.exe" } else { "python3" };

    Command::new(python)
        .arg(tool_path)
        .arg(asset_path)
        .spawn()
        .expect("Python script failed");

    Ok(())
}

#[tauri::command]
pub fn remove_entity(
    state: tauri::State<ProjectState>,
    window: Window,
    entityid: u32,
) -> Result<Vec<EntityDTO>, ProjectErrorCode> {
    let mut state_guard = lock_state!(state);

    let Some(project) = &mut *state_guard else  {
        return Err(ProjectErrorCode::NoProjectLoaded);
    };

    let Some(scene) = &mut project.current_scene else {
        return Err(ProjectErrorCode::NoSceneOpened);
    };

    remove_one_entity(scene, entityid)?;

    _ = window.emit("scene-changed", ());

    Ok(get_all_entities(scene))
}

#[tauri::command]
pub fn get_settings(
    state: tauri::State<ProjectState>,
) -> Result<ProjectSettings, ProjectErrorCode> {
    let state_guard = lock_state!(state);

    let Some(project) = &*state_guard else {
        return Err(ProjectErrorCode::NoProjectLoaded);
    };

    Ok(ProjectSettings {
        project_name: project.project_name.clone(),
        assets_path: project.assets_path.clone(),
        scene_path: project.scene_path.clone(),
        src_path: project.src_path.clone(),
        types_path: project.types_path.clone(),
        exec_path: project.exec_path.clone(),
        scenes: project.scenes.clone(),
    })
}

#[tauri::command]
pub fn set_settings(
    state: tauri::State<ProjectState>,
    window: Window,
    project_settings: ProjectSettings,
) -> Result<String, &'static str> {
    let mut state_guard = lock_state!(state);

    let Some(project) = &mut *state_guard else {
        return Err("No project loaded");
    };

    let settings_path = project
        .path
        .join(project.project_name.to_lowercase() + ".lmnproj");

    std::fs::write(
        settings_path.clone(),
        serde_json::to_string_pretty(&project_settings).unwrap(),
    )
    .unwrap();

    open_project_impl(window, state_guard, settings_path.to_str().unwrap())
}

#[tauri::command]
pub fn create_project(
    state: tauri::State<ProjectState>,
    handle: tauri::AppHandle,
    window: Window,
    path: String,
) -> Result<String, &'static str> {
    let mut state_guard = lock_state!(state);

    let project_name = Path::new(&path).file_name().unwrap().to_str().unwrap();
    let tool_path = handle.path_resolver()
                .resolve_resource("tools/create_project.py")
                .expect("failed to create project");

    //let tool_path = PathBuf::try_from(env!("CARGO_MANIFEST_DIR"))
    //    .unwrap()
    //    .parent()
    //    .unwrap()
    //    .join("tools/create_project.py");

    let python = if cfg!(windows) { "py.exe" } else { "python3" };

    Command::new(python)
        .arg(tool_path)
        .arg(project_name)
        .arg(path.clone())
        .spawn()
        .expect("Python script failed")
        .wait()
        .unwrap();

    let mut project_file_path = Path::new(&path).join(project_name);
    project_file_path.set_extension("lmnproj");

    open_project_impl(window, state_guard, project_file_path.to_str().unwrap())
}
