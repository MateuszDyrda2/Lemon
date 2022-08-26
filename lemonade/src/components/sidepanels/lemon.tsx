import "../../styles/sidepanels/lemon.scss";
import LemonIcon from "../../img/lemon.svg";
import { open } from "@tauri-apps/api/dialog";
import { invoke } from "@tauri-apps/api/tauri";
import { useEffect, useState } from "react";

const Lemon = () => {
  const [project, setProject] = useState<string | undefined>(undefined);

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
    selected &&
      invoke("open_project", { path: selected })
        .then((name) => setProject(name as string))
        .catch((error) => console.log(error));
  };

  useEffect(() => {
    invoke("get_project_name")
      .then((value) => setProject(value as string))
      .catch((_) => {});
  }, []);

  return (
    <div className="lemon">
      <div className="button-group">
        <img src={LemonIcon} id="lemon-icon" alt="logo of the game engine" />
        {project && <p>Project: {project}</p>}
        <button>New project</button>
        <button onClick={openProject}>Open project</button>
        <button>Save</button>
        <button>Save as</button>
      </div>
    </div>
  );
};

export default Lemon;
