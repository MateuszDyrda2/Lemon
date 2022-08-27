import "../../styles/sidepanels/scene_hierarchy.scss";

import ArrowDropDownIcon from "@mui/icons-material/ArrowDropDown";
import ArrowDropUpIcon from "@mui/icons-material/ArrowDropUp";
import { useState } from "react";

interface Entity {
  name: string;
  children: Entity[];
}

const entities: Entity[] = [
  { name: "mainCamera", children: [] },
  {
    name: "player",
    children: [
      { name: "sword", children: [{ name: "tip", children: [] }] },
      { name: "shield", children: [] },
    ],
  },
];

const sceneName = "Sandbox";

const SceneHierarchy = () => {
  const Ent = (ent: Entity, expFromParent: boolean) => {
    const [isExpanded, setIsExpanded] = useState(false);
    return (
      <div className={expFromParent ? "entity" : "entity--hidden"}>
        <div className="entity-container">
          {ent.children.length !== 0 &&
            (!isExpanded ? (
              <ArrowDropDownIcon
                className="entity-drop"
                fontSize="small"
                onClick={() => setIsExpanded(true)}
              />
            ) : (
              <ArrowDropUpIcon
                className="entity-drop"
                fontSize="small"
                onClick={() => setIsExpanded(false)}
              />
            ))}
          <p>{ent.name}</p>
        </div>
        {ent.children.map((key, index) => Ent(key, isExpanded))}
      </div>
    );
  };

  return (
    <div className="scene_hierarchy">
      <div className="header">
        <p className="scene-name">{sceneName}</p>
      </div>
      <div className="entities-list">
        {entities.map((key, index) => Ent(key, true))}
      </div>
    </div>
  );
};

export default SceneHierarchy;
