import {
    BottombarBar,
    BottombarBarButton,
    BottombarContainer,
} from './bottombar.styles';

const Bottombar = () => {
    return (
        <BottombarContainer>
            <BottombarBar>
                <BottombarBarButton>Scenes</BottombarBarButton>
                <BottombarBarButton>Assets</BottombarBarButton>
                <BottombarBarButton>Output</BottombarBarButton>
            </BottombarBar>
        </BottombarContainer>
    );
};

export default Bottombar;
