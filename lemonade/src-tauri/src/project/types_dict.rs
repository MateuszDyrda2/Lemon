use serde::Deserialize;
use serde_json::{from_str, Result};
use std::collections::HashMap;
use std::fs;

#[derive(Deserialize, Debug)]
pub struct Types {
    pub stages: HashMap<u32, String>,
    pub systems: HashMap<u32, String>,
    pub components: HashMap<String, Vec<String>>,
}

fn read_types_file(path: &str) -> Result<Types> {
    let contents = fs::read_to_string(path).expect("Failed to read types file");
    let types: Types = from_str(&contents)?;
    Ok(types)
}

pub fn read_types(path: &str) -> Result<Types> {
    read_types_file(path)
}
