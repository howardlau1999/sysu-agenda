import React, { Component } from "react";
import { withStyles } from "@material-ui/core/styles";
import Typography from "@material-ui/core/Typography";
import MeetingsTable from "./MeetingsTable";
import { Get } from "restful-react";
import DateTimePickerInline from "material-ui-pickers/DateTimePicker/DateTimePickerInline";
import { DateTime } from "luxon";
import LuxonUtils from "material-ui-pickers/utils/luxon-utils";
import MuiPickersUtilsProvider from "material-ui-pickers/MuiPickersUtilsProvider";
import {
  InputAdornment,
  IconButton,
  FormControl,
  InputLabel,
  Input
} from "@material-ui/core";
import SearchIcon from "@material-ui/icons/Search";
const styles = theme => ({
  root: {
    display: "flex"
  },
  textInput: {
    margin: 20
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
class Query extends Component {
  state = {
    refresh: false,
    query_title: null,
    query_start: DateTime.local(),
    query_end: DateTime.local()
  };
  handleStartTimeChange = date => {
    this.setState({
      query_start: date
    });
  };
  handleEndTimeChange = date => {
    this.setState({
      query_end: date
    });
  };
  render() {
    const { classes } = this.props;
    return (
      <React.Fragment>
        <div>
          <div className={classes.appBarSpacer} />

          <Typography variant="h4" gutterBottom component="h2">
            Query by Title
          </Typography>
          <div className={classes.tableContainer}>
            <Get
              path="/meetings/"
              resolve={data => {
                return data.meetings;
              }}
              requestOptions={() => ({
                headers: {
                  Authorization: "JWT " + localStorage.getItem("user_token")
                }
              })}
              lazy
            >
              {(meetings, states, { refetch }) => (
                <div>
                  <FormControl className={classes.textInput}>
                    <InputLabel htmlFor="queryTitle">Meeting Title</InputLabel>
                    <Input
                      id="queryTitle"
                      name="queryTitle"
                      onChange={e => {
                        this.setState({
                          query_title: e.target.value
                        });
                      }}
                      endAdornment={
                        <InputAdornment position="end">
                          <IconButton
                            onClick={() => {
                              refetch(
                                "/meeting/" + this.state.query_title
                              ).catch(console.log);
                            }}
                          >
                            <SearchIcon />
                          </IconButton>
                        </InputAdornment>
                      }
                    />
                  </FormControl>
                  <MeetingsTable
                    meetings={meetings}
                    onDelete={() => {
                      this.setState({
                        refresh: true
                      });
                    }}
                  />
                </div>
              )}
            </Get>
          </div>

          <Typography variant="h4" gutterBottom component="h2">
            Query by Time Interval
          </Typography>

          <div className={classes.tableContainer}>
            <Get
              resolve={data => {
                return data.meetings;
              }}
              requestOptions={() => ({
                headers: {
                  Authorization: "JWT " + localStorage.getItem("user_token")
                }
              })}
              lazy
            >
              {(meetings, states, { refetch }) => (
                <div>
                  <MuiPickersUtilsProvider utils={LuxonUtils}>
                    <FormControl
                      margin="normal"
                      required
                      className={classes.textInput}
                    >
                      <DateTimePickerInline
                        required
                        label="Start Time"
                        value={this.state.query_start}
                        onChange={this.handleStartTimeChange}
                        id="start_date"
                        name="start_date"
                      />
                    </FormControl>
                    <FormControl
                      margin="normal"
                      required
                      className={classes.textInput}
                    >
                      <DateTimePickerInline
                        required
                        label="End Time"
                        value={this.state.query_end}
                        onChange={this.handleEndTimeChange}
                        id="end_date"
                        name="end_date"
                      />
                    </FormControl>
                    <FormControl margin="normal" className={classes.textInput}>
                      <IconButton>
                        <SearchIcon
                          onClick={() => {
                            refetch(
                              "/meetings/query_meeting_by_date?start_date=" +
                                encodeURIComponent(
                                  this.state.query_start.toFormat(
                                    "yyyy-MM-dd/HH:mm"
                                  )
                                ) +
                                "&end_date=" +
                                encodeURIComponent(
                                  this.state.query_end.toFormat(
                                    "yyyy-MM-dd/HH:mm"
                                  )
                                )
                            );
                          }}
                        />
                      </IconButton>
                    </FormControl>
                  </MuiPickersUtilsProvider>
                  <MeetingsTable
                    meetings={meetings}
                    onDelete={() => {
                      this.setState({
                        refresh: true
                      });
                    }}
                  />
                </div>
              )}
            </Get>
          </div>
        </div>
      </React.Fragment>
    );
  }
}

export default withStyles(styles)(Query);
