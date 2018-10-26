import React, { Component } from "react";

import DashboardAppBar from "./DashboardAppBar";
import DashboardDrawer from "./DashboardDrawer";

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
    return (
      <div>
        <DashboardAppBar
          open={this.state.drawerOpen}
          handleDrawerOpen={this.handleDrawerOpen}
        />
        <DashboardDrawer
          open={this.state.drawerOpen}
          handleDrawerClose={this.handleDrawerClose}
        />
      </div>
    );
  }
}

export default DashboardBorder;
