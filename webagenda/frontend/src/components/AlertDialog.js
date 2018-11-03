import Dialog from "@material-ui/core/Dialog";

import DialogActions from "@material-ui/core/DialogActions";
import Button from "@material-ui/core/Button";
import React from "react";
class AlertDialog extends React.Component {

  render() {
    return (
      <Dialog
        {...this.props}
        aria-labelledby="alert-dialog-title"
        aria-describedby="alert-dialog-description"
      >
        {this.props.children}
        <DialogActions>
          <Button onClick={this.props.onClose} color="primary" autoFocus>
            OK
          </Button>
        </DialogActions>
      </Dialog>
    );
  }
}

export default AlertDialog;
