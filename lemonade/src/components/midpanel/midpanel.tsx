import Bottombar from './bottombar/bottombar';
import { MidpanelContainer } from './midpanel.styles';
import Topbar from './topbar/topbar';
import Viewport from './viewport/viewport';

const Midpanel = () => {
    return (
        <MidpanelContainer>
            <Topbar />
            <Viewport />
            <Bottombar />
        </MidpanelContainer>
    );
};

export default Midpanel;
