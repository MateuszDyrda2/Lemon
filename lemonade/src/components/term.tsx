import ResizeObserver from "react-resize-observer";
import "../styles/styles.scss";
import "../styles/term.scss";
import "xterm/css/xterm.css";
import { Terminal } from "xterm";
import { FitAddon } from "xterm-addon-fit";
import React, { useEffect } from "react";

var term: Terminal | null = null;

const Caret = () => {
  term?.write("\r\n$ ");
};

const Term = () => {
  const t = React.useRef<HTMLDivElement>(null);
  var fitAddon: FitAddon;
  const initTerminal = () => {
    if (term) term.dispose();
    term = new Terminal({
      rendererType: "canvas",
      convertEol: true,
      cursorBlink: false,
      cursorStyle: "block",
    });
    fitAddon = new FitAddon();
    term.loadAddon(fitAddon);
    term.open(t.current!);
    term.options = {
      theme: { background: "#303030", foreground: "white" },
      fontSize: 16,
      fontWeight: 500,
    };
    fitAddon.fit();
    term.write("Welcome to Lemon");
    Caret();
    term.onKey(
      ({ key, domEvent }: { key: string; domEvent: KeyboardEvent }) => {
        switch (domEvent.key) {
          case "Enter": {
            Caret();
            break;
          }
          case "Backspace": {
            term!.write("\b \b");
            break;
          }
          default:
            term!.write(key);
        }
      }
    );
  };
  useEffect(() => {
    initTerminal();
  });
  return (
    <div className="term" ref={t}>
      <ResizeObserver onResize={() => fitAddon && fitAddon.fit()} />
    </div>
  );
};

export default Term;
