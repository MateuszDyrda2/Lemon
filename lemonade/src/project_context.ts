import React from "react";

type ProjectContext = {
    context: string | undefined;
    setContext: React.Dispatch<any>;
}

const projectContext = React.createContext<
ProjectContext>({context: undefined, setContext: () => undefined});

export default projectContext;