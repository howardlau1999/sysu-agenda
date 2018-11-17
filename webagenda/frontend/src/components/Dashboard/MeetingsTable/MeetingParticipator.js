import React, { Component } from "react";

import Chip from "@material-ui/core/Chip";
import { Get, Mutate } from "restful-react";
import { connect } from "react-redux";
import { showSnackbarMessage } from "../../../redux/actions";
class MeetingParticipator extends Component {
  handleParticipatorDeleted = data => {
    if (data.success) {
      this.props.showSnackbarMessage("Participator was deleted successfully!");
      if (this.props.onDelete) this.props.onDelete();
    } else {
      this.props.showSnackbarMessage("Error deleting participator!");
    }
  };

  handleDeleteError = () => {
    this.props.showSnackbarMessage("Error deleting participator!");
  };
  render() {
    const { username, title, allowDelete } = this.props;
    return (
      <Get lazy path="/remove_participator">
        {() => (
          <Mutate
            verb="POST"
            requestOptions={() => ({
              headers: {
                Authorization: "JWT " + localStorage.getItem("user_token")
              }
            })}
          >
            {(post, { loading: isPosting }) => (
              <Chip
                label={username}
                key={username}
                onDelete={allowDelete ? () => {
                  post({
                    participator: username,
                    title: title
                  })
                    .then(this.handleParticipatorDeleted)
                    .catch(this.handleDeleteError);
                } : null}
              />
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
)(MeetingParticipator);
