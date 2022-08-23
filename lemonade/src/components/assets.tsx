import "../styles/assets.scss";
import { invoke } from "@tauri-apps/api/tauri";
import React from "react";
import FolderIcon from "@mui/icons-material/Folder";

interface path {
  name: string;
  path: string;
}
interface AssetData {
  textures: path[];
  sounds: path[];
  shaders: path[];
}

const Assets = () => {
  const [assets, setAssets] = React.useState<AssetData | undefined>(undefined);

  React.useEffect(() => {
    invoke("get_assets").then((result) => {
      setAssets(result as AssetData);
    });
  }, []);
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
