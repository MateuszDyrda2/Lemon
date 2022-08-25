import "../../styles/sidepanels/systems.scss";

import MenuIcon from "@mui/icons-material/DragHandle";
import MoreVertIcon from "@mui/icons-material/MoreVert";
import { useCallback, useEffect, useRef, useState } from "react";
import { invoke } from "@tauri-apps/api/tauri";
import ArrowDropDownIcon from "@mui/icons-material/ArrowDropDown";
import ArrowDropUpIcon from "@mui/icons-material/ArrowDropUp";
import AddIcon from "@mui/icons-material/Add";

type Props = {
  name: string;
  k: number;
  children?: JSX.Element | JSX.Element[];
};

interface SystemContainer {
  stage: string;
  systems: string[];
}
interface IdPair {
  parent: number;
  index: number;
}

const Systems = () => {
  const [systemlist, setSystemlist] = useState<SystemContainer[] | undefined>(
    undefined
  );

  useEffect(() => {
    invoke("get_systems")
      .then((value) => {
        setSystemlist(value as SystemContainer[]);
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
    const dragged = _newSystems[currentPair.parent].systems.splice(
      currentPair.index,
      1
    )[0];

    const overPair = dragOverItem.current as IdPair;
    _newSystems[overPair.parent].systems.splice(overPair.index, 0, dragged);

    dragItem.current = null;
    dragOverItem.current = null;

    setSystemlist(_newSystems);
  };

  const Stage = ({ name, k, children }: Props) => {
    const [expanded, setExpanded] = useState(true);
    const onExpand = useCallback(() => setExpanded(!expanded), [expanded]);

    return (
      <div key={k} className="stage-group">
        <div
          className="stage"
          onDragEnter={() => onDragEnter(k, 0)}
          onDragEnd={onDragEnded}
          onDragOver={(e) => e.preventDefault()}
        >
          {expanded ? (
            <ArrowDropUpIcon
              className="stage-arrow"
              fontSize="small"
              onClick={onExpand}
            />
          ) : (
            <ArrowDropDownIcon
              className="stage-arrow"
              fontSize="small"
              onClick={onExpand}
            />
          )}
          <p className="stage-name">{name}</p>
          <AddIcon className="stage-more" fontSize="small" />
        </div>
        <div className={expanded ? "system-list" : "system-list--hidden"}>
          {children}
        </div>
      </div>
    );
  };

  return (
    <div className="systems">
      <div className="stage-list">
        <>
          {systemlist &&
            systemlist.map((stage, index) => (
              <Stage name={stage.stage} k={index} key={index}>
                {stage.systems.map((sys, i) => (
                  <div
                    id={sys}
                    key={i}
                    className="system"
                    draggable="true"
                    onDragStart={(e) => {
                      onDragStarted(index, i);
                    }}
                    onDragEnter={(e) => {
                      onDragEnter(index, i);
                    }}
                    onDragEnd={(e) => {
                      onDragEnded();
                    }}
                    onDragOver={(e) => e.preventDefault()}
                  >
                    <MenuIcon className="system-icon" fontSize="small" />
                    <p className="system-name">{sys}</p>
                    <MoreVertIcon className="system-more" fontSize="small" />
                  </div>
                ))}
              </Stage>
            ))}
        </>
      </div>
    </div>
  );
};

export default Systems;
