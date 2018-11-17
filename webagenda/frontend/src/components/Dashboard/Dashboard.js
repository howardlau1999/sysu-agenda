import React, { Component } from "react";
import { withStyles } from "@material-ui/core/styles";
import CssBaseline from "@material-ui/core/CssBaseline";
import { Switch, Route } from "react-router-dom";
import DashboardBorder from "./DashboardBorder";
import Meetings from './Meetings';
import Users from "./Users";
import Query from "./Query";

const styles = theme => ({
  root: {
    display: "flex"
  },

  appBarSpacer: theme.mixins.toolbar,
  content: {
    flexGrow: 1,
    padding: theme.spacing.unit * 3,
    height: "100vh",
    overflow: "auto"
  },
  chartContainer: {
    marginLeft: -22
  },
  tableContainer: {
    marginBottom: 10
  },
  h5: {
    marginBottom: theme.spacing.unit * 2
  }
});
class Dashboard extends Component {
  state = {
    loggined: localStorage.getItem("loggined")
  };
  render() {
    const { classes } = this.props;
    return (
      <React.Fragment>
        <CssBaseline />
        <div className={classes.root}>
          <DashboardBorder />

          <main className={classes.content}>
            <Switch>
              <Route
                path="/dashboard/users"
                component={Users}
              />
              <Route
                path="/dashboard/meetings"
                component={Meetings}
              />
              <Route
                path="/dashboard/search"
                component={Query}
              />
            </Switch>
          </main>
        </div>
      </React.Fragment>
    );
  }
}

export default withStyles(styles)(Dashboard);
