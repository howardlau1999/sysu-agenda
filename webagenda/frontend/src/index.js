import React from "react";
import ReactDOM from "react-dom";
import "./index.css";
import Agenda from "./components/Agenda";
import * as serviceWorker from "./serviceWorker";
import { RestfulProvider } from "restful-react";
import { Provider } from "react-redux";
import store from "./redux/store";

ReactDOM.render(
  <Provider store={store}>
    <RestfulProvider base="http://127.0.0.1:8000/api/v1">
      <Agenda />
    </RestfulProvider>
    </Provider>,
  document.getElementById("root")
);

// If you want your app to work offline and load faster, you can change
// unregister() to register() below. Note this comes with some pitfalls.
// Learn more about service workers: http://bit.ly/CRA-PWA
serviceWorker.unregister();
