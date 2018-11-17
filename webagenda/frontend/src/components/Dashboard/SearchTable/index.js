import React, { Component } from "react";
import { withStyles } from "@material-ui/core/styles";
import CssBaseline from "@material-ui/core/CssBaseline";
import Typography from "@material-ui/core/Typography";
import UsersTable from "./UsersTable";
import MeetingsTable from "./MeetingsTable";
import { Switch, Route } from "react-router-dom";
import MeetingCreator from "./MeetingCreator";
import DashboardBorder from "./DashboardBorder";
import { Get } from "restful-react";

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
  tableContainer: {
    marginBottom: 10
  },
  h5: {
    marginBottom: theme.spacing.unit * 2
  }
});

class SearchTable extends Component {

}

export default withStyles(SearchTable);