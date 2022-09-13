import "../../styles/sidepanels/scene_hierarchy.scss";

import ArrowDropDownIcon from "@mui/icons-material/ArrowDropDown";
import ArrowDropUpIcon from "@mui/icons-material/ArrowDropUp";
import { useState } from "react";
import { Tabs } from "../../props/tabs";
import { useDispatch, useSelector } from "react-redux";
import { bindActionCreators } from "redux";
import { actionCreators } from "../../state";
import { State } from "../../state/reducers";
import Container from "./container";

interface Entity {
  id: number;
  name: string;
  children: Entity[];
}

const entities: Entity[] = [
  { id: 0, name: "mainCamera", children: [] },
  {
    id: 1,
    name: "player",
    children: [
      {
        id: 2,
        name: "sword",
        children: [{ id: 4, name: "tip", children: [] }],
      },
      { id: 3, name: "shield", children: [] },
    ],
  },
];

const sceneName = "Sandbox";

interface Props {
  setTab: React.Dispatch<any>;
}

const SceneHierarchy = ({ setTab }: Props) => {
  const dispatch = useDispatch();
  const { selectEntity } = bindActionCreators(actionCreators, dispatch);

  const entityPressed = (ent: Entity) => selectEntity(ent.id);
  const state = useSelector((state: State) => state.entity);

  const entityDoublePressed = (ent: Entity) => setTab(Tabs.Components);

  const Ent = (ent: Entity, expFromParent: boolean) => {
    const [isExpanded, setIsExpanded] = useState(false);
    return (
      <div className={expFromParent ? "entity" : "entity--hidden"} key={ent.id}>
        <div
          className={
            ent.id === state ? "entity-container--selected" : "entity-container"
          }
          onDoubleClick={() => entityDoublePressed(ent)}
          onClick={() => entityPressed(ent)}
        >
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
    <Container header={sceneName}>
      <div className="entities-list">
        {entities.map((key, index) => Ent(key, true))}
      </div>
    </Container>
  );
};

export default SceneHierarchy;
