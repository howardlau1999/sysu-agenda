import React, { Component } from "react";
import { withStyles } from "@material-ui/core/styles";
import CssBaseline from "@material-ui/core/CssBaseline";
import Typography from "@material-ui/core/Typography";
import UsersTable from "./UsersTable";
import MeetingsTable from "./MeetingsTable";
import { Switch, Route } from "react-router-dom";
import MeetingCreator from "./MeetingCreator";
import DashboardBorder from "./DashboardBorder";

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
                component={() => (
                  <div>
                    <div className={classes.appBarSpacer} />
                    <Typography variant="h4" gutterBottom component="h2">
                      Users
                    </Typography>
                    <div className={classes.tableContainer}>
                      <UsersTable />
                    </div>
                  </div>
                )}
              />
              <Route
                path="/dashboard/meetings"
                component={() => (
                  <div>
                    <div className={classes.appBarSpacer} />

                    <Typography variant="h4" gutterBottom component="h2">
                      My Sponsored Meetings
                      <MeetingCreator />
                    </Typography>
                    <div className={classes.tableContainer}>
                      <MeetingsTable meetingsType="sponsor" />
                    </div>

                    <Typography variant="h4" gutterBottom component="h2">
                      My Participated Meetings
                    </Typography>
                    <div className={classes.tableContainer}>
                      <MeetingsTable meetingsType="participate" />
                    </div>
                  </div>
                )}
              />
            </Switch>
          </main>
        </div>
      </React.Fragment>
    );
  }
}

export default withStyles(styles)(Dashboard);
