import ActionType from "../action_types";

interface SelectEntityAction {
  type: ActionType.SELECT;
  payload: number;
}

interface DeselectEntityAction {
  type: ActionType.DESELECT;
  payload: number;
}

type Action = SelectEntityAction | DeselectEntityAction;

export default Action;