import React, { createRef } from "react";
import Output from "./output";
import Assets from "./assets";
import Scenes from "./scenes";
import "../styles/bottombar.scss";

interface Props {
  bottomHeight: number | undefined;
  setBottomHeight: (value: number) => void;
}

const Bottombar = ({ bottomHeight, setBottomHeight }: Props) => {
  const [value, setValue] = React.useState(0);
  const bottomRef = createRef<HTMLDivElement>();

  React.useEffect(() => {
    if (bottomRef.current) {
      if (!bottomHeight) {
        setBottomHeight(bottomRef.current?.clientHeight);
        return;
      }
      bottomRef.current.style.height = `${bottomHeight}px`;
    }
  }, [bottomRef, bottomHeight, setBottomHeight]);

  const BottomTab = ({
    name,
    expected,
  }: {
    name: string;
    expected: number;
  }) => {
    return (
      <button
        className={value === expected ? "tab--chosen" : "tab"}
        onClick={() => setValue(expected)}
      >
        {name}
      </button>
    );
  };

  return (
    <div className="bottombar-container" ref={bottomRef}>
      <div className="bottombar-bar">
        <BottomTab name="Scenes" expected={0} />
        <BottomTab name="Assets" expected={1} />
        <BottomTab name="Output" expected={2} />
      </div>
      {value === 0 && <Scenes />}
      {value === 1 && <Assets />}
      {value === 2 && <Output />}
    </div>
  );
};

export default Bottombar;
