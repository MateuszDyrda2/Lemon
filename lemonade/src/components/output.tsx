import "../styles/styles.scss";
import "../styles/output.scss";
import { appWindow } from "@tauri-apps/api/window";
import React from "react";

const Output = () => {
  React.useEffect(() => {
    appWindow.listen("new-output", (event) => {
      console.log(event.payload);
      console.log("aaa");
    });
  }, []);
  return (
    <div className="output">
      <div className="typed">
        OOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutputOutpututput
      </div>
    </div>
  );
};

export default Output;
