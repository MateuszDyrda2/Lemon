import { useState } from "react";
import { useSelector } from "react-redux";
import { State } from "../../state/reducers";
import "../../styles/sidepanels/components.scss";

const Components = () => {
  const state = useSelector((state: State) => state.entity);

  return (
    <div className="components">
      <div className="header">
        <p>{state < 0 ? "none" : state}</p>
      </div>
    </div>
  );
};

export default Components;
