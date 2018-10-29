import React from "react";
import { Get, Mutate } from "restful-react";
import DeleteIcon from "@material-ui/icons/Delete";
import { IconButton } from "@material-ui/core";
import { connect } from "react-redux";
import { showSnackbarMessage } from "../../../redux/actions";
class MeetingQuitter extends React.Component {
  render() {
    const { meeting } = this.props;
    return (
      <Get lazy path="">
        {() => (
          <Mutate
            verb="DELETE"
            requestOptions={() => ({
              headers: {
                Authorization: "JWT " + localStorage.getItem("user_token")
              }
            })}
          >
            {(del, { loading: isDeleteting }) => (
              <IconButton
                onClick={() => {
                  del(meeting.title)
                    .then(data => {
                      if (data.success) {
                        this.props.showSnackbarMessage(
                          "Quitted meeting " +
                            meeting.title 
                        );
                        this.props.onDelete();
                      }
                    })
                    .catch(() => {
                      this.props.showSnackbarMessage(
                        "Error quitting meeting " + meeting.title
                      );
                    });
                }}
              >
                <DeleteIcon />
              </IconButton>
            )}
          </Mutate>
        )}
      </Get>
    );
  }
}

export default connect(
  null,
  { showSnackbarMessage }
)(MeetingQuitter);
