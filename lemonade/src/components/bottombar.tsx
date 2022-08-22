import { Tabs, Tab } from "@mui/material";
import React, { createRef } from "react";
import Output from "./output";
import Files from "./files";
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

  const handleChange = (_: React.SyntheticEvent, newValue: number) => {
    setValue(newValue);
  };

  return (
    <div className="bottombar-container" ref={bottomRef}>
      <Tabs value={value} onChange={handleChange} indicatorColor="secondary">
        <Tab label="Files" />
        <Tab label="Output" />
      </Tabs>
      {value === 0 && <Files />}
      {value === 1 && <Output />}
    </div>
  );
};

export default Bottombar;
