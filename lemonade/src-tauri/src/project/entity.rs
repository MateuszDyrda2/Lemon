use super::{error_codes::ProjectErrorCode, models::{Scene, Component}, utils::hash_string};

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
    
    let Some(entities_m) = names.as_ref() else {
        return Vec::new();
    };
    let entities = &entities_m.entities;

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
        components[c].count += 1;
        components[c]
            .entities
            .entry(new_id)
            .or_insert_with(|| HashMap::from([(String::from("name"), json!("scene_object"))]));
    } else {
        components.push(Component {
            name: "tag".to_owned(),
            id: hash_string("tag").unwrap() as u64,
            count: 1,
            entities: HashMap::from([(new_id, HashMap::from([(String::from("name"), json!("scene_object"))]))])
        });
    }
}

pub fn remove_one_entity(scene: &mut Scene, entityid: u32) -> Result<(), ProjectErrorCode> {
    let entities = &mut scene.entities;

    let Some(pos) = entities.ids.iter().position(|x| *x == entityid) else {
        return Err(ProjectErrorCode::NoEntityFound);
    };

    entities.ids.remove(pos);
    entities.count -= 1;

    let components = &mut scene.components;
    components.iter_mut().for_each(|x| {
        x.entities.remove(&entityid);
        ()
    });

    Ok(())
}
