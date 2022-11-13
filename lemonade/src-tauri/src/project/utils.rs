pub fn hash_string(val: &str) -> Option<u32> {
    if val.len() == 0 {
        return None;
    }

    let mut hash = 5381u32;

    for c in val.as_bytes() {
        hash = hash
            .wrapping_shl(5)
            .wrapping_add(hash)
            .wrapping_add(u32::from(*c));
    }

    Some(hash)
}
