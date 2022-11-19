use serde::{Deserialize, Serialize};

#[derive(Serialize, Deserialize, Debug)]
pub enum ProjectErrorCode {
    NoProjectLoaded,
    NoSceneOpened,
    NoDatasetLoaded,
    NoAssetsLoaded,
    NoEntityFound,
    EntityDoesNotHaveCompoonent,
    NoComponentFound,
    NoComponentDefinitionFound,
    EngineNotRunning,
    FailedToStartEngine,
}
