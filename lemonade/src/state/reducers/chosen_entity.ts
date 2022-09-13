import Action from "../actions";
import ActionType from "../action_types";

const initialState = -1;

const entityReducer = (state: number = initialState, action: Action) => {
  switch (action.type) {
    case ActionType.SELECT:
      return action.payload;
    case ActionType.DESELECT:
      return -1;
    default:
      return -1;
  }
};

export default entityReducer;
