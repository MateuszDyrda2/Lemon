import "../../styles/sidepanels/systems.scss";

import MenuIcon from "@mui/icons-material/DragHandle";
import MoreVertIcon from "@mui/icons-material/MoreVert";
import { useCallback, useEffect, useRef, useState } from "react";
import { invoke } from "@tauri-apps/api/tauri";
import ArrowDropDownIcon from "@mui/icons-material/ArrowDropDown";
import ArrowDropUpIcon from "@mui/icons-material/ArrowDropUp";
import CloseIcon from "@mui/icons-material/Close";
import Container from "./container";

type Props = {
  name: string;
  k: number;
  children?: JSX.Element | JSX.Element[];
};

interface SystemDef {
  nameid: number;
  name: string;
}
interface StageDef {
  id: number;
  name: string;
}

interface SystemContainer {
  stage: StageDef;
  systems: SystemDef[];
}

interface IdPair {
  parent: number;
  index: number;
}

const Systems = () => {
  const [systemlist, setSystemlist] = useState<SystemContainer[] | undefined>(
    undefined
  );

  const [systemDefs, setSystemDefs] = useState<SystemDef[] | undefined>(
    undefined
  );

  const saveSystems = () => {
    invoke("set_systems", { systemlist: systemlist }).catch((error) =>
      console.log(error)
    );
  };

  useEffect(() => {
    invoke("get_systems")
      .then((value) => {
        setSystemlist(value as SystemContainer[]);
      })
      .catch((error) => console.log(error));

    invoke("get_system_definitions")
      .then((value) => {
        setSystemDefs(value as SystemDef[]);
      })
      .catch((error) => console.log(error));
  }, []);

  const dragItem = useRef<IdPair | any>(null);
  const dragOverItem = useRef<number | any>(null);

  const onDragStarted = (parent: number, index: number) => {
    dragItem.current = { parent, index };
  };
  const onDragEnter = (parent: number, index: number) => {
    dragOverItem.current = { parent, index };
  };
  const onDragEnded = () => {
    let _newSystems = [...(systemlist as SystemContainer[])];

    const currentPair = dragItem.current as IdPair;

    const dragged =
      currentPair.parent !== -1
        ? _newSystems[currentPair.parent].systems.splice(
            currentPair.index,
            1
          )[0]
        : (systemDefs as SystemDef[])[currentPair.index];

    const overPair = dragOverItem.current as IdPair;

    if (overPair.parent !== -1) {
      _newSystems[overPair.parent].systems.splice(overPair.index, 0, dragged);
    }

    dragItem.current = null;
    dragOverItem.current = null;

    setSystemlist(_newSystems);
    saveSystems();
  };

  const Stage = ({ name, k, children }: Props) => {
    const [expanded, setExpanded] = useState(
      (systemlist as SystemContainer[])[k].systems.length !== 0
    );
    const onExpand = useCallback(() => {
      (systemlist as SystemContainer[])[k].systems.length !== 0 &&
        setExpanded(!expanded);
    }, [expanded]);

    return (
      <div key={k} className="stage-group">
        <div
          className="stage"
          onDragEnter={() => onDragEnter(k, 0)}
          onDragEnd={onDragEnded}
          onDragOver={(e) => e.preventDefault()}
          onClick={onExpand}
        >
          {expanded ? (
            <ArrowDropUpIcon className="stage-arrow" fontSize="small" />
          ) : (
            <ArrowDropDownIcon className="stage-arrow" fontSize="small" />
          )}
          <p className="stage-name">{name}</p>
        </div>
        <div className={expanded ? "system-list" : "system-list--hidden"}>
          {children}
        </div>
      </div>
    );
  };

  const onRemoveSys = (i: number, j: number) => {
    let _newSystems = [...(systemlist as SystemContainer[])];
    _newSystems[i].systems.splice(j, 1);

    setSystemlist(_newSystems);
    saveSystems();

    invoke("get_components")
      .then((value) => console.log(value))
      .catch((err) => console.log(err));
  };

  return (
    <div className="systems">
      <Container header="Sandbox" st={{ height: "50%", marginBottom: "25px" }}>
        <div className="stage-list">
          <>
            {systemlist &&
              systemlist.map((stage, index) => (
                <Stage name={stage.stage.name} k={index} key={index}>
                  {stage.systems.map((sys, i) => (
                    <div
                      id={sys.name}
                      key={i}
                      className="system"
                      draggable="true"
                      onDragStart={() => {
                        onDragStarted(index, i);
                      }}
                      onDragEnter={() => {
                        onDragEnter(index, i);
                      }}
                      onDragEnd={onDragEnded}
                      onDragOver={(e) => e.preventDefault()}
                    >
                      <MenuIcon className="system-icon" fontSize="small" />
                      <p className="system-name">{sys.name}</p>
                      <CloseIcon
                        className="system-del"
                        fontSize="small"
                        onClick={() => onRemoveSys(index, i)}
                      />
                      <MoreVertIcon className="system-more" fontSize="small" />
                    </div>
                  ))}
                </Stage>
              ))}
          </>
        </div>
      </Container>
      <Container header="Defined Systems" st={{ height: "45%", bottom: "0" }}>
        <div className="def-lists">
          <>
            {systemDefs &&
              systemDefs.map((system, index) => (
                <div
                  id={system.name}
                  key={index}
                  className="system-def"
                  draggable="true"
                  onDragStart={() => onDragStarted(-1, index)}
                  onDragEnter={() => onDragEnter(-1, index)}
                  onDragEnd={() => onDragEnded()}
                  onDragOver={(e) => e.preventDefault()}
                >
                  <MenuIcon className="systemdef-icon" fontSize="small" />
                  <p className="systemdef-name">{system.name}</p>
                </div>
              ))}
          </>
        </div>
      </Container>
    </div>
  );
};

export default Systems;
