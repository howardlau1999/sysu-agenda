import React from "react";
import { withStyles } from "@material-ui/core/styles";
import Table from "@material-ui/core/Table";
import TableBody from "@material-ui/core/TableBody";
import TableCell from "@material-ui/core/TableCell";
import TableHead from "@material-ui/core/TableHead";
import TableRow from "@material-ui/core/TableRow";
import Paper from "@material-ui/core/Paper";
import Get from "restful-react";

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
  render() {
    const { classes } = this.props;
    const meetingsType = this.props.meetingsType;
    return (
      <Get
        path={"/list/meetings/" + meetingsType}
        resolve={data => {
          console.log(data);
          return data.meetings;
        }}
        requestOptions={() => ({
          headers: {
            Authorization: "JWT " + localStorage.getItem("user_token")
          }
        })}
      >
        {meetings => (
          <Paper className={classes.root}>
            <Table className={classes.table}>
              <TableHead>
                <TableRow>
                  <TableCell>Title</TableCell>
                  <TableCell>Start Date</TableCell>
                  <TableCell>End Date</TableCell>
                  <TableCell>Sponsor</TableCell>
                  <TableCell>Participators</TableCell>
                </TableRow>
              </TableHead>
              <TableBody>
                {(meetings || []).map(meeting => (
                  <TableRow key={meeting.title}>
                    <TableCell>{meeting.title}</TableCell>
                    <TableCell>{meeting.start_date}</TableCell>
                    <TableCell>{meeting.end_date}</TableCell>
                    <TableCell>{meeting.sponsor}</TableCell>
                    <TableCell>{meeting.participators.join(" & ")}</TableCell>
                  </TableRow>
                ))}
              </TableBody>
            </Table>
          </Paper>
        )}
      </Get>
    );
  }
}

export default withStyles(styles)(MeetingsTable);
