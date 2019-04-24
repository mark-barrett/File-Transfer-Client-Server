Basic Operation:
- The client initiates communication by connecting to an open socket setup by the server.
- The server tries to accept this connection, if the connection is succesful, the server will immediately send the "uid" prompt requsting the users uid.
- The client will send this and the server will then issue the "filename" prompt then the "path" prompt requiring the server to send both of them respectively.
- The server will then check to see if the user has access to that particular directory and if they do will issue the "sendfile" prompt, where the client will then send the entire contents of the file in blocks.
- The server will then save this to the appropriate file.
- The server will return "success" or "failure" messages throughout if problems are encountered.
