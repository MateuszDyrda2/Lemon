import React from "react";

export interface Project {
  project_name: String;
  assets_path: String;
  scene_path: String;
  src_path: String;
  build_dir: String;
  scenes: String[];
}

export const UserContext = React.createContext<Project | undefined>(undefined);
