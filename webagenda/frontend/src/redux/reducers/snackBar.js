import {
    SHOW_SNACKBAR_NORMAL,
    HIDE_SNACKBAR
} from "../actionTypes";

const initialState = {
    open: false,
    message: null
}

export default function (state = initialState, action) {
    switch (action.type) {
        case SHOW_SNACKBAR_NORMAL:
            return {
                open: true,
                message: action.payload.message
            };
        case HIDE_SNACKBAR:
            return {
                open: false,
                message: state.message
            }
        default:
            return state;
    }
}