import React from "react";
import ReactDOM from "react-dom/client";
import "./index.css";
import App from "./App";
import reportWebVitals from "./reportWebVitals";
import "./fonts/Roboto_Mono/RobotoMono-VariableFont_wght.ttf";
import { Provider } from "react-redux";
import { store } from "./state/index";
import { invoke } from "@tauri-apps/api/tauri";

const root = ReactDOM.createRoot(
  document.getElementById("root") as HTMLElement
);
document.addEventListener("DOMContentLoaded", () => {
  invoke("close_splashscreen");
  console.log("loaded");
});

root.render(
  <React.StrictMode>
    <Provider store={store}>
      <App />
    </Provider>
  </React.StrictMode>
);

// If you want to start measuring performance in your app, pass a function
// to log results (for example: reportWebVitals(console.log))
// or send to an analytics endpoint. Learn more: https://bit.ly/CRA-vitals
reportWebVitals();