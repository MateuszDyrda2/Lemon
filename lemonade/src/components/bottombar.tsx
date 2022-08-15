import { Tabs, Tab, Divider } from "@mui/material";
import React, { createRef } from "react";
import Term from "./term";
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
        <Tab label="Terminal" />
        <Tab label="Files" />
      </Tabs>
      {value === 0 && <Term />}
      {value === 1 && <Files />}
    </div>
  );
};

export default Bottombar;
