import React, { Component } from "react";
import Login from './Login';
import Dashboard from './Dashboard';
import { BrowserRouter, Route } from "react-router-dom";
class Agenda extends Component {
  render() {
    return (
      <BrowserRouter>
        <div className="Agenda">
          <Route path="/dashboard" component={Dashboard} />
          <Route path="/login" component={Login} />
        </div>
      </BrowserRouter>
    );
  }
}

export default Agenda;
