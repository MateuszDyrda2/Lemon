use serde::{Deserialize, Serialize};

#[derive(Serialize, Deserialize, Debug)]
pub enum ProjectErrorCode {
    NoProjectLoaded,
    NoSceneOpened,
    NoDatasetLoaded,
    NoMatchedStage,
    NoMatchedSystem,
}
