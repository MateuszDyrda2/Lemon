import "../styles/assets.scss";
import React from "react";
import FolderIcon from "@mui/icons-material/Folder";

const Assets = () => {
  return (
    <div className="assets">
      <div className="grid-parent">
        <div className="directory">
          <FolderIcon className="icon" />
          <span className="caption">textures</span>
        </div>
        <div className="directory">
          <FolderIcon className="icon" />
          <span className="caption">sounds</span>
        </div>
        <div className="directory">
          <FolderIcon className="icon" />
          <span className="caption">shaders</span>
        </div>
      </div>
    </div>
  );
};

export default Assets;
