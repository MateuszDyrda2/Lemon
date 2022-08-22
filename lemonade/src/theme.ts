import { createTheme } from "@mui/material";

const darkTheme = createTheme({
    palette: {
        mode: "dark",
        background: {
            default: "black",
            paper: "black",
        },
        primary:{
            main: "#827717",
        } ,
        secondary: {
            main: "#cdcc29",
        },
    },
});
export default darkTheme;