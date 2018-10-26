import {
    SHOW_SNACKBAR_NORMAL,
    HIDE_SNACKBAR
} from "./actionTypes";

export const showSnackbarMessage = content => ({
    type: SHOW_SNACKBAR_NORMAL,
    payload: {
        message: content
    }
});

export const hideSnackbar = () => ({
    type: HIDE_SNACKBAR,
    payload: null
});