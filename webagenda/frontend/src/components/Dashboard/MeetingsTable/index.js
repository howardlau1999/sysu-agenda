import React from "react";
import { withStyles } from "@material-ui/core/styles";
import Table from "@material-ui/core/Table";
import TableBody from "@material-ui/core/TableBody";
import TableCell from "@material-ui/core/TableCell";
import TableHead from "@material-ui/core/TableHead";
import TableRow from "@material-ui/core/TableRow";
import Paper from "@material-ui/core/Paper";
import { Get} from "restful-react";
import Chip from "@material-ui/core/Chip";
import MeetingDeleter from "./MeetingDeleter";
const styles = {
  root: {
    width: "100%",
    overflowX: "auto"
  },
  table: {
    minWidth: 700
  }
};

class MeetingsTable extends React.Component {
  state = {
    refresh: false
  };
  render() {
    const { classes } = this.props;
    const meetingsType = this.props.meetingsType;
    return (
      <Paper className={classes.root}>
        <Table className={classes.table}>
          <TableHead>
            <TableRow>
              <TableCell>Title</TableCell>
              <TableCell>Start Date</TableCell>
              <TableCell>End Date</TableCell>
              <TableCell>Sponsor</TableCell>
              <TableCell>Participators</TableCell>
              {meetingsType === "sponsor" ? (
                <TableCell>Actions</TableCell>
              ) : null}
            </TableRow>
          </TableHead>

          <TableBody>
            <Get
              path={"/meetings/" + meetingsType}
              resolve={data => {
                return data.meetings;
              }}
              requestOptions={() => ({
                headers: {
                  Authorization: "JWT " + localStorage.getItem("user_token")
                }
              })}
            >
              {meetings =>
                (meetings || []).map(meeting => (
                  <TableRow key={meeting.title}>
                    <TableCell>{meeting.title}</TableCell>
                    <TableCell>{meeting.start_date}</TableCell>
                    <TableCell>{meeting.end_date}</TableCell>
                    <TableCell>{meeting.sponsor}</TableCell>
                    <TableCell>
                      {meeting.participators
                        ? meeting.participators.map(username => (
                            <Chip label={username} key={username} />
                          ))
                        : null}
                    </TableCell>
                    {meetingsType === "sponsor" ? (
                      <TableCell>
                        <MeetingDeleter meeting={meeting} onDelete={this.props.onDelete} />
                      </TableCell>
                    ) : null}
                  </TableRow>
                ))
              }
            </Get>
          </TableBody>
        </Table>
      </Paper>
    );
  }
}

export default withStyles(styles)(MeetingsTable);
