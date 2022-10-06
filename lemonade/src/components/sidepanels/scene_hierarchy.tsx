import "../../styles/sidepanels/scene_hierarchy.scss";

import { useEffect, useState } from "react";
import { Tabs } from "../../props/tabs";
import { useDispatch, useSelector } from "react-redux";
import { bindActionCreators } from "redux";
import { actionCreators } from "../../state";
import { State } from "../../state/reducers";
import Container from "./container";
import { invoke } from "@tauri-apps/api/tauri";

interface Entity {
  id: number;
  name: string;
}

const sceneName = "Sandbox";

interface Props {
  setTab: React.Dispatch<any>;
}

const SceneHierarchy = ({ setTab }: Props) => {
  const dispatch = useDispatch();
  const { selectEntity } = bindActionCreators(actionCreators, dispatch);
  const [entities, setEntities] = useState<Entity[] | undefined>(undefined);

  const entityPressed = (ent: Entity) => selectEntity(ent.id);
  const state = useSelector((state: State) => state.entity);

  const entityDoublePressed = (ent: Entity) => setTab(Tabs.Components);
  useEffect(() => {
    invoke("get_entities")
      .then((value) => setEntities(value as Entity[]))
      .catch((err) => console.log(err));
  });

  const Ent = (ent: Entity) => {
    return (
      <div
        className={ent.id === state ? "entity--selected" : "entity"}
        key={ent.id}
        onClick={() => entityPressed(ent)}
        onDoubleClick={() => entityDoublePressed(ent)}
      >
        <p>{ent.name}</p>
      </div>
    );
  };

  return (
    <Container header={sceneName}>
      <div className="entities-list">
        {entities && entities.map((key, index) => Ent(key))}
      </div>
    </Container>
  );
};

export default SceneHierarchy;
