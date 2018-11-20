import React, { Component } from "react";

import DashboardAppBar from "./DashboardAppBar";
import DashboardDrawer from "./DashboardDrawer";
import { withStyles } from '@material-ui/core/styles';
import CssBaseline from '@material-ui/core/CssBaseline';
const styles = theme => ({
  root: {
    display: 'flex',
  },
});


class DashboardBorder extends Component {
  state = {
    drawerOpen: true
  };

  handleDrawerOpen = () => {
    this.setState({ drawerOpen: true });
  };

  handleDrawerClose = () => {
    this.setState({ drawerOpen: false });
  };
  render() {
    const { classes } = this.props;
    return (
      <React.Fragment>
        <CssBaseline />
          <div className={classes.root}>
            <DashboardAppBar
               open={this.state.drawerOpen}
               handleDrawerOpen={this.handleDrawerOpen}
             />
             <DashboardDrawer
               open={this.state.drawerOpen}
               handleDrawerClose={this.handleDrawerClose}
             />
          </div>
      </React.Fragment>
    );
  }
}

export default withStyles(styles)(DashboardBorder);
