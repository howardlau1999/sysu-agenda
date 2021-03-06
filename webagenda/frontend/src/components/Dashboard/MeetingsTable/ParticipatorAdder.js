import React, { Component } from "react";
import LuxonUtils from "material-ui-pickers/utils/luxon-utils";
import MuiPickersUtilsProvider from "material-ui-pickers/MuiPickersUtilsProvider";
import MaterialUIForm from "react-material-ui-form";
import JssProvider from "react-jss/lib/JssProvider";
import { Get, Mutate } from "restful-react";
import CssBaseline from "@material-ui/core/CssBaseline";
import FormControl from "@material-ui/core/FormControl";
import Button from "@material-ui/core/Button";
import Paper from "@material-ui/core/Paper";
import MenuItem from "@material-ui/core/MenuItem";
import withStyles from "@material-ui/core/styles/withStyles";
import { DateTime } from "luxon";
import Autosuggest from "react-autosuggest";
import match from "autosuggest-highlight/match";
import parse from "autosuggest-highlight/parse";
import Dialog from "@material-ui/core/Dialog";
import DialogTitle from "@material-ui/core/DialogTitle";
import DialogContent from "@material-ui/core/DialogContent";

import IconButton from "@material-ui/core/IconButton";
import AddCircleOutlineIcon from "@material-ui/icons/AddCircleOutline";
import { connect } from "react-redux";
import { showSnackbarMessage } from "../../../redux/actions";
import { TextField } from "@material-ui/core";
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
  },

  container: {
    flexGrow: 1,
    position: "relative",
    zIndex: 1000
  },
  suggestionsContainerOpen: {
    position: "absolute",
    marginTop: theme.spacing.unit,
    marginBottom: theme.spacing.unit * 3,
    left: 0,
    right: 0,
    zIndex: 100
  },
  dialog: {
    height: 500
  },
  suggestion: {
    zIndex: 100,
    display: "block"
  },
  suggestionsList: {
    margin: 0,
    padding: 0,
    zIndex: 100,
    listStyleType: "none"
  },
  textField: {
    width: "100%"
  }
});
function renderSuggestion(suggestion, { query, isHighlighted }) {
  const matches = match(suggestion.username, query);
  const parts = parse(suggestion.username, matches);

  return (
    <MenuItem
      selected={isHighlighted}
      component="div"
      onMouseDown={e => e.preventDefault()} // prevent the click causing the input to be blurred
    >
      <div>
        {parts.map((part, index) => {
          return part.highlight ? (
            <span key={String(index)} style={{ fontWeight: 300 }}>
              {part.text}
            </span>
          ) : (
            <strong key={String(index)} style={{ fontWeight: 500 }}>
              {part.text}
            </strong>
          );
        })}
      </div>
    </MenuItem>
  );
}

function renderSuggestionsContainer(options) {
  const { containerProps, children } = options;

  return (
    <Paper {...containerProps} square>
      {children}
    </Paper>
  );
}

function getSuggestionValue(suggestion) {
  return suggestion.username;
}
function renderInput(inputProps) {
  const {
    classes,
    autoFocus,
    value,
    onChange,
    onAdd,
    onDelete,
    chips,
    ref,
    ...other
  } = inputProps;
  return (
    <TextField
      fullWidth
      label="Participator"
      onChange={onChange}
      value={value}
      inputRef={ref}
      {...other}
    />
  );
}
class ParticipatorAdder extends Component {
  handleCreateBegin = () => {};

  handleCreateSuccess = result => {
    if (result.success) {
      this.handleMeetingCreatorClose();
      this.props.onSuccess();
      this.props.showSnackbarMessage("Add participator successfully!");
    } else this.props.showSnackbarMessage("Add participator failed!");
  };

  handleCreateError = err => {
    console.log(err);
  };

  handleParticipatorsAdd = participator => {
    this.setState({
      textFieldInput: participator
    });
  };

