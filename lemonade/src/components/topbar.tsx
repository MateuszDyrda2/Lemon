import "../styles/topbar.scss";

import PlayArrowRoundedIcon from "@mui/icons-material/PlayArrowRounded";
import StopRounded from "@mui/icons-material/StopRounded";

const Topbar = () => {
  return (
    <div className="topbar">
      <div className="icon-container">
        <StopRounded className="icon" />
        <PlayArrowRoundedIcon className="icon" />
      </div>
    </div>
  );
};

export default Topbar;
