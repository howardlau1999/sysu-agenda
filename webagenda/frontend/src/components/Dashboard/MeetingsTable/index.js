import React from "react";
import { withStyles } from "@material-ui/core/styles";
import Table from "@material-ui/core/Table";
import TableBody from "@material-ui/core/TableBody";
import TableCell from "@material-ui/core/TableCell";
import TableHead from "@material-ui/core/TableHead";
import TableRow from "@material-ui/core/TableRow";
import Paper from "@material-ui/core/Paper";
import MeetingDeleter from "./MeetingDeleter";
import MeetingQuiter from "./MeetingQuiter";
import MeetingParticipator from "./MeetingParticipator";
import ParticipatorAdder from "./ParticipatorAdder";
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
    const meetings = this.props.meetings;
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
              <TableCell>Actions</TableCell>
            </TableRow>
          </TableHead>

          <TableBody>
            
              {
                (meetings || []).map(meeting => (
                  <TableRow key={meeting.title}>
                    <TableCell>{meeting.title}</TableCell>
                    <TableCell>{meeting.start_date}</TableCell>
                    <TableCell>{meeting.end_date}</TableCell>
                    <TableCell>{meeting.sponsor}</TableCell>
                    <TableCell>
                      <div>
                      {meeting.participators
                        ? meeting.participators.map(username => (
                            <MeetingParticipator
                              username={username}
                              title={meeting.title}
                              onDelete={this.props.onDelete}
                              allowDelete={meeting.is_sponsor}
                              key={username}
                            />
                          ))
                        : null}
                        {meeting.is_sponsor && (<ParticipatorAdder title={meeting.title} onSuccess={this.props.onDelete} />)}
                        </div>
                    </TableCell>

                    <TableCell>
                      {meeting.is_sponsor ? (
                        <MeetingDeleter
                          meeting={meeting}
                          onDelete={this.props.onDelete}
                        />
                      ) : (
                        <MeetingQuiter
                          meeting={meeting}
                          onDelete={this.props.onDelete}
                        />
                      )}
                    </TableCell>
                  </TableRow>
                ))
              }
            
          </TableBody>
        </Table>
      </Paper>
    );
  }
}

export default withStyles(styles)(MeetingsTable);
