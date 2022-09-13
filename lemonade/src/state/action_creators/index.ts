import ActionType from "../action_types"
import { Dispatch } from "redux"
import Action from "../actions"

export const selectEntity = (entity: number) => {
    return (dispatch: Dispatch<Action>) => {
        dispatch({
            type: ActionType.SELECT,
            payload: entity
        })
    }
}

export const deselectEntity = (entity: number) => {
    return (dispatch: Dispatch<Action>) => {
        dispatch({
            type: ActionType.DESELECT,
            payload: entity
        })
    }
}
