use super::models::Scene;
use super::utils::hash_string;

use serde::{Deserialize, Serialize};
use serde_json::json;
use std::collections::HashMap;

#[derive(Serialize, Deserialize, Debug)]
pub struct EntityDTO {
    id: u32,
    name: String,
}

pub fn get_all_entities(scene: &Scene) -> Vec<EntityDTO> {
    let names = (&scene.components).iter().find(|x| x.name == "tag");
    let entities = &names.as_ref().unwrap().entities;

    entities
        .iter()
        .map(|(key, val)| EntityDTO {
            id: *key,
            name: val.get("name").unwrap().as_str().unwrap().to_string(),
        })
        .collect::<Vec<EntityDTO>>()
}

pub fn add_new_entity(scene: &mut Scene) {
    let entities = &mut scene.entities;
    let new_id = entities.count;
    entities.count += 1;
    entities.ids.insert(0, new_id);

    let components = &mut scene.components;
    if let Some(c) = components.iter().position(|x| x.name == "tag") {
        components[c].entities.entry(new_id).or_insert_with(|| {
            HashMap::from([
                (String::from("name"), json!("scene_object")),
                (String::from("id"), json!(hash_string("scene_object"))),
            ])
        });
    }
}
