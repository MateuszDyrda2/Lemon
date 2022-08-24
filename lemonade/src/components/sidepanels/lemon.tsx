import "../../styles/sidepanels/lemon.scss";
import LemonIcon from "../../img/lemon.svg";
import { open } from "@tauri-apps/api/dialog";
import { emit, listen } from "@tauri-apps/api/event";
import { appWindow } from "@tauri-apps/api/window";
import React from "react";
import { Project, UserContext } from "../../props/project";

const Lemon = () => {
  const proj = React.useContext<Project | undefined>(UserContext);

  const openProject = async () => {
    const selected = await open({
      multiple: false,
      filters: [
        {
          name: "Lemon Project",
          extensions: ["lmnproj"],
        },
      ],
    });
    selected && appWindow.emit("open-project", selected);
  };

  return (
    <div className="lemon">
      <div className="button-group">
        <img src={LemonIcon} id="lemon-icon" alt="logo of the game engine" />
        {proj && <p>Project: {proj.project_name}</p>}
        <button>New project</button>
        <button onClick={openProject}>Open project</button>
        <button>Save</button>
        <button>Save as</button>
      </div>
    </div>
  );
};

export default Lemon;
