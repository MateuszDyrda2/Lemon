import React, { createRef, useEffect, useState } from "react";
import "./styles/app.scss";
import Sidebar from "./components/sidebar";
import Sidepanel from "./components/sidepanel";
import { Tabs } from "./props/tabs";
import Midpanel from "./components/midpanel";
import darkTheme from "./theme";
import ScopedCssBaseline from "@mui/material/ScopedCssBaseline/ScopedCssBaseline";
import { ThemeProvider } from "@emotion/react";
import projectContext from "./project_context";

const MIN_WIDTH = 50;

function App() {
  const [currentTab, setTab] = useState<Tabs>(Tabs.None);
  const splitAppRef = createRef<HTMLDivElement>();
  const [leftWidth, setLeftWidth] = useState<number | undefined>(300);
  const [separatorXPosition, setSeparatorXPosition] = useState<
    number | undefined
  >(undefined);
  const [dragging, setDragging] = useState(false);
  const [projectName, setProjetName] = useState<string | undefined>(undefined);

  const onMouseDown = (e: React.MouseEvent) => {
    setSeparatorXPosition(e.clientX);
    setDragging(true);
  };
  const onMouseMove = (e: MouseEvent) => {
    if (dragging && leftWidth && separatorXPosition) {
      const newWidth = leftWidth + e.clientX - separatorXPosition;
      setSeparatorXPosition(e.clientX);
      const splitPanelWidth = splitAppRef.current?.clientWidth ?? newWidth;
      setLeftWidth(Math.min(Math.max(MIN_WIDTH, newWidth), splitPanelWidth));
    }
  };
  const onMouseUp = () => {
    setDragging(false);
  };

  useEffect(() => {
    document.addEventListener("mousemove", onMouseMove);
    document.addEventListener("mouseup", onMouseUp);
    return () => {
      document.removeEventListener("mousemove", onMouseMove);
      document.removeEventListener("mouseup", onMouseUp);
    };
  });

  return (
    <ThemeProvider theme={darkTheme}>
      <ScopedCssBaseline enableColorScheme>
        <projectContext.Provider
          value={{ context: projectName, setContext: setProjetName }}
        >
          <div className="App" ref={splitAppRef}>
            <Sidebar currentTab={currentTab} setTab={setTab} />
            <Sidepanel
              child={currentTab}
              setTab={setTab}
              leftWidth={leftWidth}
              setLeftWidth={setLeftWidth}
            />
            <div
              className="vertDiv-container"
              onMouseDown={onMouseDown}
              style={{ display: currentTab === Tabs.None ? "none" : "block" }}
            >
              <div className="vertDiv" />
            </div>
            <Midpanel />
          </div>
        </projectContext.Provider>
      </ScopedCssBaseline>
    </ThemeProvider>
  );
}

export default App;