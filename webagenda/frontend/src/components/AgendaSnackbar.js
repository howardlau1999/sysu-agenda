import React, { Component } from "react";
import { withStyles } from "@material-ui/core/styles";
import Snackbar from "@material-ui/core/Snackbar";
import IconButton from "@material-ui/core/IconButton";
import CloseIcon from "@material-ui/icons/Close";
import { connect } from "react-redux";
import { hideSnackbar } from "../redux/actions";
const styles = theme => ({
  close: {
    padding: theme.spacing.unit / 2
  }
});

class AgendaSnackbar extends Component {

  render() {
    const { open, message, classes, hideSnackbar } = this.props;
    return (
      <Snackbar
        anchorOrigin={{
          vertical: "bottom",
          horizontal: "left"
        }}
        open={open}
        autoHideDuration={6000}
        onClose={hideSnackbar}
        ContentProps={{
          "aria-describedby": "message-id"
        }}
        message={<span id="message-id">{message}</span>}
        action={[
          <IconButton
            key="close"
            aria-label="Close"
            color="inherit"
            className={classes.close}
            onClick={hideSnackbar}
          >
            <CloseIcon />
          </IconButton>
        ]}
      />
    );
  }
}

const mapStateToProps = state => {
  return state.snackBar;
};

export default connect(
  mapStateToProps,
  { hideSnackbar }
)(withStyles(styles)(AgendaSnackbar));
