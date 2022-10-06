import React from "react";
import Stack from "@mui/material/Stack";
import AccountTreeRounded from "@mui/icons-material/AccountTreeRounded";
import MenuBookRounded from "@mui/icons-material/MenuBookRounded";
import ExtensionRounded from "@mui/icons-material/ExtensionRounded";
import SettingsRounded from "@mui/icons-material/SettingsRounded";
import Icon from "@mui/material/Icon";

import "../styles/sidebar.scss";
import { Tabs } from "../props/tabs";

import LemonIcon from "../img/lemon.svg";

interface Props {
  currentTab: Tabs;
  setTab: React.Dispatch<any>;
}

const Sidebar = (tab: Props) => {
  const changeTab = (newTab: Tabs) => {
    tab.setTab(tab.currentTab === newTab ? Tabs.None : newTab);
  };

  return (
    <div className="sidebar-container">
      <div className="sidebar">
        <div className="sidebar-child">
          <Stack spacing={2}>
            <Icon
              fontSize="large"
              className={
                tab.currentTab === Tabs.Lemon
                  ? "sidebar-item--choosen"
                  : "sidebar-item"
              }
              onClick={() => changeTab(Tabs.Lemon)}
            >
              <img
                src={LemonIcon}
                id="lemon-ico"
                alt="icon of the lemon engine"
              />
            </Icon>
            <AccountTreeRounded
              fontSize="large"
              className={
                tab.currentTab === Tabs.SceneHierarchy
                  ? "sidebar-item--choosen"
                  : "sidebar-item"
              }
              onClick={() => changeTab(Tabs.SceneHierarchy)}
            />
            <MenuBookRounded
              fontSize="large"
              className={
                tab.currentTab === Tabs.Systems
                  ? "sidebar-item--choosen"
                  : "sidebar-item"
              }
              onClick={() => changeTab(Tabs.Systems)}
            />
            <ExtensionRounded
              fontSize="large"
              className={
                tab.currentTab === Tabs.Components
                  ? "sidebar-item--choosen"
                  : "sidebar-item"
              }
              onClick={() => changeTab(Tabs.Components)}
            />
            <SettingsRounded
              fontSize="large"
              className={
                tab.currentTab === Tabs.Settings
                  ? "sidebar-item--choosen"
                  : "sidebar-item"
              }
              onClick={() => changeTab(Tabs.Settings)}
            />
          </Stack>
        </div>
      </div>
    </div>
  );
};

export default Sidebar;