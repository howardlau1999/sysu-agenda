import React, { Component } from "react";
import PropTypes from "prop-types";
import Avatar from "@material-ui/core/Avatar";
import Button from "@material-ui/core/Button";
import CssBaseline from "@material-ui/core/CssBaseline";
import FormControl from "@material-ui/core/FormControl";
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

class Register extends Component {
  state = {
    dialogShow: false,
    redirectToReferrer: false,
    errorMessage: null
  };

  handleRegisterBegin = () => {
    this.setState({
      errorMessage: null
    });
  };

  handleRegisterResult = result => {
    if (result.success) {
        this.props.showSnackMessage("Register success!");
       this.setState({
        redirectToReferrer: true
      }); 
    }
      
    else
      this.setState({
        errorMessage: "Register failed"
      });
  };

  handleRegisterError = err => {
    this.setState({
      errorMessage: err.data.detail
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
      from: { pathname: "/login" }
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
              Register
            </Typography>
            <JssProvider>
              <Get lazy path="/register">
                {() => (
                  <Mutate verb="POST">
                    {(post, { loading: isPosting }) => (
                      <MaterialUIForm
                        className={classes.form}
                        onSubmit={(values, pristineValues) => {
                          this.handleRegisterBegin();
                          post({ user: values })
                            .then(this.handleRegisterResult)
                            .catch(this.handleRegisterError);
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
                        <FormControl margin="normal" required fullWidth>
                          <InputLabel htmlFor="email">E-mail</InputLabel>
                          <Input
                            name="email"
                            type="email"
                            id="email"
                            autoComplete="email"
                            value=""
                          />
                        </FormControl>
                        <FormControl margin="normal" required fullWidth>
                          <InputLabel htmlFor="phone">Phone</InputLabel>
                          <Input
                            name="phone"
                            type="phone"
                            id="phone"
                            autoComplete="phone"
                            value=""
                          />
                        </FormControl>
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
                            Register
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

Register.propTypes = {
  classes: PropTypes.object.isRequired
};

export default withStyles(styles)(Register);
