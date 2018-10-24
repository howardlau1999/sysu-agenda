import React, { Component } from "react";
import PropTypes from "prop-types";
import Avatar from "@material-ui/core/Avatar";
import Button from "@material-ui/core/Button";
import CssBaseline from "@material-ui/core/CssBaseline";
import FormControl from "@material-ui/core/FormControl";
import FormControlLabel from "@material-ui/core/FormControlLabel";
import Checkbox from "@material-ui/core/Checkbox";
import Input from "@material-ui/core/Input";
import InputLabel from "@material-ui/core/InputLabel";
import LockIcon from "@material-ui/icons/LockOutlined";
import Paper from "@material-ui/core/Paper";
import Typography from "@material-ui/core/Typography";
import withStyles from "@material-ui/core/styles/withStyles";
import MaterialUIForm from "react-material-ui-form";
import JssProvider from "react-jss/lib/JssProvider";
import { Get, Mutate } from "restful-react";
import AlertDialog from "./AlertDialog";
import DialogTitle from "@material-ui/core/DialogTitle";
import DialogContent from "@material-ui/core/DialogContent";
import DialogContentText from "@material-ui/core/DialogContent";
import FormHelperText from "@material-ui/core/FormHelperText";
import { Redirect } from "react-router-dom";

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

class Login extends Component {
  state = {
    dialogShow: false,
    redirectToReferrer: false,
    errorMessage: null
  };

  handleLoginBegin = () => {
    this.setState({
      errorMessage: null
    });
  };

  handleLoginSuccess = user => {
    localStorage.setItem("user_token", user.token);
    localStorage.setItem("loggined", true);
    console.log("Logged In");
    this.setState({
      redirectToReferrer: true
    });
  };

  handleLoginFailure = err => {
    this.setState({
      errorMessage: "Your credentials are invalid."
    });
  };

  handleDialogClose = () => {
    this.setState({
      dialogShow: false
    });
  };

  handleDialogOpen = () => {
    this.setState({
      dialogShow: true
    });
  };
  render() {
    const { classes } = this.props;
    const { from } = this.props.location.state || {
      from: { pathname: "/dashboard" }
    };
    const { redirectToReferrer } = this.state;

    if (redirectToReferrer) {
      return <Redirect to={from} />;
    }

    return (
      <React.Fragment>
        <CssBaseline />
        <main className={classes.layout}>
          <Paper className={classes.paper}>
            <Avatar className={classes.avatar}>
              <LockIcon />
            </Avatar>
            <Typography component="h1" variant="h5">
              Sign in
            </Typography>
            <JssProvider>
              <Get lazy path="/token-auth">
                {() => (
                  <Mutate verb="POST">
                    {(post, { loading: isPosting }) => (
                      <MaterialUIForm
                        className={classes.form}
                        onSubmit={(values, pristineValues) => {
                          this.handleLoginBegin();
                          post(values)
                            .then(this.handleLoginSuccess)
                            .catch(this.handleLoginFailure);
                        }}
                      >
                        <FormControl margin="normal" required fullWidth>
                          <InputLabel htmlFor="username">Username</InputLabel>
                          <Input
                            id="username"
                            name="username"
                            autoComplete="username"
                            autoFocus
                            value=""
                          />
                        </FormControl>
                        <FormControl margin="normal" required fullWidth>
                          <InputLabel htmlFor="password">Password</InputLabel>
                          <Input
                            name="password"
                            type="password"
                            id="password"
                            autoComplete="current-password"
                            value=""
                          />
                        </FormControl>
                        <FormControlLabel
                          control={
                            <Checkbox
                              name="remember"
                              value="yes"
                              color="primary"
                            />
                          }
                          label="Remember me"
                        />
                        {this.state.errorMessage ? (
                          <FormHelperText error={true}>
                            {this.state.errorMessage}
                          </FormHelperText>
                        ) : null}
                        <FormControl fullWidth>
                          <Button
                            type="submit"
                            variant="contained"
                            color="primary"
                            className={classes.submit}
                          >
                            Login
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

        <AlertDialog
          open={this.state.dialogShow}
          onClose={this.handleDialogClose}
        >
          <DialogTitle>Login Failed</DialogTitle>
          <DialogContent>
            <DialogContentText>
              Your credentials are invalid, please check your input.
            </DialogContentText>
          </DialogContent>
        </AlertDialog>
      </React.Fragment>
    );
  }
}

Login.propTypes = {
  classes: PropTypes.object.isRequired
};

export default withStyles(styles)(Login);
