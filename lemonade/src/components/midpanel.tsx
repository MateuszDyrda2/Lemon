import Bottombar from "./bottombar";
import Viewport from "./viewport";
import Topbar from "./topbar";

import "../styles/midpanel.scss";
import React, { createRef, useEffect, useState } from "react";

const MIN_HEIGHT = 50;

const Midpanel = () => {
  const splitPanelRef = createRef<HTMLDivElement>();
  const [bottomHeight, setBottomHeight] = useState<number | undefined>(
    undefined
  );
  const [separatorYPosition, setSeparatorYPosition] = useState<
    number | undefined
  >(undefined);
  const [dragging, setDragging] = useState(false);

  const onMouseDown = (e: React.MouseEvent) => {
    setSeparatorYPosition(e.clientY);
    setDragging(true);
  };
  const onMouseMove = (e: MouseEvent) => {
    if (dragging && bottomHeight && separatorYPosition) {
      const newHeight = bottomHeight - e.clientY + separatorYPosition;
      setSeparatorYPosition(e.clientY);
      const splitPanelHeight = splitPanelRef.current?.clientHeight ?? newHeight;
      setBottomHeight(
        Math.min(Math.max(MIN_HEIGHT, newHeight), splitPanelHeight)
      );
    }
  };
  useEffect(() => {
    document.addEventListener("mousemove", onMouseMove);
    document.addEventListener("mouseup", onMouseUp);
    return () => {
      document.removeEventListener("mousemove", onMouseMove);
      document.removeEventListener("mouseup", onMouseUp);
    };
  });
  const onMouseUp = () => {
    setDragging(false);
  };

  return (
    <div className="midpanel" ref={splitPanelRef}>
      <Topbar />
      <Viewport />
      <div className="divider-container" onMouseDown={onMouseDown}>
        <div className="divider" />
      </div>
      <Bottombar
        bottomHeight={bottomHeight}
        setBottomHeight={setBottomHeight}
      />
    </div>
  );
};

export default Midpanel;
