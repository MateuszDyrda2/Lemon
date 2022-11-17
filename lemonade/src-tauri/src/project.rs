mod error_codes;
mod models;
mod utils;

use serde::{Deserialize, Serialize};
use serde_json::{from_str, json, Value};
use std::collections::HashMap;
use std::{fs, path::Path, result::Result, sync::Mutex};
use tauri::Window;

use self::models::*;
use error_codes::ProjectErrorCode;
use ndarray::arr2;
use utils::hash_string;

use imagesize::size;

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

fn read_assets_file(path: &str) -> Result<Assets, serde_json::error::Error> {
    let contents = fs::read_to_string(path).expect("Failed to read assets file");

    let assets: Assets = from_str(&contents)?;
    Ok(assets)
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

fn get_assets(project: &Project) -> Result<&Assets, ProjectErrorCode> {
    project
        .assets
        .as_ref()
        .ok_or(ProjectErrorCode::NoAssetsLoaded)
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
    };

    if let Some(project) = &mut (*state_guard) {
        let parent = Path::new(path).parent();
        let scenes_path = parent.unwrap().join(&project.scene_path);
        let scene_name = project.scenes.first().unwrap().to_owned();
        let scene_path = scenes_path.join(scene_name + ".json");

        match read_scene_file(scene_path.to_str().unwrap()) {
            Ok(s) => project.current_scene = Some(s),
            Err(e) => println!("{}", e.to_string()),
        };

        let types = parent
            .unwrap()
            .join(project.build_dir.to_string() + "/_generated/types.json");

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
    entityid: u32,
) -> Result<HashMap<String, HashMap<String, Value>>, ProjectErrorCode> {
    let state_guard = state.0.lock().unwrap();

    let project = unwrap_or_return!(get_project(&(*state_guard)));
    let scene = unwrap_or_return!(get_scene(project));

    Ok((&scene.components)
        .iter()
        .filter(|c| c.entities.contains_key(&entityid))
        .map(|c| (c.name.clone(), c.entities.get(&entityid).unwrap().clone()))
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

#[tauri::command]
pub fn get_rendering_data(
    _window: Window,
    state: tauri::State<ProjectState>,
) -> Result<Vec<RenderingData>, ProjectErrorCode> {
    let state_guard = lock_state!(state);

    let project = unwrap_or_return!(get_project(&(*state_guard)));
    let scene = unwrap_or_return!(get_scene(project));

    let transforms = (&scene.components).iter().find(|x| x.name == "transform");
    let transform_entities = &transforms.as_ref().unwrap().entities;
    let sprite_renderers = (&scene.components)
        .iter()
        .find(|x| x.name == "sprite_renderer");
    let rendering_entities = &sprite_renderers.as_ref().unwrap().entities;

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
                    [position[0], position[1], 0f32, 1f32],
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

                RenderingData {
                    nameid: *key,
                    model,
                    textureid: value["tex"].as_u64().unwrap() as u32,
                    texCoords: tex_coords,
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
                    Err(err) => (
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

fn field_to_value(ftype: &FieldType) -> Value {
    match ftype {
        FieldType::Bool => json!(false),
        FieldType::I8
        | FieldType::I16
        | FieldType::I32
        | FieldType::I64
        | FieldType::U8
        | FieldType::U16
        | FieldType::U32
        | FieldType::U64
        | FieldType::SizeT
        | FieldType::HashStr
        | FieldType::Asset
        | FieldType::Entity
        | FieldType::Unknown => json!(0),
        FieldType::F32 | FieldType::F64 => json!(0.0f64),
        FieldType::IVec2 | FieldType::UVec2 | FieldType::Vec2 => json!([0, 0]),
        FieldType::IVec3 | FieldType::UVec3 | FieldType::Vec3 => json!([0, 0, 0]),
        FieldType::IVec4 | FieldType::UVec4 | FieldType::Vec4 => json!([0, 0, 0, 0]),
        FieldType::Mat2 => json!([[0, 0], [0, 0]]),
        FieldType::Mat3 => json!([[0, 0, 0], [0, 0, 0], [0, 0, 0]]),
        FieldType::Mat4 => json!([[0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0]]),
        FieldType::Str => json!(""),
    }
}

#[tauri::command]
pub fn add_component_to_entity(
    state: tauri::State<ProjectState>,
    entityid: u32,
    componentname: String,
) -> Result<(), ProjectErrorCode> {
    let mut state_guard = state.0.lock().unwrap();

    if let Some(project) = &mut (*state_guard) {
        if let Some(data) = &project.data_set {
            if let Some(scene) = &mut project.current_scene {
                if let Some(found) = data.components.get(&componentname) {
                    let md = found
                        .iter()
                        .map(|x| (x.name.clone(), field_to_value(&x._type)))
                        .collect::<HashMap<String, Value>>();

                    let components = &mut scene.components;
                    if let Some(c) = components.iter().position(|x| x.name == componentname) {
                        components[c].entities.insert(entityid, md);
                    }
                }
                Ok(())
            } else {
                Err(ProjectErrorCode::NoSceneOpened)
            }
        } else {
            Err(ProjectErrorCode::NoDatasetLoaded)
        }
    } else {
        Err(ProjectErrorCode::NoProjectLoaded)
    }
}
