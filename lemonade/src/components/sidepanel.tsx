import "../styles/sidepanel.scss";

import { Tabs } from "../props/tabs";
import { createRef, useEffect } from "react";

interface Props {
  child: Tabs;
  leftWidth: number | undefined;
  setLeftWidth: (value: number) => void;
}

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

  return <div id="sidepanel" className="sidepanel" ref={leftRef}></div>;
};

export default Sidepanel;
