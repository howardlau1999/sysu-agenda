import React, { Component } from "react";
import { withStyles } from "@material-ui/core/styles";
import Typography from "@material-ui/core/Typography";
import UsersTable from "./UsersTable";

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
class Users extends Component {
  render() {
    const { classes } = this.props;
    return (
      <div>
        <div className={classes.appBarSpacer} />
        <Typography variant="h4" gutterBottom component="h2">
          Users
        </Typography>
        <div className={classes.tableContainer}>
          <UsersTable />
        </div>
      </div>
    );
  }
}

export default withStyles(styles)(Users);
