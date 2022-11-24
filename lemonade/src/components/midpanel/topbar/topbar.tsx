import {
    TopbarContainer,
    ToolboxContainer,
    ToolWrapper,
    PlayWrapper,
    PlayContainer,
} from './topbar.styles';

import { RiDragMove2Line } from 'react-icons/ri';
import { GiHand } from 'react-icons/gi';
import { MdOutlineGrid4X4 } from 'react-icons/md';
import { FaPlay, FaStop } from 'react-icons/fa';
import { useRecoilState } from 'recoil';
import { selectedTools, SelectedTools } from '../../../state/selected_tools';
import { Tools } from '../../../props/tools';
import { invoke } from '@tauri-apps/api';

const Topbar = () => {
    const [selected, setSelected] = useRecoilState(selectedTools);

    const setTool = (tool: Tools) => {
        setSelected((current) => {
            const s: SelectedTools = {
                tool: tool,
                grid: current.grid,
            };
            return s;
        });
    };

    const setGrid = () => {
        setSelected((current) => {
            const s: SelectedTools = {
                tool: current.tool,
                grid: !current.grid,
            };
            return s;
        });
    };
    const runEngine = () => {
        invoke('run_engine').catch(console.error);
    };

    const stopEngine = () => {
        invoke('stop_engine').catch(console.error);
    };

    return (
        <TopbarContainer>
            <ToolboxContainer>
                <ToolWrapper
                    isActive={selected.tool === Tools.Mouse}
                    onClick={() => setTool(Tools.Mouse)}
                >
                    <GiHand size="25" />
                </ToolWrapper>
                <ToolWrapper
                    isActive={selected.tool === Tools.Move}
                    onClick={() => setTool(Tools.Move)}
                >
                    <RiDragMove2Line size="25" />
                </ToolWrapper>
                <ToolWrapper isActive={selected.grid} onClick={setGrid}>
                    <MdOutlineGrid4X4 size="25" />
                </ToolWrapper>
            </ToolboxContainer>
            <PlayContainer>
                <PlayWrapper onClick={runEngine}>
                    <FaPlay size="25" color="#12F012" />
                </PlayWrapper>
                <PlayWrapper onClick={stopEngine}>
                    <FaStop size="25" />
                </PlayWrapper>
            </PlayContainer>
        </TopbarContainer>
    );
};

export default Topbar;
