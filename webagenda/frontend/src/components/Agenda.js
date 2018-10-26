import React, { Component } from "react";
import { withStyles } from "@material-ui/core/styles";
import Login from "./Login";
import Register from "./Register";
import { Dashboard } from "./Dashboard";
import { BrowserRouter, Route } from "react-router-dom";
import Snackbar from "@material-ui/core/Snackbar";
import IconButton from "@material-ui/core/IconButton";
import CloseIcon from "@material-ui/icons/Close";
const styles = theme => ({
  close: {
    padding: theme.spacing.unit / 2
  }
});

class Agenda extends Component {
  state = {
    snackOpen: false,
    snackMessage: null
  };

  showSnackMessage = msg => {
    this.setState({
      snackOpen: true,
      snackMessage: msg
    });
  };

  handleClose = () => {
    this.setState({
      snackOpen: false,
      snackMessage: null
    });
  };
  render() {
    const { classes } = this.props;

    return (
      <BrowserRouter>
        <div className="Agenda">
          <Route path="/dashboard" component={Dashboard} />
          <Route path="/login" component={Login} />
          <Route
            path="/register"
            render={props => (
              <Register {...props} showSnackMessage={this.showSnackMessage} />
            )}
          />
          <Snackbar
            anchorOrigin={{
              vertical: "bottom",
              horizontal: "left"
            }}
            open={this.state.snackOpen}
            autoHideDuration={6000}
            onClose={this.handleClose}
            ContentProps={{
              "aria-describedby": "message-id"
            }}
            message={<span id="message-id">{this.state.snackMessage}</span>}
            action={[
              <IconButton
                key="close"
                aria-label="Close"
                color="inherit"
                className={classes.close}
                onClick={this.handleClose}
              >
                <CloseIcon />
              </IconButton>
            ]}
          />
        </div>
      </BrowserRouter>
    );
  }
}

export default withStyles(styles)(Agenda);
