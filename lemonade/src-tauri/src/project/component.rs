use std::collections::HashMap;

use serde::Serialize;
use serde_json::Value;

use super::{
    error_codes::ProjectErrorCode,
    models::{Component, Scene, Types},
    utils::{field_to_value, hash_string},
};

#[derive(Serialize, Debug)]
pub struct EntityComponents {
    pub components: HashMap<String, HashMap<String, Value>>,
    pub name: String,
}

pub fn get_entity_components(scene: &Scene, entityid: u32) -> EntityComponents {
    let components = &scene.components;

    let c = components
        .iter()
        .filter(|c| c.entities.contains_key(&entityid) && c.name != "tag")
        .map(|c| (c.name.clone(), c.entities.get(&entityid).unwrap().clone()))
        .collect::<HashMap<String, HashMap<String, Value>>>();

    let names = components.iter().find(|x| x.name == "tag").unwrap();
    let name = names
        .entities
        .get(&entityid)
        .unwrap()
        .get("name")
        .unwrap()
        .as_str()
        .unwrap();

    EntityComponents {
        components: c,
        name: String::from(name),
    }
}

pub fn add_component(
    scene: &mut Scene,
    data: &Types,
    entityid: u32,
    componentname: String,
) -> Result<(), ProjectErrorCode> {
    let Some(found) = data.components.get(&componentname) else {
        return Err(ProjectErrorCode::NoComponentDefinitionFound);
    };

    let md = found
        .iter()
        .map(|x| (x.name.clone(), field_to_value(&x._type)))
        .collect::<HashMap<String, Value>>();

    let components = &mut scene.components;

    if let Some(c) = components.iter().position(|x| x.name == componentname) {
        components[c].entities.entry(entityid).or_insert_with(|| md);
        components[c].count += 1;
    } else {
        components.push(Component {
            name: componentname.clone(),
            id: hash_string(&componentname).unwrap() as u64,
            count: 1,
            entities: HashMap::from([(entityid, md)]),
        });
    }

    Ok(())
}

pub fn remove_entity_component(
    scene: &mut Scene,
    entityid: u32,
    componentname: String,
) -> Result<(), ProjectErrorCode> {
    let components = &mut scene.components;

    if let Some(c) = components.iter().position(|x| x.name == componentname) {
        components[c].entities.remove(&entityid);
        Ok(())
    } else {
        Err(ProjectErrorCode::NoComponentFound)
    }
}

pub fn change_component(
    scene: &mut Scene,
    entityid: u32,
    componentname: String,
    fieldname: String,
    newvalue: Value,
) -> Result<(), ProjectErrorCode> {
    let components = &mut scene.components;

    let Some(c) = components.iter().position(|x| x.name == componentname) else {
        return Err(ProjectErrorCode::NoComponentFound);
    };

    let Some(entity) = components[c].entities.get_mut(&entityid) else {
        return Err(ProjectErrorCode::EntityDoesNotHaveCompoonent) ;
    };
    let component = entity.get_mut(&fieldname).unwrap();

    *component = newvalue;
    Ok(())
}
