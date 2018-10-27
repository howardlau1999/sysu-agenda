import React, { Component } from "react";
import { withStyles } from "@material-ui/core/styles";
import Login from "./Login";
import Register from "./Register";
import { Dashboard } from "./Dashboard";
import { BrowserRouter, Route} from "react-router-dom";
import AgendaSnackbar from "./AgendaSnackbar";

const styles = theme => ({
  close: {
    padding: theme.spacing.unit / 2
  }
});

class Agenda extends Component {
  render() {
    return (
      <BrowserRouter>
        <div className="Agenda">
          <Route path="/dashboard" component={Dashboard} />
          <Route path="/login" component={Login} />
          <Route path="/register" component={Register} />
          
          <AgendaSnackbar />
        </div>
      </BrowserRouter>
    );
  }
}

export default withStyles(styles)(Agenda);
