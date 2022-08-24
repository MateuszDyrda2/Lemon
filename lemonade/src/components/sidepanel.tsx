import "../styles/sidepanel.scss";

import { Tabs } from "../props/tabs";
import { createRef, useEffect } from "react";
import Lemon from "./sidepanels/lemon";
import SceneHierarchy from "./sidepanels/scene_hierarchy";
import Systems from "./sidepanels/systems";
import Components from "./sidepanels/components";
import Settings from "./sidepanels/settings";

interface Props {
  child: Tabs;
  leftWidth: number | undefined;
  setLeftWidth: (value: number) => void;
}

const TabToName = (tab: Tabs) => {
  switch (tab) {
    case Tabs.Lemon:
      return "Lemon";
    case Tabs.SceneHierarchy:
      return "Scene Hierarchy";
    case Tabs.Systems:
      return "Systems";
    case Tabs.Components:
      return "Components";
    case Tabs.Settings:
      return "Settings";

    default:
      return "None";
  }
};

const Sidepanel = ({ child, leftWidth, setLeftWidth }: Props) => {
  const leftRef = createRef<HTMLDivElement>();

  useEffect(() => {
    if (leftRef.current) {
      if (!leftWidth) {
        setLeftWidth(leftRef.current?.clientWidth);
        return;
      }
      const setWidth = child === Tabs.None ? 0 : leftWidth;
      leftRef.current.style.width = `${setWidth}px`;
    }
  }, [leftRef, leftWidth, setLeftWidth, child]);

  return (
    <div id="sidepanel" className="sidepanel" ref={leftRef}>
      <p className="sidepanel-top">{TabToName(child)}</p>
      <div className="content">
        {child === Tabs.Lemon && <Lemon />}
        {child === Tabs.SceneHierarchy && <SceneHierarchy />}
        {child === Tabs.Systems && <Systems />}
        {child === Tabs.Components && <Components />}
        {child === Tabs.Settings && <Settings />}
      </div>
    </div>
  );
};

export default Sidepanel;