  getSuggestions = value => {
    const inputValue = value.trim().toLowerCase();
    const inputLength = inputValue.length;
    let count = 0;
    return inputLength === 0
      ? []
      : this.userList.filter(suggestion => {
          const keep =
            count < 3 &&
            suggestion.username.toLowerCase().slice(0, inputLength) ===
              inputValue;

          if (keep) {
            count += 1;
          }

          return keep;
        });
  };

  handleSuggestionsFetchRequested = ({ value }) => {
    this.setState({
      participatorsSuggestions: this.getSuggestions(value)
    });
  };

  handleSuggestionsClearRequested = () => {
    this.setState({
      participatorsSuggestions: []
    });
  };

  handletextFieldInputChange = (event, { newValue }) => {
    this.setState({
      participator: newValue
    });
  };

  userList = [];

  state = {
    startTime: DateTime.local(),
    endTime: DateTime.local(),
    participator: "",
    participatorsSuggestions: [],
    textFieldInput: "",
    meetingCreatorOpen: false
  };

  handleMeetingCreatorOpen = () => {
    this.setState({ meetingCreatorOpen: true });
  };

  handleMeetingCreatorClose = () => {
    this.setState({ meetingCreatorOpen: false });
  };
  render() {
    const { classes, is_query } = this.props;

    return (
      <React.Fragment>
        <IconButton onClick={this.handleMeetingCreatorOpen}>
          <AddCircleOutlineIcon />
        </IconButton>
        <Dialog
          open={this.state.meetingCreatorOpen}
          onClose={this.handleMeetingCreatorClose}
        >
          <DialogTitle>Add Participator</DialogTitle>
          <DialogContent>
            <MuiPickersUtilsProvider utils={LuxonUtils}>
              <CssBaseline />
              <Get
                path={is_query ? "/../users" : "../../../users"}
                requestOptions={() => ({
                  headers: {
                    Authorization: "JWT " + localStorage.getItem("user_token")
                  }
                })}
              >
                {data => {
                  if (data) {
                    this.userList = data.users;
                  }
                  return null;
                }}
              </Get>

              <JssProvider>
                <Get lazy path={(is_query ? "/sponsor" : "") + "/add_participator"}>
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
                          onSubmit={() => {
                            let data = {
                              title: this.props.title,
                              participator: this.state.participator
                            };
                            this.handleCreateBegin();
                            post(data)
                              .then(this.handleCreateSuccess)
                              .catch(this.handleCreateError);
                          }}
                        >
                          <FormControl margin="normal" required fullWidth>
                            <Autosuggest
                              theme={{
                                container: classes.container,
                                suggestionsContainerOpen:
                                  classes.suggestionsContainerOpen,
                                suggestionsList: classes.suggestionsList,
                                suggestion: classes.suggestion
                              }}
                              renderInputComponent={renderInput}
                              suggestions={this.state.participatorsSuggestions}
                              onSuggestionsFetchRequested={
                                this.handleSuggestionsFetchRequested
                              }
                              onSuggestionsClearRequested={
                                this.handleSuggestionsClearRequested
                              }
                              focusInputOnSuggestionClick={true}
                              renderSuggestionsContainer={
                                renderSuggestionsContainer
                              }
                              getSuggestionValue={getSuggestionValue}
                              renderSuggestion={renderSuggestion}
                              onSuggestionSelected={(
                                e,
                                { suggestionValue }
                              ) => {
                                this.handleParticipatorsAdd(suggestionValue);
                                e.preventDefault();
                              }}
                              inputProps={{
                                classes,
                                onChange: this.handletextFieldInputChange,
                                value: this.state.participator,
                              }}
                            />
                            <Button
                              type="submit"
                              variant="contained"
                              color="primary"
                              className={classes.submit}
                            >
                              Add Participator
                            </Button>
                          </FormControl>
                        </MaterialUIForm>
                      )}
                    </Mutate>
                  )}
                </Get>
              </JssProvider>
            </MuiPickersUtilsProvider>
          </DialogContent>
        </Dialog>
      </React.Fragment>
    );
  }
}

export default connect(
    null,
    { showSnackbarMessage }
  )(withStyles(styles)(ParticipatorAdder));
