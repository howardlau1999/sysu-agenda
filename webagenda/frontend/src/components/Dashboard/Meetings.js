import React, { Component } from "react";
import { withStyles } from "@material-ui/core/styles";
import Typography from "@material-ui/core/Typography";
import MeetingsTable from "./MeetingsTable";
import MeetingCreator from "./MeetingsTable/MeetingCreator";
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
class Meetings extends Component {
  state = {
    refresh: false
  };
  render() {
    const { classes } = this.props;
    return (
      <React.Fragment>
        <div>
          <div className={classes.appBarSpacer} />

          <Get
            path="/meetings/sponsor"
            resolve={data => {
              return data.meetings;
            }}
            requestOptions={() => ({
              headers: {
                Authorization: "JWT " + localStorage.getItem("user_token")
              }
            })}
          >
            {(meetings, states, { refetch }) => {
              return (
                <React.Fragment>
                  <Typography variant="h4" gutterBottom component="h2">
                    My Sponsored Meetings
                    <MeetingCreator
                      onSuccess={refetch}
                    />
                  </Typography>
                  <div className={classes.tableContainer}>
                    <MeetingsTable meetings={meetings} onDelete={refetch} />
                  </div>
                </React.Fragment>
              );
            }}
          </Get>

          <Typography variant="h4" gutterBottom component="h2">
            My Participated Meetings
          </Typography>
          <div className={classes.tableContainer}>
            <Get
              path="/meetings/participate"
              resolve={data => {
                return data.meetings;
              }}
              requestOptions={() => ({
                headers: {
                  Authorization: "JWT " + localStorage.getItem("user_token")
                }
              })}
            >
              {(meetings, states, { refetch }) => (
                <MeetingsTable
                  meetings={meetings}
                  onDelete={refetch}
                />
              )}
            </Get>
          </div>
        </div>
      </React.Fragment>
    );
  }
}

export default withStyles(styles)(Meetings);
