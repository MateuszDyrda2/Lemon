import { combineReducers  } from "redux";
import entityReducer from "./chosen_entity";

const reducers = combineReducers({
    entity: entityReducer
})

export default reducers;

export type State = ReturnType<typeof reducers>;