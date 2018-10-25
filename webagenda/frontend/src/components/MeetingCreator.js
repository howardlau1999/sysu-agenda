import React, { Component } from "react";

import LuxonUtils from "material-ui-pickers/utils/luxon-utils";
import MuiPickersUtilsProvider from "material-ui-pickers/utils/MuiPickersUtilsProvider";
import MaterialUIForm from "react-material-ui-form";
import JssProvider from "react-jss/lib/JssProvider";
import { Get, Mutate } from "restful-react";
import CssBaseline from "@material-ui/core/CssBaseline";
import FormControl from "@material-ui/core/FormControl";
import Input from "@material-ui/core/Input";
import InputLabel from "@material-ui/core/InputLabel";
import Button from "@material-ui/core/Button";
import Paper from "@material-ui/core/Paper";
import withStyles from "@material-ui/core/styles/withStyles";
import DateTimePickerInline from "_material-ui-pickers@1.0.0-rc.17@material-ui-pickers/DateTimePicker/DateTimePickerInline";
import {DateTime} from "luxon";
const styles = theme => ({
  layout: {
    width: "auto",
    display: "block", // Fix IE 11 issue.
    marginLeft: theme.spacing.unit * 3,
    marginRight: theme.spacing.unit * 3,
    [theme.breakpoints.up(400 + theme.spacing.unit * 3 * 2)]: {
      width: 400,
      marginLeft: "auto",
      marginRight: "auto"
    }
  },
  paper: {
    marginTop: theme.spacing.unit * 8,
    display: "flex",
    flexDirection: "column",
    alignItems: "center",
    padding: `${theme.spacing.unit * 2}px ${theme.spacing.unit * 3}px ${theme
      .spacing.unit * 3}px`
  },
  avatar: {
    margin: theme.spacing.unit,
    backgroundColor: theme.palette.secondary.main
  },
  form: {
    width: "100%", // Fix IE 11 issue.
    marginTop: theme.spacing.unit
  },
  submit: {
    marginTop: theme.spacing.unit * 3
  }
});

class MeetingCreator extends Component {
  handleCreateBegin = () => {};
  handleCreateSuccess = () => {};
  handleCreateError = err => {
    console.log(err);
  };
  handleStartTimeChange = date => {
    this.setState({
      startTime: date
    });
  };
  handleEndTimeChange = date => {
    this.setState({
      endTime: date
    });
  };

  state = {
    startTime: DateTime.local(),
    endTime: DateTime.local()
  };
  render() {
    const { classes } = this.props;
    return (
      <MuiPickersUtilsProvider utils={LuxonUtils}>
        <CssBaseline />
        <main className={classes.layout}>
          <Paper className={classes.paper}>
            <JssProvider>
              <Get lazy path="/create_meeting">
                {() => (
                  <Mutate
                    verb="POST"
                    requestOptions={() => ({
                      headers: {
                        Authorization:
                          "JWT " + localStorage.getItem("user_token")
                      }
                    })}
                  >
                    {(post, { loading: isPosting }) => (
                      <MaterialUIForm
                        className={classes.form}
                        onSubmit={(values, pristineValues) => {
                        
                          let data = {
                            meeting: {
                              title: values.title,
                              start_date:  this.state.startTime.toFormat("yyyy-MM-dd/HH:mm"),
                              end_date: this.state.endTime.toFormat("yyyy-MM-dd/HH:mm"),
                              
                              participators: []
                            }
                          };
                          this.handleCreateBegin();
                          post(data)
                            .then(this.handleCreateSuccess)
                            .catch(this.handleCreateError);
                        }}
                      >
                        <FormControl margin="normal" required fullWidth>
                          <InputLabel htmlFor="title">Title</InputLabel>
                          <Input id="title" name="title" autoFocus value="" />
                        </FormControl>
                        <FormControl margin="normal" required fullWidth>
                          <DateTimePickerInline
                            label="Start Time"
                            value={this.state.startTime}
                            onChange={this.handleStartTimeChange}
                            id="start_date"
                            name="start_date"
                          />
                        </FormControl>
                        <FormControl margin="normal" required fullWidth>
                          <DateTimePickerInline
                            label="End Time"
                            value={this.state.endTime}
                            onChange={this.handleEndTimeChange}
                            id="end_date"
                            name="end_date"
                          />
                        </FormControl>
                        <FormControl fullWidth>
                          <Button
                            type="submit"
                            variant="contained"
                            color="primary"
                            className={classes.submit}
                          >
                            Create Meeting
                          </Button>
                        </FormControl>
                      </MaterialUIForm>
                    )}
                  </Mutate>
                )}
              </Get>
            </JssProvider>
          </Paper>
        </main>
      </MuiPickersUtilsProvider>
    );
  }
}

export default withStyles(styles)(MeetingCreator);
